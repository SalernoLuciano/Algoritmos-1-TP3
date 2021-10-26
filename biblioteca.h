#ifndef __BIBLIOTECA_H__
#define __BIBLIOTECA_H__
#include<stdbool.h>

#define COINCIDEN 0
#define MUERTO -1

#define MAX_NOMBRE 50
#define MAX_RANGO 50
#define MAX_DESCRIPCION 200
#define MAX_MISION 50

#define OBRERO 1
#define APRENDIZ 2
#define GUERRERO 3
#define LIDER 4
#define GENERAL 5
#define NINGUN_ENANO 0

#define CANT_APRENDICES 10

#define MISIONES_MIN_GUERRERO 10
#define MISIONES_MIN_LIDER 100
#define MISIONES_MIN_GENERAL 250

#define MIN_EDAD_LIDER 40
#define MIN_EDAD_GENERAL 50
#define MAX_EDAD_GENERAL 60

#define RUTA_CENSO "censo.csv"
#define RUTA_RANGO_MAESTRO "rango_MAESTRO.csv"
#define RUTA_RANGO_BINARIO "rango.dat"
#define RUTA_TEMPORAL "temporal_censo.csv"
#define MISION_BASE "mision"

typedef struct enano{
	char nombre[MAX_NOMBRE];
	int edad;
	int cantidad_misiones;
	int id_rango;
}enano_t;

typedef struct rango{
	int id;
	char rango[MAX_RANGO];
	char descripcion[MAX_DESCRIPCION];
}rango_t;

/*
 *Pre: Tiene que existir previamente un archivo .csv con los rangos de los enanos ordenados. El usuario debera pasar como argumento por linea de comando: 'convertir'
 *Post: Genera un archivo binario con los rangos de los enanos ordenados.
 */
void convertir_a_binario();

/*
 *Pre: Recibira un numero entre el 1 y 5 (que representa los distintos rangos de enanos) que fue enviado como argumento pasado por linea de comando y luego transformado a entero. Para invocar a esta funcion el usuario debera pasar como argumento por linea de comando: 'listar_enano 'rango_elegido'.'
 *Post:Imprimira por pantalla el listado de todos los enanos del archivo censo.csv que tengan el rango elegido por el usuario en linea de comando.
 */
void listar_enanos(int listar_enano);

/*
 *Pre:recibe un enano completo del archivo censo.csv, siempre y cuando el usuario haya elejido por comando la opcion 'promover_enanos'
 *Post:cambiara el rango del enano enviado cuando este cumpla con las condiciones para el cambio y solo se cambiara un rango por mas que este en condiciones de cambiar mas de 1 rango.
 */
void ascender(enano_t* enano);

/*
 *Pre:El usuario debera pasar como argumento por linea de comando: 'promover_enanos'
 *Post:dejara el archivo censo.csv con los rango de todos los enanos actualizados
 */
void promover_enanos();

/*
 *Pre:Se le pasa un enano del archivo censo.csv, rango del que queremos elegir enanos(del 2 al 5), cuantos enanos de ese rango ya estan asignados a la mision, cuantos enanos del rango elegido son neceraios (cantidad de enanos del rango elegido pasados por linea de comando) y el archivo de la mision abierto correctamente.
 *Post:Deja el enano pasado escrito en el archivo binario de la mision si es que cumple con las condiciones y aumenta la cantidad de enanos del rango elegido.
 */
void enlistar_enano(enano_t aux_enano, int rango_a_comparar, int* asignados, int requeridos, FILE* mision);

/*
 *Pre:Le paso el archivo censo.csv y el binario de la mision correspondiente abiertos y La cantidad requerida ( y los asignados, inicializados en cero) de: guerreros, lideres y generales.
 *Post:deja el archivo binario de la mision cargado con los enanos de cada rango pasado y ademas con 10 enanos de rango aprendiz o los que haya.
 */
void asignar_enanos(FILE* arch_censo, FILE*arch_mision, int guerreros, int lideres, int generales, int* guerreros_asignados, int* lideres_asignados, int* generales_asignados);

/*
 *Pre:se le pasa el nombre del archivo de la mision(nombre de mision+extension)
 *Post:Devuelve true si ya existe el archivo con ese nombre o false si no existe el archivo de la mision
 */
bool chequear_mision(char mision[MAX_MISION]);

/*
 *Pre: Se le pasa a la funcion el nombre de la mision y un auxiliar vacio del tipo string
 *Post:Deja al auxiliar cargado con el nombre que va a tener el archivo binario de la mision.(nombre+extension)
 */
void asignar_nombre_mision(char nombre_mision[MAX_MISION], char mision[MAX_MISION]);

/*
 *Pre:1° debe invocarse la funcion por linea de comando. Luego se le pasan las cantidades de guerreros, lideres y generales ( en ese orden y convertidos a enteros ) y el nombre de la mision que se quiere llevar a cabo( si no se le paso por comando ningun nombre de mision se le pasa el nombre 'mision')
 *Post: Creara un archivo binario con el nombre de la mision donde estaran los enanos elegidos desde censo.csv (si la cantidad de algun rango no se cumple la mision no se lleva a cabo y se elimina el archivo de la mision) 
 */
void integrantes_mision(int guerreros, int lideres, int generales, char nombre_mision[MAX_MISION]);

/*
 *Pre:una vez que se invoca ala funcion actualizar censo se envian los archivos censo.csv y el archivo binario de la mision elegida.
 *Post:dejara actualizado el archivo censo.csv con los enanos sin modificaciones que no hayan ido a la mision, le suma 1 a misiones del enano que fue a la mision y volvio con vida y se suma al censo los enanos que se creian muertos (NO estan en el censo pero si en la mision).
 */
void union_de_archivos(FILE* censo, FILE* mision, FILE* nuevo_censo);

/*
 *Pre:recibira por linea de comando 1°: invocar la funcion actualizar_censo y 2°: nombre de la mision con la cual vamos aactualizar el censo
 *Post:dejara el archivo censo actualizado, teniendo en cuenta si murio un enano en la mision, o volvio un enano que se creia muerto.
 */
void actualizar_censo(char nombre_mision[MAX_MISION]);

#endif