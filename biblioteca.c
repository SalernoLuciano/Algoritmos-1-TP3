#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "biblioteca.h"

void convertir_a_binario(){
	FILE* rangos = fopen(RUTA_RANGO_MAESTRO,"r");
	if(rangos == NULL){
		//printf("El archivo no se pudo abrir correctamente.\n");
		return; 
	}
	FILE* binario = fopen(RUTA_RANGO_BINARIO, "w");
	if(binario == NULL){
		//printf("Ocurrio un error al abrir el archivo, se creara uno nuevo.\n");
		return; 
	}
	rango_t aux_rango;
	int leidos = fscanf(rangos, "%i;%[^;];%[^\n]\n", &aux_rango.id, aux_rango.rango, aux_rango.descripcion);
	while(leidos!=EOF){
		fwrite(&aux_rango, sizeof(rango_t), 1, binario);
		leidos = fscanf(rangos, "%i;%[^;];%[^\n]\n", &aux_rango.id, aux_rango.rango, aux_rango.descripcion);
	}
	fclose(rangos);
	fclose(binario);
}

void listar_enanos(int listar_enano){
	FILE* censo = fopen(RUTA_CENSO,"r");
	if(censo == NULL){
		//printf("El archivo 'censo.csv' no existe.\n");
		return;
	}
	FILE* rango_bin = fopen(RUTA_RANGO_BINARIO, "r");
	if(rango_bin == NULL){
		//printf("El archivo 'rango.dat' no existe.\n");
		fclose(censo);
		return;
	}
	rango_t aux_rango;
	enano_t aux_enano;
	fseek(rango_bin, (listar_enano-1)*(long)sizeof(rango_t), SEEK_SET);
	fread(&aux_rango, sizeof(rango_t), 1, rango_bin);
	printf("______________________________________________________\nRango: %s.\tDescripcion: %s.\n______________________________________________________\n",aux_rango.rango, aux_rango.descripcion);
	int leidos = fscanf(censo, "%[^;];%i;%i;%i\n", aux_enano.nombre, &aux_enano.edad, &aux_enano.cantidad_misiones, &aux_enano.id_rango);
	while(leidos!=EOF){
		if(aux_enano.id_rango == listar_enano){
			printf("\tNombre de enano: %s.\n\tEdad: %i.\n\tCantidad Misiones: %i.\n\tId del Rango: %i.\n\t___________________________\n", aux_enano.nombre, aux_enano.edad, aux_enano.cantidad_misiones, aux_enano.id_rango);
		}
		leidos = fscanf(censo, "%[^;];%i;%i;%i\n", aux_enano.nombre, &aux_enano.edad, &aux_enano.cantidad_misiones, &aux_enano.id_rango);	
	}	
	fclose(censo);
	fclose(rango_bin);
}

void ascender(enano_t* enano){
	if(enano->id_rango == LIDER && enano->cantidad_misiones > MISIONES_MIN_GENERAL && enano->edad >= MIN_EDAD_GENERAL && enano->edad < MAX_EDAD_GENERAL){
		enano->id_rango = GENERAL;
	}else if(enano->id_rango == GUERRERO && enano->cantidad_misiones > MISIONES_MIN_LIDER && enano->edad >= MIN_EDAD_LIDER && enano->edad < MIN_EDAD_GENERAL){
		enano->id_rango = LIDER;
	}else if(enano->id_rango == APRENDIZ && enano->cantidad_misiones > MISIONES_MIN_GUERRERO){
		enano->id_rango = GUERRERO;
	}
}

