#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

struct Estudiante{
    int codigo;
    char sexo[15];
    char nombre[20];
    char apellido[20];
    int edad;
    char carrera[35];
    int ciclo;
    long next;
    long prev;

    Estudiante() = default;

    Estudiante(int codigo, const string& sexo, const string& nombre, const string& apellido, int edad, const string& carrera, int ciclo){
        this->codigo = codigo;
        strcpy(this->sexo, sexo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->apellido, apellido.c_str());
        this->edad = edad;
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
    }

    void print(Estudiante &record){
        cout<<record.codigo<<" "<<record.sexo<<" "<<record.nombre<<" "<<record.apellido<<" "<<record.edad<<" "<<record.carrera<<" "<<record.ciclo<<" ";
        cout<<endl;
    }
};

class Sequential_File{
    private:
        string file_name;
        string sequential_file_name;
        string aux_file_name;
        string header_file_name;
        int registros_ordenados;
        int registros_desordenados;

        long get_file_size(string file){
            ifstream file_name(file, ios::ate | ios::binary);
            long size = file_name.tellg();
            file_name.close();
            return size;
        }

        void iniciar_sequential(){
            ifstream input(this->file_name, ios::in);
            ofstream sequential(this->sequential_file_name, ios::out);

            Estudiante record;
            long actual_next = 1;
            long actual_prev = -1;

            long total_records = get_file_size(this->file_name) / (sizeof(Estudiante) - 2 * sizeof(long));
            while(input.read((char*)& record, sizeof(Estudiante) - 2 * sizeof(long))){
                record.next = total_records == actual_next ? -2 : actual_next++;
                record.prev = actual_prev++;
                sequential.write((char*)& record, sizeof(Estudiante))
            }
            registros_ordenados = total_records;
            registros_desordenados = 0;
        }

        Estudiante read(string file, long pos){
            fstream file(file);
            Estudiante record;
            file.seekg(pos * sizeof(Estudiante));
            file.read((char*)& record, sizeof(Estudiante));
            file.close()
            return record;
        }

        void write(Estudiante record, string file, long pos){
            fstream file(file);
            file.seekp(pos * sizeof(Estudiante));
            file.read((char*)& record, sizeof(Estudiante));
            file.close()
        }

        Estudiante get_prev_estudiante(Estudiante record){
            if(record.next == -2){
                record = this->read(this->sequential_file_name, registros_ordenados - 2);
            }
            else{
                record = this->read(this->sequential_file_name, record.next);
                record = this->read(this->sequential_file_name, record.prev - 1);
            }
            return record;
        }

        long read_header(){
            fstream header(this->header_file_name);
            header.seekg(0);
            long pos;
            header.read((char*)& pos, sizeof(pos));
            header.close();
            return pos;
        }

        void write_header(long pos){
            fstream header(this->header_file_name);
            header.seekp(0);
            header.write((char*)& pos, sizeof(pos));
            header.close()
        }

        long pos_primer_registro(){
            long current_record_pos = 0;
            Estudiante current = this->read(this->sequential_file_name, current_record_pos);
            while(current.prev == -1){
                current = this->read(this->sequential_file_name, ++current_record_pos);
            }
            return current_record_pos - 1;
        }

        //Busca donde insertar pero en con registros desordenados, considerando una free list
        void pos_insert(){
            long current_header = read_header(); //Leemos el header
            if(current_header == -1){
                return registros_ordenados + registros_desordenados;
            }
            else{
                Estudiante borrado = read(this->sequential_file_name, current_header); //Recuperamos el registro que fue apagado
                write_header(borrado.next); //El siguiente espacio vacio
                return current_header;
            }
        }

        void insert_basico(Estudiante base, Estudiante record){
            Estudiante base_next = this->read(this->sequential_file_name, base.next); //Obtenemos el registro siguiente del base
            long location = this->pos_insert(); //Buscamos el espacio para insertar
            long base_pos = base_next.prev; //obtenemos la posicion del registro previo
            long base_pos_next = base.next; //obtenemos la posicion del siguiente del base

            //intercambio (pensarlo como una linked list)
            base.next = location;  
            record.prev = base_pos;
            record.next = base_pos_next;
            base_next.prev = location;

            this->write(base, this->sequential_file_name, base_pos);
            this->write(record, this->sequential_file_name, location);
            this->write(base_next, this->sequential_file_name, base_pos_next);
        }

