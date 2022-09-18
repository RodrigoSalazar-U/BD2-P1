### BD2 - Grupo 4
**Integrantes:**
- Salazar Alva, Rodrigo Gabriel
- Sara Junco, Juan Sebastian
- Ponce Contreras, Luis Eduardo
- Lapa Carhuamaca, Arleth Ivhy
---

# Proyecto 1 | Organización de Archivos

## Introducción
La organización de archivos facilita la búsqueda y localización de los datos, para ello se emplea los distintos medios de almacenamiento secundario y manejo de los fundamentos de archivos. El objetivo del proyecto es la implementación y análisis de los algoritmos de almacenamiento _Seguential_ _File_ y _Extendible_ _Hashing_, los cuales son técnicas de organización de archivos en memoria secundaria y acceso concurrente para lograr una buena gestión y eficacia, asimismo mostrar los resultados en una interfaz grafica de usuario (GUI).

En primer lugar, se brinda una breve descripción de las dos técnicas de indexación de archivos en memoria secundaria, las cuales son: _Seguential_ _File_ y _Extendible_ _Hashing_, y de cada proceso de sus respectivas operaciones principales, tales como, inserción, eliminación y búsqueda. En segundo lugar, el dominio de datos a usar es de un dataset público llamado _Kaggle_, contiene un archivo plano con datos reales de una lista de //escoger dataset//. En tercer lugar, se realizó pruebas con la implementacion de estas dos técnicas y una comparación, discusión y análisis de estos resultados experimentales obtenidos. En último lugar, se muestra las funcionalidades de este programa en un video.

## Técnicas Utilizadas

### Sequential File
Es una técnica simple de organización de datos,en esta los archivos se almacenan secuencialmente según una secuencia lógica en la memoria de la computadora, tiene dos formas de implementación: Pile File y Sorted File. Asimismo, admite operaciones de datos, tales como: insertar, actualizar, eliminar y recuperar datos.

#### Búsqueda específica:
Se implementó una búsqueda binaria, ya que los archivos están ordenados, con esta función se retorna la posición donde se encuentra el registro que se busca.
#### Búsqueda por rango:
Se implementó una búsqueda binaria, por la razón antes mencionada, solo que ahora se retorna todo lo que se encuentre entre los dos parámetros de la función que son keys y se leen secuencialmente.
#### Inserción:
Se recibe a un archivo como input, los archivos se encuentran ordenados físicamente, por ello se realiza un auxiliar para insertar el nuevo registro y luego se actualiza, con ello se asegura que nuestro archivo se encuentre ordenado.
#### Eliminación:
Primero se busca y encuentra el registro a eliminar, con la función seekg(pos) obtenemos la posición, con esta posición aplicamos la técnica LIFO, donde en el header escribimos el nuevo next delete y modificamos el next delete a eliminar. 


### Extendible Hashing
Es una técnica de hash dinámico, por lo cual es muy flexible para localizar los datos que estan asociados con un identificador único o clave determinada. Esta técnica proporciona una estructura dinámica que crece y se reduce a medida que la base de datos crece y se reduce.  
#### Búsqueda específica:
Se aplica la función Hash sobre la key que se quiere encontrar y esta devuelve una secuencia de índices, luego la entrada al directorio que coincide con dicho índice es la que dirige a un bucket, el cual contiene el registro que buscamos.
#### Búsqueda por rango:
Utilizando la lógica anterior, se utilizan los dos parámetros para buscar en cada bucket y verificar si estos se encuentran en el rango buscado.
#### Inserción:
Primero se localiza el bucket respectivo, si no encontró el bucket, procede a buscar uno con la profundidad local mínima y a crearlo. Si encuentra un bucket y no está lleno, procede a instalarlo, y si el bucket está lleno, este se divide y reinsertar todos los registros.
#### Eliminación:
Se localiza el buffer respectivo y se elimina dicho registro, cuando éste quede vacío, este puede ser liberado.

### Cantidad de accesos a memoria secundaria:

|                |SEQUENTIAL FILE                |EXTENDIBLE HASHING                         |
|----------------|-------------------------------|-----------------------------|
|INSERCION       |//On                           |//On              |
|BUSQUEDA        |//On                           |//On              |
|ELIMINACION     |//On                           |//On              |

## Resultados Experimentales

#### Gráfico Comparativo Inserción 
<img src="interfaz.jpeg" alt="interfaz"/>

#### Gráfico Comparativo Busqueda
<img src="interfaz.jpeg" alt="interfaz"/>

## Pruebas de uso y Presentación

#### Interfaz Grafica 
<img src="interfaz.jpeg" alt="interfaz"/>

#### Funcionalidad del Aplicativo 
[Video Presentación](url)

StackEdit stores your files in your browser, which means all your files are automatically saved locally and are accessible **offline!**
