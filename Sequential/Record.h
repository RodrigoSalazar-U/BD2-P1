#include <iostream>
#include <cstring>
using namespace std;

struct Estudiante{
    int codigo{};
    char sexo[15]{};
    char nombre[20]{};
    char apellido[20]{};
    int edad{};
    char carrera[35]{};
    int ciclo{};
    long next;
    long prev;

    Estudiante() {};

    Estudiante(int codigo, const string& sexo, const string& nombre, const string& apellido, int edad, const string& carrera, int ciclo){
        this->codigo = codigo;
        strcpy(this->sexo, sexo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->apellido, apellido.c_str());
        this->edad = edad;
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
    }

    void print(){
        cout<<this->codigo<<","<<this->sexo<<","<<this->nombre<<","<<this->apellido<<","<<this->edad<<","<<this->carrera<<","<<this->ciclo<<","<<this->prev<<","<<this->next;
        cout<<endl;
    }

    string print_csv(){
        return to_string(this->codigo)+","+this->sexo+","+this->nombre+","+this->apellido+","+to_string(this->edad)+","+this->carrera+","+to_string(this->ciclo)+","+to_string(this->prev)+","+to_string(this->next)+"\n";
    }
};