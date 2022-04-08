#ifndef JVM_H
#define JVM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "listaClasses.h"
#include "pilhaFrames.h"
#include "instrucoes.h"

typedef struct lista_objetos {
	ClassFile * obj;
	u1 sizeData;
	u4 * data;
	struct lista_objetos * prox;
	struct lista_objetos * ant;
}Lista_Objetos;

typedef struct jvm{
	classesCarregadas * classes; 
	pilha_frames *frames; 
	u4 pc; 
	u2 excecao; 
	Lista_Objetos * objetos; 
	char *excecao_nome; 
}JVM;

extern JVM *jvm;
extern instrucao* instrucoes;

JVM* CriarJVM();

int menu();

JVM* InicializarJVM();

frame* criarFrame(char *classeCorrente, u2 max_locals);

void executarJVM();

u1 * atualizarPCMetodoAtual (u1 * code, u4 length);

void ImprimeCode (u1 * code, u4 length);

bool instrucaoBranch (char * nomeInstrucao);

void executarMetodo(method_info *m, char *classeCorrente, int chamador);

void interpretaCode(u1 *code, u4 length,method_info *m);

void freeVetorLocais(vetor_locais *v, u2 vetor_length);

u2 verificaHandlerMetodo(method_info *m);
/*
#define MAIN_NOME
#define DESCRIPTOR_MAIN 
#define PUBLIC_STATIC 0x0009
#define CLINIT_NOME "<clinit>"
#define DESCRIPTOR_CLINIT 
#define STATICCLINIT 
#define INIT_NOME "<init>"
#define DESCRIPTOR_INIT "()V"
*/
#endif
