#include <iostream>
#include "Sequential_file.h"
using namespace std;

void print_vec(vector<Estudiante> vec){
    for(auto i : vec){
        i.print();
    }
}

#define TEST_TIMECHRONO // Print time taken
/*
EX:
1,51720,Virginia,Norton,2020,0,4,36
2,51185,Virginia,Tazewell,2020,0,9,37
4,53019,Washington,Ferry,2020,0,1,48
*/

int main(int argc, char **argv){
 
	int i;

    Sequential_File seq("DB/db_sequential");


    if(argc>=2){
        
        if(string(argv[1])=="-s"){
            if(argc==3){
                #ifdef TEST_TIMECHRONO
                auto start = chrono::steady_clock::now();
                #endif

                Estudiante es = seq.search(stoi(string(argv[2])));

                #ifdef TEST_TIMECHRONO
                auto end = chrono::steady_clock::now();
                cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
                #else
                seq.write_csv(es);
                #endif
            }
            else if(argc==4){
                #ifdef TEST_TIMECHRONO
                auto start = chrono::steady_clock::now();
                #endif

                vector<Estudiante> vec = seq.range_search(stoi(string(argv[2])),stoi(string(argv[3])));

                #ifdef TEST_TIMECHRONO
                auto end = chrono::steady_clock::now();
                cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
                #else
                seq.write_csv(vec);
                #endif
            }
        }
        else if(string(argv[1])=="-p"){
            vector<Estudiante> vec3 = seq.load();
            seq.write_csv(vec3);
        }
        else if(string(argv[1])=="-d"){
            if(argc==3){
                #ifdef TEST_TIMECHRONO
                auto start = chrono::steady_clock::now();
                #endif

                seq.delete_record(stoi(string(argv[2])));

                #ifdef TEST_TIMECHRONO
                auto end = chrono::steady_clock::now();
                cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
                #else
                vector<Estudiante> vec3 = seq.load();
                seq.write_csv(vec3);
                #endif                
            }
        }
        else if(string(argv[1])=="-i"){
            if(argc==3){
                #ifdef TEST_TIMECHRONO
                auto start = chrono::steady_clock::now();
                #endif
                
                seq.insert(Estudiante(string(argv[2])));
                
                #ifdef TEST_TIMECHRONO
                auto end = chrono::steady_clock::now();
                cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
                #else
                vector<Estudiante> vec3 = seq.load();
                seq.write_csv(vec3);
                #endif   
            }
        }
    }
}