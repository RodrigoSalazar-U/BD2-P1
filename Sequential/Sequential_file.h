#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "Record.h"
#include <chrono>
#include <sys/time.h>
#include <ctime>
using namespace std;

class Sequential_File{
    private:
        string file_name;
        string sequential_file_name;
        string aux_file_name;
        string header_file_name;
        long registros_ordenados;
        long registros_desordenados;
        //int registros_anadidos = 0;

        unsigned long get_file_size(string file){
            ifstream file_name(file, ios::ate | ios::binary);
            if(!file_name){
                cout<<"ERROR EN LA FUNCION GET_FILE_SIZE"<<endl;
            }
            unsigned long size = file_name.tellg();
            file_name.close();
            return size;
        }

        void write_unordered_register_in_header(long unordered_register){
            fstream header(this->header_file_name, ios::in | ios::out | ios::binary);
            if(!header){
                cout<<"ERROR FUNCION WRITE_HEADER_ORDERED_REGISTER"<<endl;
            }
            header.seekp(2 * sizeof(unordered_register));
            header.write((char*)& unordered_register, sizeof(unordered_register));
            header.close();
        }

        long read_unordered_register_in_header(){
            fstream header(this->header_file_name, ios::in| ios::out | ios::binary);
            if(!header){
                cout<<"ERROR FUNCION READ_HEADER"<<endl;
            }
            header.seekg(2 * sizeof(long));
            long unordered_register;
            header.read((char*)& unordered_register, sizeof(unordered_register));
            header.close();
            return unordered_register;
        }


        void write_ordered_register_in_header(long ordered_register){
            fstream header(this->header_file_name, ios::in | ios::out | ios::binary);
            if(!header){
                cout<<"ERROR FUNCION WRITE_HEADER_ORDERED_REGISTER"<<endl;
            }
            header.seekp(1 * sizeof(ordered_register));
            header.write((char*)& ordered_register, sizeof(ordered_register));
            header.close();
        }

        long read_ordered_register_in_header(){
            fstream header(this->header_file_name, ios::in| ios::out | ios::binary);
            if(!header){
                cout<<"ERROR FUNCION READ_HEADER"<<endl;
            }
            header.seekg(1 * sizeof(long));
            long ordered_register;
            header.read((char*)& ordered_register, sizeof(ordered_register));
            header.close();
            return ordered_register;
        }

        void init_sequential(){
            ifstream input(this->file_name, ios::in | ios::binary);
            ofstream sequential(this->sequential_file_name, ios::out | ios::binary);

            if(!input or !sequential){
                cout<<"ERROR EN FUNCION INICIAR_SEQUENTIAL"<<endl;
                return;
            }

            Estudiante record;
            long actual_next = 1;
            long actual_prev = -1;

            unsigned long total_records = get_file_size(this->file_name) / (sizeof(Estudiante) - 2 * sizeof(long));
            while(input.read((char*)& record, sizeof(Estudiante) - 2 * sizeof(long))){
                record.next = total_records == actual_next ? -2 : actual_next++;
                record.prev = actual_prev++;
                sequential.write((char*)& record, sizeof(Estudiante));
            }
            registros_ordenados = total_records;
            registros_desordenados = 0;

        }

        void iniciar_sequential(){
            auto start = chrono::steady_clock::now();

            ifstream input(this->sequential_file_name, ios::in | ios::binary);
            if(input.is_open()){

                this->registros_ordenados = read_ordered_register_in_header();
                this->registros_desordenados = read_unordered_register_in_header();
  
            }
            else{
                input.close();
                init_sequential();
            }
            auto end = chrono::steady_clock::now();
            
            cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
            input.close();
        }

        void iniciar_free_list(){
            fstream header(this->header_file_name, ios::out);
            if(!header){
                cout<<"ERROR FUNCION INICIAR_FREE_LIST"<<endl;
            }
            long header_value = -1;
            header.seekp(0);
            header.write((char*)& header_value, sizeof(header_value));
            header.close();
        }

