/*
#include "Sequential_file.h"

//Funcion para leer en un archivo determinado
bool readRecord(Record* &record, fstream &file){
    file.read((char*)&record, sizeof(record));
    return file.good();
}

//Funcion para escribir en un archivo determinado
bool writeRecord(Record* &record, fstream& file){
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
void print_record(Record& record){
    cout<<record.codigo<<" "<<record.sexo<<" "<<record.nombre<<" "<<record.apellido<<" "<<record.edad<<" "<<record.carrera<<" "<<record.ciclo<<" ";
    cout<<endl;
}

//Buscara el key en los archivos correspondientes y devolvera informacion acerca de la posicion y en que archivo se encuentra
location find(int key, fstream& data, fstream& aux){
    long record_size = sizeof(Record);
    long ini = 0;
    long end = (get_file_size(data) / record_size) - 1;
    long pos;
    Record* record = new Record;
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
        existe_reg = readRecord(record,data)
    }
    existe_reg = true;
    while(existe_reg and record.get_codigo() > key){
        pos -= record.size;
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

Record* Sequential_File::search(int key){
    fstream data(data_file, ios::in | ios::binary);
    fstream aux(aux_file, ios::in | ios::binary);
    if(!data || !aux){
        return nullptr;
    }
    Record* record = new Record;
    location ubi = find(key, data, aux);
    if(ubi.file == file_type::data){
        data.seekg(ubi.position);
        readRecord(record, data);
    }
    else{
        aux.seekg(ubi.position);
        readRecord(record, aux);
    }

    data.close;
    aux.close;
    if(record.get_codigo() == key){
        return record;
    }
    return nullptr;
}

// Mezcla los datos del file de datos con el auxiliar, se encargara de hacer las inserciones en los vacios
void Sequential_File::merge(){
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    Record* record = new Record;
    readRecord(record, data);
    while(record.next_file == file_type::data){
        data.seekg(record.next);
        readRecord(record, data);
    }
    auto inicio_merge = data.tellg();
    inicio_merge = inicio_merge - sizeof(Record);
    vector<Record> records;
    while(record.next_file != file_type::none){
        records.push_back(record);
        (record.next_file == file_type::data ? data : aux).seekg(record.next);
        readRecord(record, record.next_file == file_type::data ? data : aux);
    }
    data.seekg(inicio_merge);
    for(auto i:records){
        i.next_file = file_type::data;
        inicio_merge = inicio_merge + sizeof(i);
        i.next = inicio_merge;
        writeRecord(i,data)
    }
    data.close();
    aux.close();
    aux.open(aux_file, fstream::out | fstream::trunc);
    aux.close();
}

bool Sequential_File::insert(Record* record){
    if(record.key <= 0){
        return false;
    }
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    if(!data or !aux){
        return false;
    }
    location loc = find(record.get_codigo(), data, aux); //Buscamos un registro antes del que vamos a ingresar
    (loc.file == file_type::data ? data : aux).seekg(loc.position); 
    Record temp;
    bool band = readRecord(temp, loc.file == file_type::data ? data : aux); //Lo recuperamos
    if(!band){
        return false;
    }
    if(record.get_codigo() == temp.get_codigo()){
        return false;
    }
    //Guardamos sus punteros
    auto next_file = temp.next_file;
    auto next = temp.next;
    //Actualizamos los punteros
    temp.next_file = file_type::aux; //Se coloca en el archivo aux
    temp.next = aux.tellg(); //No importa donde lo escribamos, lo que importa es la conexion
    (loc.file == file_type::data ? data : aux).seekg(loc.position);
    band = writeRecord(temp, (loc.file == file_type::data ? data : aux));
    if(!band){
        return false;
    }
    //Escribimos el registro
    record = new Record;
    record.next_file = next_file;
    record.next = next;
    band = writeRecord(record,aux);
    if(!band){
        return false;
    }
    if(aux.tellg() >= aux_size * sizeof(Record)){
        data.close();
        aux.close();
        merge();
    }
    return true;
}


vector<Record> Sequential_File::range_search(int beg, int end);
bool Sequential_File::remove(int key);

void Sequential_File::print_records(){
    fstream file_data(data_file, ios::in | ios::binary);
    fstream file_aux(aux_file, ios::in | ios::binary);
    if(!file_data or !file_aux){
        return;
    }

    Record record;
    readRecord(record,file_data);
    print_record(record);

    if(record.next_file == file_type::data){
        file_data.seekg(record.next);
    }
    else{
        file_aux.seekg(record.next);
    }

    while(readRecord(record, record.next_file == file_type::data ? file_data : file_aux)){
        print_record(record);
        (record.next_file == file_type::data ? file_data : file_aux).seekg(record.next);
    }

    file_data.close();
    file_aux.close();
}
*/