void promover_enanos(){
	FILE* censo = fopen(RUTA_CENSO, "r");
	if(!censo){
		return;
	}
	FILE* tmp = fopen(RUTA_TEMPORAL,"w");
	if(!tmp){
		return;
	}
	enano_t enano;
	int leidos = fscanf(censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
	while(leidos!=EOF || !feof(censo)){
		ascender(&enano);
		fprintf(tmp, "%s;%i;%i;%i\n", enano.nombre, enano.edad, enano.cantidad_misiones, enano.id_rango);
		leidos = fscanf(censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
	}
	fclose(censo);
	fclose(tmp);
	rename(RUTA_TEMPORAL, RUTA_CENSO);
}

void enlistar_enano(enano_t aux_enano, int rango_a_comparar, int* asignados, int requeridos, FILE* mision){
	if(aux_enano.id_rango == rango_a_comparar && (*asignados) < requeridos){
		(*asignados)++;
		fwrite(&aux_enano, sizeof(enano_t), 1, mision);
		//printf("Enano elegido:\n\t %s;%i\n-----------------------------\n",aux_enano.nombre, aux_enano.id_rango );
	}
}

void asignar_enanos(FILE* arch_censo, FILE*arch_mision, int guerreros, int lideres, int generales, int* guerreros_asignados, int* lideres_asignados, int* generales_asignados){
	enano_t aux_enano;
	int aprendices_asignados = NINGUN_ENANO;
	int leidos = fscanf(arch_censo, "%[^;];%i;%i;%i\n", aux_enano.nombre, &aux_enano.edad, &aux_enano.cantidad_misiones, &aux_enano.id_rango);
	enlistar_enano(aux_enano, APRENDIZ, &aprendices_asignados, CANT_APRENDICES, arch_mision);
	enlistar_enano(aux_enano, GUERRERO, guerreros_asignados, guerreros, arch_mision);
	enlistar_enano(aux_enano, LIDER, lideres_asignados, lideres, arch_mision);
	enlistar_enano(aux_enano, GENERAL, generales_asignados, generales, arch_mision);
	while((leidos!=EOF && !feof(arch_censo)) || ((*guerreros_asignados) < guerreros && (*lideres_asignados) < lideres && (*generales_asignados) < generales)){
		leidos = fscanf(arch_censo, "%[^;];%i;%i;%i\n", aux_enano.nombre, &aux_enano.edad, &aux_enano.cantidad_misiones, &aux_enano.id_rango);
		enlistar_enano(aux_enano, APRENDIZ, &aprendices_asignados, CANT_APRENDICES, arch_mision);
		enlistar_enano(aux_enano, GUERRERO, guerreros_asignados, guerreros, arch_mision);
		enlistar_enano(aux_enano, LIDER, lideres_asignados, lideres, arch_mision);
		enlistar_enano(aux_enano, GENERAL, generales_asignados, generales, arch_mision);
	}
}

bool chequear_mision(char mision[MAX_MISION]){
	FILE* arch = fopen(mision, "r");
	if(arch == NULL){
		return false;
	}else{
		fclose(arch);
		return true;
	}
}

void asignar_nombre_mision(char nombre_mision[MAX_MISION], char mision[MAX_MISION]){
	strcpy(mision, nombre_mision);
	strcat(mision, ".dat");
}

void integrantes_mision(int guerreros, int lideres, int generales, char nombre_mision[MAX_MISION]){
	char mision[MAX_MISION];
	asignar_nombre_mision(nombre_mision, mision);
	bool existe_mision = chequear_mision(mision);
	if(existe_mision){
		//printf("La mision ya se ha realizado anteriormente.\n");
		return;
	}else{
		FILE* arch_censo = fopen(RUTA_CENSO, "r");
		if(!arch_censo){
			return;
		}
		FILE* arch_mision = fopen(mision,"w");
		if(!arch_mision){
			return;
		}
		int guerreros_asignados = NINGUN_ENANO;
		int lideres_asignados = NINGUN_ENANO;
		int generales_asignados = NINGUN_ENANO;
		asignar_enanos(arch_censo, arch_mision, guerreros, lideres, generales, &guerreros_asignados, &lideres_asignados, &generales_asignados);
		if(guerreros_asignados < guerreros || lideres_asignados < lideres || generales_asignados < generales){
			fclose(arch_censo);
			fclose(arch_mision);
			//printf("No se pudo reunir a los enanos requeridos para esta mision. Se eliminara esta mision.\n");
			remove(mision);
		}else{
			fclose(arch_censo);
			fclose(arch_mision);
		}
	}
}

void union_de_archivos(FILE* censo, FILE* mision, FILE* nuevo_censo){
	enano_t enano_censo;
	enano_t enano_mision;
	int leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
	fread(&enano_mision, sizeof(enano_t), 1, mision);
	if(strcmp(enano_mision.nombre, enano_censo.nombre) == COINCIDEN){
		if(enano_mision.edad!=MUERTO){
				enano_mision.cantidad_misiones++;
				fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_mision.nombre, enano_mision.edad, enano_mision.cantidad_misiones, enano_mision.id_rango);
				leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
				fread(&enano_mision, sizeof(enano_t), 1, mision);	
			}else{
				leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
				fread(&enano_mision, sizeof(enano_t), 1, mision);
			}
	}else if(strcmp(enano_mision.nombre, enano_censo.nombre) > COINCIDEN){
		fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_censo.nombre, enano_censo.edad, enano_censo.cantidad_misiones, enano_censo.id_rango);
		leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
	}else if(strcmp(enano_mision.nombre, enano_censo.nombre) < COINCIDEN){
		if(enano_mision.edad!=MUERTO){
				enano_mision.cantidad_misiones++;
				fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_mision.nombre, enano_mision.edad, enano_mision.cantidad_misiones, enano_mision.id_rango);
				fread(&enano_mision, sizeof(enano_t), 1, mision);
			}else{
				fread(&enano_mision, sizeof(enano_t), 1, mision);
			}
	}
	while(!feof(mision) && leido_censo!=EOF){
		if(strcmp(enano_mision.nombre, enano_censo.nombre)== COINCIDEN){
			if(enano_mision.edad!=MUERTO){
				enano_mision.cantidad_misiones++;
				fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_mision.nombre, enano_mision.edad, enano_mision.cantidad_misiones, enano_mision.id_rango);
				leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
				fread(&enano_mision, sizeof(enano_t), 1, mision);	
			}else{
				leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
				fread(&enano_mision, sizeof(enano_t), 1, mision);
			}
		}else if(strcmp(enano_mision.nombre, enano_censo.nombre) > COINCIDEN){
			fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_censo.nombre, enano_censo.edad, enano_censo.cantidad_misiones, enano_censo.id_rango);
			leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
		}else if(strcmp(enano_mision.nombre, enano_censo.nombre) < COINCIDEN){
			if(enano_mision.edad!=MUERTO){
				enano_mision.cantidad_misiones++;
				fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_mision.nombre, enano_mision.edad, enano_mision.cantidad_misiones, enano_mision.id_rango);
				fread(&enano_mision, sizeof(enano_t), 1, mision);
			}else{
				fread(&enano_mision, sizeof(enano_t), 1, mision);
			}
		}
	}
	while(!feof(mision)){
		if(enano_mision.edad!=MUERTO){
			enano_mision.cantidad_misiones++;
			fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_mision.nombre, enano_mision.edad, enano_mision.cantidad_misiones, enano_mision.id_rango);
			fread(&enano_mision, sizeof(enano_t), 1, mision);
		}else{
			fread(&enano_mision, sizeof(enano_t), 1, mision);
		}
	}
	while(leido_censo!=EOF || !feof(censo)){
		fprintf(nuevo_censo, "%s;%i;%i;%i\n", enano_censo.nombre, enano_censo.edad, enano_censo.cantidad_misiones, enano_censo.id_rango);
		leido_censo = fscanf(censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
	}
}

void actualizar_censo(char nombre_mision[MAX_MISION]){
	char msn[MAX_MISION];
	asignar_nombre_mision(nombre_mision, msn);
	bool hay_mision = chequear_mision(msn);
	if(!hay_mision){
		//printf("No existe esa mision, por ende no hay nada para actualizar.\n");
		return;
	}else{
		FILE* censo = fopen(RUTA_CENSO, "r");
		if(censo == NULL){
			return;
		}
		FILE* mision = fopen(msn,"r");
		if(mision == NULL){
			fclose(censo);
			return;
		}
		FILE* nuevo_censo = fopen(RUTA_TEMPORAL, "w");
		if(nuevo_censo == NULL){
			fclose(censo);
			fclose(mision);
			return;
		}
		union_de_archivos(censo, mision, nuevo_censo);
		fclose(censo);
		fclose(mision);
		fclose(nuevo_censo);
		remove(RUTA_CENSO);
		rename(RUTA_TEMPORAL, RUTA_CENSO);
	}
}