        Estudiante read(string file, long pos){
            fstream filexd(file, ios::in | ios::out | ios::binary);
            if(!filexd){
                cout<<"ERROR FUNCION READ"<<endl;
            }
            Estudiante record;
            filexd.seekg(pos * sizeof(Estudiante));
            filexd.read((char*)& record, sizeof(Estudiante));
            filexd.close();
            return record;
        }

        void write(Estudiante record, string file, long pos){
            fstream filexd(file, ios::in | ios::out | ios::binary);
            if(!filexd){
                cout<<"ERROR FUNCION WRITE"<<endl;
            }
            filexd.seekp(pos * sizeof(Estudiante));
            filexd.write((char*)& record, sizeof(Estudiante));
            filexd.close();
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
            fstream header(this->header_file_name, ios::in| ios::out | ios::binary);
            if(!header){
                cout<<"ERROR FUNCION READ_HEADER"<<endl;
            }
            header.seekg(0);
            long pos;
            header.read((char*)& pos, sizeof(pos));
            header.close();
            return pos;
        }

        void write_header(long pos){
            fstream header(this->header_file_name, ios::in | ios::out | ios::binary);
            if(!header){
                cout<<"ERROR FUNCION WRITE_HEADER"<<endl;
            }
            header.seekp(0);
            header.write((char*)& pos, sizeof(pos));
            header.close();
        }