        //Insercion a la izquierda del actual registro
        void insert_punteros_actualizados(Estudiante record, Estudiante current){
            //Obtenemos el estudiante previo
            long prev_record_pos = current.prev;
            Estudiante prevEstudiante = this->read(this->sequential_file_name, prev_record_pos);

            //Posicion (tipo array) del actual registro
            long current_pos = prevEstudiante.next;
            //Obtenemos la posicion para insertar
            long location = this->pos_insert();

            //Actualizamos punteros
            //actualizamos el siguiente del previo
            prevEstudiante.next = location;
            this->write(prevEstudiante, this->sequential_file_name, prev_record_pos);

            //Configuramos los punteros del regiistro a insertar
            record.next = current_pos;
            record.prev = prev_record_pos;
            this->write(record, this->sequential_file_name, location);

            //Actualiziamos el previo del actual registro
            current.prev = location;
            this->write(current, this->sequential_file_name, current_pos);
        }

        void insertar_inicio(Estudiante record){
            Estudiante primer_registro = this->read(this->sequential_file_name, this->pos_primer_registro());
            Estudiante siguiente_registro = this->read(this->sequential_file_name, primer_registro.next);

            long location = this->pos_insert(); //Obtengo la posicion de insercion
            record.next = location;
            siguiente_registro.prev = location; //su previo ahora tiene como localizacion donde antes estaba el primer registro
            record.prev = -1; //Su previo no existe

            primer_registro.prev = 0; //Posicion inicial

            this->write(record, this->sequential_file_name, this->pos_primer_registro());
            this->write(primer_registro, this->sequential_file_name, location);
            this->write(siguiente_registro, this->sequential_file_name, primer_registro.next)
        }

        void insertar_final(Estudiante record){
            Estudiante ultimo_registro = this->read(this->sequential_file_name, registros_ordenados - 1);
            record.next = -2;
            record.prev = registros_ordenados - 1;

            long location = this->pos_insert();
            ultimo_registro.next = location;

            this->write(ultimo_registro, this->sequential_file_name, registros_ordenados - 1);
            this->write(record, this->sequential_file_name, location);
        }

        void insert_despues_null(Estudiante current, Estudiante record){
            Estudiante previo_actual = this->read(this->sequential_file_name, current.prev); // 
            record.next = -2;
            record.prev = previo_actual.next;
            long location = pos_insert();
            current.next = location;

            this->write(current,this->sequential_file_name,previo_actual.next);
            this->write(record,this->sequential_file_name,location)
        }

        void rebuild_insertar(){
            long total_registros = get_file_size(this->sequential_file_name) / sizeof(Estudiante);
            this->rebuild(total_registros);
            registros_ordenados += 5;
            registros_desordenados = 0;
        }

    public:
};

