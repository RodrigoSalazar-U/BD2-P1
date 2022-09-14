#include "Sequential_file.h"

//Funcion para leer en un archivo determinado
bool readRecord(Record& record, fstream &file){
    file.read((char*)&record, sizeof(record));
    return file.good();
}

//Funcion para escribir en un archivo determinado
bool writeRecord(){
    file.write((char*)&record, sizeof(record));
    return file.good();
}

//Funcion para tener el tamano del archivo
long get_file_size(fstream& file){
    auto pos = file.tellg();
    file.seekg(0, ios::end);
    auto size = file.tellg();
    file.seekg(pos);
    return size;
}

//Imprime el registro
void print_record(Record& record){
    cout<<record.codigo<<" "<<record.sexo<<" "<<record.nombre<<" "<<record.apellido<<" "<<record.edad<<" "<<record.carrera<<" "<<record.ciclo<<" ";
    cout<<endl;
}

//Buscara el key en los archivos correspondientes y devolvera informacion acerca de la posicion y en que archivo se encuentra
location find(int key, fstream& data, fstream& aux);

bool Sequential_File::insert(Record record);
bool Sequential_File::remove(int key);
Record Sequential_File::search(int key);
vector<Record> Sequential_File::range_search(int beg, int end);
void Sequential_File::merge();

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