        void rebuild(unsigned long registros_totales){
            fstream sequential(this->sequential_file_name, ios::in | ios::binary);
            fstream aux(this->aux_file_name, ios::out | ios::binary);

            if(!sequential or !aux){
                cout<<"ERROR FUNCION REBUILD PARTE 1"<<endl;
            }

            Estudiante record = this->read(this->sequential_file_name, this->pos_primer_registro());
            //record.print();
            while(record.next != -2){
                aux.write((char*)& record, sizeof(Estudiante));

                record = this->read(this->sequential_file_name, record.next);
                //record.print();
            }
            aux.write((char*)& record, sizeof(Estudiante));
            sequential.close();
            aux.close();

            sequential.open(this->sequential_file_name, ios::out | ios::binary);
            aux.open(this->aux_file_name, ios::in | ios::binary);

            if(!sequential or !aux){
                cout<<"ERROR FUNCION REBUILD PARTE 2"<<endl;
            }

            long current_next = 1;
            long current_prev = -1;

            while(aux.read((char*)& record, sizeof(Estudiante))){
                if (record.prev == -3){
                    continue;
                }
                record.next = current_next == registros_totales ? -2 : current_next++;
                record.prev = current_prev++;
                sequential.write((char*)& record, sizeof(Estudiante));
            }
            sequential.close();
            aux.close();
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
        long pos_insert(){
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
            this->write(siguiente_registro, this->sequential_file_name, primer_registro.next);
        }

        void insertar_final(Estudiante record){
            Estudiante ultimo_registro = this->read(this->sequential_file_name, registros_ordenados - 1);
            //Configuro mi nuevo registro con sus punteros
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
            this->write(record,this->sequential_file_name,location);
        }

        //Reconstrute el archivo de sequential despues de una insercion
        void rebuild_insertar(){
            unsigned long total_registros = get_file_size(this->sequential_file_name) / sizeof(Estudiante);
            this->rebuild(total_registros);
            registros_ordenados += registros_desordenados;
            registros_desordenados = 0;
        }

        void rebuild_delete(){
            unsigned long total_registros = get_file_size(this->sequential_file_name) / sizeof(Estudiante);
           
            this->rebuild(--total_registros);
            //cout<<"TOTAL DE REGISTROS "<< total_registros<<endl;

            registros_ordenados += registros_desordenados-1;
            registros_desordenados = 0;

        }

        

        //Recordemos que en el AUX FILE los registros estan desordenados, pero conectados de tal manera que logicamente estan ordenados
        void insertar_registros_desordenados(Estudiante base, Estudiante record){
            Estudiante current;

            long base_pos;
            Estudiante base_next = this->read(this->sequential_file_name, base.next); //Siguiente registro del base
            base_pos = base_next.prev; //La posicion previa del siguiente, sera nuestro registro actual
            current = this->read(this->sequential_file_name, base_pos);
            //record.print();
            //current.print();

            //Primera condicion busca su registro mas cercano para insertar el nuevo
            //Segunda condicion por si el registro actual apunta a un file con registros ordenados
            //Tercera condicion por si debe insertarse al final del archivo
            while(current.codigo < record.codigo and current.next > registros_ordenados - 1 and current.next != -2){ //Buscamos la posicion donde insertar
                if(current.codigo == record.codigo){
                    cout<<"Estas insertando un Codigo existente"<<endl;
                    return;
                }
                current = this->read(this->sequential_file_name, current.next);
            }
            if(current.codigo == record.codigo){ //Chequeamos si el ultimo registro adquirido ya existe con ese codigo
                cout<<"Estas insertando un Codigo existente"<<endl;
                return;
            }
            if(current.next < registros_ordenados){ //Si el nodo actual apunta a registros ordenados o al sequential
                if(current.codigo > record.codigo){
                    this->insert_punteros_actualizados(record, current);
                }
                else{
                    if(current.next == -2){
                        this->insert_despues_null(current, record);
                    }
                    else{
                        long current_next = current.next;
                        Estudiante registro_siguiente = this->read(this->sequential_file_name, current_next);
                        this->insert_punteros_actualizados(record, registro_siguiente); //Insertamos a la izquierda del (siguiente del actual) o en medio del actual y su siguiente lo quieren ver asi XD
                    }
                }
            }
            else{ // Si el nodo actual apunta a registros desordenados o al fiile auxiliar
                this->insert_punteros_actualizados(record, current); //Insertamos a la izquierda del current
            }
        }

        Estudiante search_file_ordenado(int codigo_busqueda){
            long ini = 0;
            long end = registros_ordenados - 1;
            long mid;
            Estudiante record;
            //cout<<"VALOR END: "<<end<<endl;

            while(end >= ini){
                mid = (ini + end) / 2;
                record = this->read(this->sequential_file_name, mid);
                //record.print();
                int codigo_record = record.codigo;
                //cout<<"VALOR MID: "<<mid<<endl;
                //cout<<"CODIGO ACTUAL: "<<codigo_record<<endl;

                if(codigo_record < codigo_busqueda){
                    //cout<<"AUMENTA EL INI"<<endl;
                    ini = mid + 1;
                    //cout<<"NUEVO VALOR DEL INI: "<<ini<<endl;
                }
                else if(codigo_record > codigo_busqueda){
                    //cout<<"DECREMENTA EL END"<<endl;
                    end = mid - 1;
                    //cout<<"NUEVO VALOR DEL END: "<<end<<endl;
                }
                else{
                    return record;
                }
            }
            return record;
        }
    public:
    Sequential_File() {};
    
    Sequential_File(string fileName, string sequentialFile){
        this->file_name = fileName;
        this->sequential_file_name = sequentialFile;
        this->aux_file_name = "Sequential/fileNameAux.bin";
        this->header_file_name = "Sequential/fileNameHeader.bin";

        this->iniciar_sequential();
        this->iniciar_free_list();
    }

    ~Sequential_File(){
        write_ordered_register_in_header(this->registros_ordenados);
        write_unordered_register_in_header(this->registros_desordenados);
    }

    void print_duda(){
            Estudiante record = this->read(this->sequential_file_name, 11);
            record.print();
            cout<<"PREV: "<<record.prev<<endl;
        }

    void select_all(vector<Estudiante> recibido){
        std::ofstream myfile;
        myfile.open ("example.csv");
        for (auto val : recibido){
            myfile<<val.print_csv();
        }
        myfile.close();

    }

    vector<Estudiante> load(){
        vector<Estudiante> records;
        fstream sequential(this->sequential_file_name, ios::in | ios::binary);

        if(!sequential){
                cout<<"ERROR FUNCION LOAD"<<endl;
            }

        Estudiante record;
        while(sequential.read((char*)& record, sizeof(record))){
            if(record.codigo != 0){
                records.push_back(record);
            }
        }
        return records;
    }

    void insert(Estudiante record){
        Estudiante base = this->search_file_ordenado(record.codigo);

        if(base.codigo == record.codigo){
            cout<<"Estas por insertar un codigo existente"<<endl;
            return;
        }
        if(base.prev == -1 and record.codigo < base.codigo){ //Insertar al inicio
            cout<<"Insertando al inicio"<<endl;
            this->insertar_inicio(record);
            //this->registros_anadidos+=2;
        }
        else if(base.next == -2){ //Insertar al final
            cout<<"Insertando al final"<<endl;
            this->insertar_final(record);
            //this->registros_anadidos+=2;
        }
        else{
            if(base.codigo > record.codigo){
                cout<<"Buscando base previo"<<endl;
                base = this->get_prev_estudiante(base);
                //this->registros_anadidos+=2;
            }
            if(base.next < registros_ordenados){ //El base apunta a un file ordenado
                cout<<"Insert Basico"<<endl;
                this->insert_basico(base,record);
                //this->registros_anadidos+=2;
            }
            else{ //Apunta a file desordenado
                cout<<"Insert en registros desordenados"<<endl;
                this->insertar_registros_desordenados(base,record);
                //this->registros_anadidos+=2;
            }
        }
        if(++registros_desordenados == 5){
            cout<<"Rebuild"<<endl;
            this->rebuild_insertar();
            //this->registros_anadidos = 0;
        }
    }

    void Delete(int codigo){
        Estudiante base = this->search_file_ordenado(codigo);
        //this->rebuild_insertar();
        if(base.codigo == codigo){
            if(base.prev == -1){
                Estudiante temp = this->read(this->sequential_file_name, base.next);
                base.prev = -3;
                this->write(base,this->sequential_file_name,temp.prev);
            }
            else{
                Estudiante temp = this->read(this->sequential_file_name, base.prev);
                base.prev = -3;
                this->write(base,this->sequential_file_name,temp.next);
            }
        this->rebuild_delete();

        } 
        else{
            cout<<"Estas queriendo borrar un codigo inexistente"<<endl;
            return;

        }
    }

    Estudiante search(int codigo){
        Estudiante base = this->search_file_ordenado(codigo);
        //cout<<base.codigo<<endl;

        if(base.codigo == codigo){
            return base;
        }
        if((base.prev == -1 and codigo < base.codigo) or (base.next == -2 and codigo > base.codigo)){
            cout<<"La busqueda no fue exitosa, estaba fuera de rango"<<endl;
            return Estudiante();
        }
        if(base.codigo > codigo){
            base = this->get_prev_estudiante(base);
        }

        Estudiante current = base;
        current = this->read(this->sequential_file_name, current.next);

        while(current.codigo <= codigo){
            if(current.codigo == codigo){
                return current;
            }
            else{
                current = this->read(this->sequential_file_name, current.next);
            }
        }

        cout<<"Busqueda fuera de rango de alcance"<<endl;
        return Estudiante();
    }

    void write_csv(vector<Estudiante> recibido){
        std::ofstream myfile;
        myfile.open ("InputFiles/dataset.csv");
        for (auto val : recibido){
            myfile<<val.print_csv();
        }
        myfile.close();

    }

    void write_csv(Estudiante recibido){
        std::ofstream myfile;
        myfile.open ("InputFiles/dataset.csv");
        myfile<<recibido.print_csv();
        myfile.close();

    }

    vector<Estudiante> range_search(int key1, int key2){
        vector<Estudiante> result;

        if(key1 > key2){
            cout<<"Por favor, introduce un intervalo valido"<<endl;
            return result;
        }

        Estudiante current = this->search_file_ordenado(key1); //Buscamos el key correspondiente o el mas cercano a este

        if(current.prev != -1){ //El registro encontrado no esta al inicio del file
            if(current.codigo > key1){ //Si el resultado de la busqueda mas cercana es mayor al key, entonces solo basta obtener el previo
                current = this->get_prev_estudiante(current);
            }
        }

        while(true){
            if(current.codigo >= key1 and current.codigo <= key2){ //Si esta entre el key1 y key2 lo metemos en el vector
                result.push_back(current);
            }
            if(current.codigo > key2 or current.next == -2){ //Si ya sobrepaso el ID del key2 o llego al final del archivo
                return result;
            }
            current = this->read(this->sequential_file_name, current.next);
        }
    }
};