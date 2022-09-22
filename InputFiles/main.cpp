#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <utility>
#include "Record.h"
using namespace std;

//PRIIMERO EJECUTAR ESTOOOOOOOOOOOOOOOO

void convertir(){
    fstream file("dataset.csv", ios::in);
    string fila;

    vector<Estudiante> estudiantes;
    Estudiante estudiante;

    int i = 0;
    while(getline(file,fila)){
        vector<string> vec;
        stringstream line(fila); //Recupero la fila
        while(line.good()){
            string cadena;
            getline(line, cadena, ','); //Recupero cada elemento de la cadena con el delimitador
            vec.push_back(cadena); //Pusheo la cadena a mi vector
        }
        estudiante.codigo = stoi(vec[0]);
        estudiante.FIPS_code = stoi(vec[1]);
        strcpy(estudiante.estado, vec.at(2).c_str()); 
        strcpy(estudiante.date, vec.at(3).c_str()); 
        estudiante.totaldeath = stoi(vec[4]);
        estudiante.confirmedCase = stoi(vec[5]);
        estudiante.longitd = stol(vec[6]);
        estudiante.latitud = stol(vec[7]);
/*
        strcpy(estudiante.apellido, vec.at(3).c_str()); 
        estudiante.edad = stol(vec[4]);
        strcpy(estudiante.carrera, vec.at(5).c_str()); 
        estudiante.ciclo = stol(vec[6]);
*/
        estudiantes.push_back(estudiante);
    }
    file.close();
    fstream new_file("../Sequential/Estudiantes.bin", ios::out | ios::binary);

    if(!new_file){
        cout<<"ERROR"<<endl;
    }

    for(auto i:estudiantes){
        new_file.write((char*)& i, sizeof(Estudiante));
    }
    new_file.close();
}

int main(){
    convertir();
    return 0;
}