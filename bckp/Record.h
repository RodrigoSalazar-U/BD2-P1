/*
struct Estudiante{
    int codigo;
    char sexo[15];
    char nombre[20];
    char apellido[20];
    int edad;
    char carrera[35];
    int ciclo;
};
*/

struct Estudiante{
 
    int codigo{};
    int FIPS_code{};
    char estado[30]{};
    char date[11]{};
    int totaldeath{};
    int confirmedCase{};
    long longitd{};
    long latitud{};
};