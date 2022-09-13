#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

enum file_type{
    data,
    aux,
    none
};

struct location{
    long position;
    file_type file;
}

struct Record{
    char codigo[4];
    char sexo[15];
    char nombre[20];
    char apellido[20];
    char edad[3];
    char carrera[35];
    char ciclo[2];
    long next; //puntero siguiente
    file_type next_file = file_type::data; //Archivo en donde se encuentra el registro

    int get_codigo(){
        return stoi(this->codigo); //lo retorna como entero
    }

    void load_data(vector<string> data){
        if(data.size() == 7){
        strcpy(this->codigo, data[0].c_str());
        strcpy(this->sexo, data[1].c_str());
        strcpy(this->nombre, data[2].c_str());
        strcpy(this->apellido, data[3].c_str());
        strcpy(this->edad, data[4].c_str());
        strcpy(this->carrera, data[5].c_str());
        strcpy(this->ciclo, data[6].c_str());
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
    bool remove(int); //Remueve registro
    Record search(int); //Busca el registro con una determinado codigo
    vector<Record> range_search(int, int); //Busqueda de registros por rango
    void merge(); //Mezcla el archivo aux con el data
    void print_records(); //Imprime los registros
};