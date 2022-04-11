/* 
lista_classes.h
Arquivo que contém uma estrutura para uma lista de aulas carregadas
 */

#ifndef LISTA_CLASSES_H
#define LISTA_CLASSES_H

#include <stdio.h>
#include "classFile.h"

struct classesCarregadas {
	ClassFile * arquivoClass; 			
	struct classesCarregadas * prox;	
	struct classesCarregadas * ant;		
};
typedef struct classesCarregadas classesCarregadas;

classesCarregadas* CriarLista_classes();

classesCarregadas* InserirFim_classes(classesCarregadas *lista, ClassFile *arquivoClass);

classesCarregadas* RemoverFim_classes(classesCarregadas *lista);

classesCarregadas* RemoverElemento_classes(classesCarregadas *lista, char *string);

classesCarregadas* BuscarElemento_classes(classesCarregadas *lista, char *string);

cp_info* BuscarCPClasseCorrente_classes(classesCarregadas *lista, char *string);

method_info * BuscarMethodClasseCorrente_classes(classesCarregadas *lista, char *nomeClasse, char*nomeMetodo);

void ImprimirLista_classes(classesCarregadas *lista);

void LiberarLista_classes(classesCarregadas *lista);

field_info * BuscarFieldClasseCorrente_classes(classesCarregadas *lista, char * nomeClasse, char * nomeField);

#endif
