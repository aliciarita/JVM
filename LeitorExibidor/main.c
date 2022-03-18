/*Leitor e Exibidor de bytecodes

Alícia Rita Oliveira dos Reis – 17/0129306  

Cintia Leal Rodrigues – 17/0125696 

Edilton Costa Alves – 17/0002365 

Eduardo Vaz Fagundes Rech – 18/0075161 

Mariana Borges de Sampaio – 18/0046926 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "classFile.h"
#include "estruturas.h"

int main(int argc, char const *argv[]){
    ClassFile *classfile = malloc(sizeof(ClassFile));
    int entrada=0;
	int saida=0;

	//Armazenar a string para conter o nome do arquivo .class que será lido pelo leitor
	char *filename = malloc(100*sizeof(char));
	char *outputName = malloc(100*sizeof(char));
	
    //Condicionais pra formar o menu
    if(argc > 1){
		if(strlen(argv[1]) > 6 && !strcmp(argv[1] + strlen(argv[1]) - 6, ".class")){
			strcpy(filename, argv[1]);
			entrada = 1;
		} else if(strlen(argv[1]) > 4 && !strcmp(argv[1] + strlen(argv[1]) - 4, ".txt")){
			strcpy(outputName, argv[1]);
			saida = 1;
		}
		if(argc > 2){
			if(strlen(argv[2]) > 4 && !strcmp(argv[2] + strlen(argv[2]) - 4, ".txt")){
				strcpy(outputName, argv[2]);
				saida = 1;
			} else if(strlen(argv[2]) > 6 && !strcmp(argv[2] + strlen(argv[2]) - 6, ".class")){
				strcpy(filename, argv[2]);
				entrada = 1;
			}	
		}
	}
	if(!entrada){
		scanf("%s",filename);
	}
	if(!saida){
		printf("Dê um nome ao arquivo\n");
		scanf("%s",outputName);
	}	

	classfile = readFile(filename);

	FILE *outputFile = fopen(outputName, "w"); /*w text writing*/
	if (classfile != NULL && outputFile != NULL) {
        imprimirClassFile(classfile, stdout);
		imprimirClassFile(classfile, outputFile);
		fclose(outputFile);
	}else{
		printf("Erro ao criar arquivo. Tente novamente.\n");
	}

	free(classfile);
	free(filename);
	free(outputName);

	return (0);
}

