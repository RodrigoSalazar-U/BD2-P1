 
#include <iostream>
#include <fstream>
 #include <chrono>
#include <sys/time.h>
#include <ctime>
using namespace std;
 
int main()
{
    ifstream file;
    file.open("datatest_kaggle_100.csv");
    if(!file.is_open())
    {
        cout<<"Unable to open the file."<<endl;
        return 0;
    }
    
    string line;
    auto start = chrono::steady_clock::now();

    while(getline(file, line))
    {
        system(&line[0]);
    }
    auto end = chrono::steady_clock::now();
            
    cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
    file.close();
    return 0;
}