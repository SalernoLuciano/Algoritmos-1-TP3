#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "biblioteca.h"


int main(int argc, char* argv[]){
	if(argc == 1){
		return 0;
	}else{
		if(strcmp(argv[1], "convertir") == COINCIDEN){
			convertir_a_binario();
		}
		if(argc > 1){
			if(strcmp(argv[1],"promover_enanos") == COINCIDEN){
				promover_enanos();
			}
			if(strcmp(argv[1],"listar_enanos") == COINCIDEN){
				int listar_enano = atoi(argv[2]);
				listar_enanos(listar_enano);
			}
			if(strcmp(argv[1], "integrantes_mision") == COINCIDEN){
				int guerreros = atoi(argv[2]), lideres = atoi(argv[3]), generales = atoi(argv[4]);
				if(argc == 6){	
					integrantes_mision(guerreros, lideres, generales, argv[5]);
				}else{
					integrantes_mision(guerreros, lideres, generales, MISION_BASE);
				}
			}
			if(strcmp(argv[1], "actualizar_censo") == COINCIDEN){
				actualizar_censo(argv[2]);
			}
		}
		return 0;
	}
}