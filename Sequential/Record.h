#include <iostream>
#include <cstring>
using namespace std;

struct Estudiante{
 
    int codigo{};
    int FIPS_code{};
    char estado[30]{};
    char date[11]{};
    int totaldeath{};
    int confirmedCase{};
    long longitd{};
    long latitud{};
    long next;
    long prev;
/*
    int codigo{};
    char sexo[15]{};
    char nombre[20]{};
    char apellido[20]{};
    int edad{};
    char carrera[35]{};
    int ciclo{};
    long next;
    long prev;
*/
    Estudiante() {};
/*
    Estudiante(int codigo, const string& sexo, const string& nombre, const string& apellido, int edad, const string& carrera, int ciclo){
        this->codigo = codigo;
        strcpy(this->sexo, sexo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->apellido, apellido.c_str());
        this->edad = edad;
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
    }
*/
   Estudiante(int codigo,int FIPS_code,const string& estado,  const string& date, int totaldeath, int confimedCase, long logitud, long latitud  ){
        this->codigo = codigo;
        this->FIPS_code=FIPS_code;
        strcpy(this->estado, estado.c_str());
        strcpy(this->date, date.c_str());
        this->totaldeath=totaldeath;
        this->confirmedCase=confimedCase;
        this->longitd=longitd;
        this->latitud=latitud;
    }

    void print(){
        cout<<this->codigo<<","<<this->FIPS_code<<","<<this->estado<<","<<this->date<<","<<this->totaldeath<<","<<this->confirmedCase<<","<<this->longitd<<","<<this->latitud;
        cout<<endl;
    }

    string print_csv(){
        return to_string(this->codigo)+","+to_string(this->FIPS_code)+","+this->estado+","+this->date+","+to_string(this->totaldeath)+","+to_string(this->confirmedCase)+","+to_string(this->longitd)+","+to_string(this->latitud)+"\n";
    }
/*
    void print(){
        cout<<this->codigo<<","<<this->sexo<<","<<this->nombre<<","<<this->apellido<<","<<this->edad<<","<<this->carrera<<","<<this->ciclo<<","<<this->prev<<","<<this->next;
        cout<<endl;
    }

    string print_csv(){
        return to_string(this->codigo)+","+this->sexo+","+this->nombre+","+this->apellido+","+to_string(this->edad)+","+this->carrera+","+to_string(this->ciclo)+","+to_string(this->prev)+","+to_string(this->next)+"\n";
    }
*/
};