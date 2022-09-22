#include <iostream>
#include "Sequential/Sequential_file.h"
using namespace std;

void print_vec(vector<Estudiante> vec){
    for(auto i : vec){
        i.print();
    }
}

//LOS REGISTROS DEL DATA SET DEBEN DE ESTAR ORDENADOS

int main(int argc, char **argv){
	int i;

    Sequential_File seq("Sequential/Estudiantes.bin", "Sequential/SequentialEstudiante.bin");


    if(argc>=2){
        
        if(string(argv[1])=="-s"){
            if(argc==3){
                //cout<<stoi(string(argv[2]))<<endl;
                Estudiante es = seq.search(stoi(string(argv[2])));
                seq.write_csv(es);
            }
            else if(argc==4){
                vector<Estudiante> vec = seq.range_search(stoi(string(argv[2])),stoi(string(argv[3])));
                seq.write_csv(vec);
                //cout<<stoi(string(argv[2]))<<endl;
                //cout<<stoi(string(argv[3]))<<endl;
            }
        }
        else if(string(argv[1])=="-p"){
            vector<Estudiante> vec3 = seq.load();
            seq.write_csv(vec3);
        }
        else if(string(argv[1])=="-d"){
            if(argc==3){
                //cout<<stoi(string(argv[2]))<<endl;
                seq.Delete(stoi(string(argv[2])));
            }
        }
        else if(string(argv[1])=="-i"){
            if(argc==3){
                seq.insert(Estudiante(string(argv[2])));
            }
        }
    }


        //seq.insert(Estudiante(16,"Femenino","Alondra","Palacios",32,"Energia",10));

    /*       seq.insert(Estudiante(18,"Femenino","Valeria","Gonzales",56,"Comunicaciones",8));

 
    seq.insert(Estudiante(11,"Masculino","Juan","Sara",19,"Industrial",3));
    seq.insert(Estudiante(12,"Masculino","Alonso","Junco",29,"CS",6));
    seq.insert(Estudiante(13,"Femenino","Karla","Tarazona",7,"DS",7));
    seq.insert(Estudiante(14,"Femenino","Marcia","Valverde",23,"Mecanica",4));
    seq.insert(Estudiante(15,"Masculino","Diego","Landauri",54,"Minas",2));
    seq.insert(Estudiante(17,"Masculino","Heider","Sanchez",4,"Electronica",7));
    seq.insert(Estudiante(18,"Femenino","Valeria","Gonzales",56,"Comunicaciones",8));
    vector<Estudiante> estudiantes = seq.load();
    print_vec(estudiantes);
    */
        //seq.insert(Estudiante(34,"Femenino","Valeria","Gonzales",56,"Comunicaciones",8));
    //cout<<"-------------------EMPIEZA EL SEARCH---------------"<<endl;
    //Estudiante es = seq.search(98);
    //es.print();
    //cout<<"-------------------TERMINO EL SEARCH---------------"<<endl;
    //cout<<"-------------------EMPIEZA EL RANGE SEARCH---------------"<<endl;
    //vector<Estudiante> vec = seq.range_search(14,21);
    //print_vec(vec);
    //cout<<"-------------------TERMINO EL RANGE SEARCH----------------"<<endl;
    //seq.Delete(34);
    //seq.Delete(12);

    //vector<Estudiante> vec2 = seq.load();
    //cout<<"-------------------TERMINO EL DELETE-------------------"<<endl;
    //seq.insert(Estudiante(19,"Femenino","Valeria","Pruebas",33,"Matematica",8));

    //vector<Estudiante> vec3 = seq.load();
    //print_vec(vec3);


    //seq.select_all(seq.load());
}