/*
using namespace std;

enum file_type{
    data,
    aux,
    none
};

struct location{
    long position;
    file_type file;
};

struct Record{
    int codigo;
    char sexo[15];
    char nombre[20];
    char apellido[20];
    int edad;
    char carrera[35];
    int ciclo;
    long next{}; //puntero siguiente
    file_type next_file = file_type::data; //Archivo en donde se encuentra el registro

    int get_codigo(){
        //return std::stoi(this->codigo); //lo retorna como entero
        return this->codigo;
    }

    void load_data(vector<string> data){
        string cod = data[0];
        string se = data[1];
        string nom = data[2];
        string ap = data[3];
        string ed = data[4];
        string car = data[5];
        string ci = data[6];
        
        //char *codigo1 = new char [cod.length()+1];
        char *sexo1 = new char [se.length()+1];
        char *nombre1 = new char [nom.length()+1];
        char *apellido1 = new char [ap.length()+1];
        //char *edad1 = new char [ed.length()+1];
        char *carrera1 = new char [car.length()+1];
        //char *ciclo1 = new char [ci.length()+1];
        
        if(data.size() == 7){
            //strcpy(codigo1, cod.c_str());
            strcpy(sexo1, se.c_str());
            strcpy(nombre1, nom.c_str());
            strcpy(apellido1, ap.c_str());
            //strcpy(edad1, ed.c_str());
            strcpy(carrera1, car.c_str());
            //strcpy(ciclo1, ci.c_str());

            codigo = stoi(cod);
            strcpy(sexo,sexo1);
            strcpy(nombre,nombre1);
            strcpy(apellido,apellido1);
            edad = stoi(ed);
            strcpy(carrera,carrera1);
            ciclo = stoi(ci);
        }
    }
};

class Sequential_File{
    private:
        string data_file;
        string aux_file;
        int aux_size; //Maximo tamano del archivo auxiliar
    public:
        Sequential_File(string dataFile = "seq-data.dat"){
            this->data_file = dataFile;
            this->aux_file = "seq-aux.dat";
            this->aux_size = 5;
        }
    
    bool insert(Record); //Insertar registro
    bool remove(int key); //Remueve registro
    Record search(int); //Busca el registro con una determinado codigo
    vector<Record> range_search(int beg, int end); //Busqueda de registros por rango
    void merge(); //Mezcla el archivo aux con el data
    void print_records(); //Imprime los registros
    void load_data(const string& file_csv);
};

//Funcion para leer en un archivo determinado
bool readRecord(Record &record, fstream &file){
    file.read((char*)&record, sizeof(record));
    return file.good();
}

//Funcion para escribir en un archivo determinado
bool writeRecord(Record &record, fstream& file){
    file.write((char*)&record, sizeof(record));
    return file.good();
}

//Funcion para tener el tamano del archivo
long get_file_size(fstream& file){
    auto pos = file.tellg(); //Guardamos la posicion donde nos encontramos
    file.seekg(0, ios::end); //Nos ubicamos al final del archivo
    auto size = file.tellg(); //Recuperamos la posicion
    file.seekg(pos); //Volvemos a la posicion donde estabamos
    return size;
}

//Imprime el registro
void print_record(Record &record){
    cout<<record.codigo<<" "<<record.sexo<<" "<<record.nombre<<" "<<record.apellido<<" "<<record.edad<<" "<<record.carrera<<" "<<record.ciclo<<" ";
    cout<<endl;
}

//Buscara el key en los archivos correspondientes y devolvera informacion acerca de la posicion y en que archivo se encuentra
location find(int key, fstream& data, fstream& aux){
    long record_size = sizeof(Record);
    long ini = 0;
    long end = (get_file_size(data) / record_size) - 1;
    long pos;
    Record record;
    while(ini < end){
        long mid = (ini + end) / 2;
        pos = mid * record_size;
        data.seekg(pos);
        readRecord(record, data);
        if(record.get_codigo() < key){
            ini = mid + 1;
        }
        else if(record.get_codigo() > key){
            end = mid - 1;
        }
        else{
            location loc;
            loc.position = pos;
            loc.file = file_type::data;
            return loc;
        }
    }
    bool existe_reg = true;
    while(existe_reg and record.get_codigo() < key){
        pos += record_size;
        data.seekg(pos);
        existe_reg = readRecord(record,data);
    }
    existe_reg = true;
    while(existe_reg and record.get_codigo() > key){
        pos -= record_size;
        data.seekg(pos);
        existe_reg = readRecord(record, data);
    }
    if(record.next_file == file_type::aux){
        bool existe_reg = true;
        while(record.get_codigo() < key and record.next_file == file_type::aux and existe_reg){
            pos = record.next;
            aux.seekg(pos);
            existe_reg = readRecord(record,aux);
        }
        location loc;
        loc.position = pos;
        loc.file = file_type::aux;
        return loc;
    }
    location loc;
    loc.position = pos;
    loc.file = file_type::data;
    return loc;
}

void Sequential_File::load_data(const string& file_csv){
    Record record;
    fstream data(data_file, ios::out | ios::binary | ios::app);
    fstream aux(aux_file, ios::out | ios::binary | ios::app);
    if(!data or !aux){
        return;
    }
    rapidcsv::Document document(file_csv);
    auto len = document.GetRowCount();
    long mov = 0;
    for(int i = 0; i < len; i++){
        vector<string> row = document.GetRow<string>(i);
        record.load_data(row);
        //print_record(record);
        mov += sizeof(record);
        if(i == len - 1){
            record.next_file = file_type::none;
        }
        else{
            record.next_file = file_type::data;
        }
        record.next = mov;
        writeRecord(record,data);
    }
    data.close();
    aux.close();
}

Record Sequential_File::search(int key){
    fstream data(data_file, ios::in | ios::binary);
    fstream aux(aux_file, ios::in | ios::binary);
    if(!data || !aux){
        return Record();
    }
    Record record;
    location ubi = find(key, data, aux);
    if(ubi.file == file_type::data){
        data.seekg(ubi.position);
        readRecord(record, data);
    }
    else{
        aux.seekg(ubi.position);
        readRecord(record, aux);
    }

    data.close();
    aux.close();
    if(record.get_codigo() == key){
        return record;
    }
    return Record();
}

// Mezcla los datos del file de datos con el auxiliar, se encargara de hacer las inserciones en los vacios
void Sequential_File::merge(){
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    Record record;
    readRecord(record, data);
    while(record.next_file == file_type::data){
        data.seekg(record.next);
        readRecord(record, data);
    }
    auto inicio_merge = data.tellg();
    auto size_record = sizeof(Record);
    inicio_merge =- size_record;
    vector<Record> records;
    while(record.next_file != file_type::none){
        records.push_back(record);
        (record.next_file == file_type::data ? data : aux).seekg(record.next);
        readRecord(record, record.next_file == file_type::data ? data : aux);
    }
    data.seekg(inicio_merge);
    for(auto i:records){
        i.next_file = file_type::data;
        size_record = sizeof(i);
        inicio_merge =+ size_record;
        i.next = inicio_merge;
        writeRecord(i,data);
    }
    data.close();
    aux.close();
    aux.open(aux_file, fstream::out | fstream::trunc);
    aux.close();
}

//El problema esta aca
bool Sequential_File::insert(Record record){
    if(record.codigo <= 0){
        return false;
    }
    fstream data(data_file, ios::out | ios::in | ios::binary | ios::app);
    fstream aux(aux_file, ios::out | ios::in | ios::binary | ios::app);
    if(!data or !aux){
        cout<<"No se abrio el archivo"<<endl;
        return false;
    }
    location loc = find(record.get_codigo(), data, aux); //Buscamos un registro antes del que vamos a ingresar
    (loc.file == file_type::data ? data : aux).seekg(loc.position); 
    Record temp;
    bool band = readRecord(temp, (loc.file == file_type::data ? data : aux)); //Lo recuperamos
    if(!band){
        return false;
    }
    if(record.get_codigo() == temp.get_codigo()){
        return false;
    }
    //Guardamos sus punteros
    auto next_file = temp.next_file;
    auto next = temp.next;
    cout<<"next file del temp: "<<next_file<<endl;
    //Actualizamos los punteros
    temp.next_file = file_type::aux; //Se coloca en el archivo aux
    temp.next = aux.tellg(); //No importa donde lo escribamos, lo que importa es la conexion
    cout<<"pos de aux: "<<temp.next<<endl;
    cout<<"---------------------------------------------------------------"<<endl;
    print_record(temp);
    cout<<"Tamano file aux: "<<(get_file_size(aux) / sizeof(record))<<endl;
    cout<<"---------------------------------------------------------------"<<endl;
    cout<<temp.next_file<<endl;
    (loc.file == file_type::data ? data : aux).seekg(loc.position);
    band = writeRecord(temp, (loc.file == file_type::data ? data : aux));
    if(!band){
        return false;
    }
    //Escribimos el registro
    record.next_file = next_file;
    record.next = next;
    band = writeRecord(record,aux);
    if(!band){
        return false;
    }
    if((get_file_size(aux) / sizeof(Record)) >= aux_size * sizeof(Record)){
        data.close();
        aux.close();
        merge();
    }
    return true;
}

void Sequential_File::print_records(){
    fstream file_data(data_file, ios::in | ios::binary | ios::app);
    fstream file_aux(aux_file, ios::in | ios::binary | ios::app);
    if(!file_data or !file_aux){
        return;
    }

    Record record;
    readRecord(record,file_data);
    cout<<record.next_file<<endl;
    //print_record(record);

    if(record.next_file == file_type::data){
        file_data.seekg(record.next);
    }
    else{
        file_aux.seekg(record.next);
    }

    while(readRecord(record, record.next_file == file_type::data ? file_data : file_aux)){
        cout<<record.next_file<<endl;
        //print_record(record);
        (record.next_file == file_type::data ? file_data : file_aux).seekg(record.next);
        if(record.next_file == file_type::none){
            break;
        }
    }

    file_data.close();
    file_aux.close();
}

vector<Record> Sequential_File::range_search(int beg, int end){}

bool Sequential_File::remove(int key){}
*/
