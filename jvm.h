/**
 * @file jvm.h
 * @brief Arquivo de cabecalho das funcoes utilizadas pela JVM implementada
 */
#ifndef JVM_H
#define JVM_H

// Bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "listaClasses.h"
#include "pilhaFrames.h"
#include "instrucoes.h"

/**
 * @struct Lista_Objetos
 * @brief Estrutura de dados para armazenar os Objetos instanciados
 */
typedef struct lista_objetos {
	ClassFile * obj;
	u1 sizeData;
	u4 * data;
	struct lista_objetos * prox;
	struct lista_objetos * ant;
}Lista_Objetos;

/**
 * @struct JVM
 * @brief Estrutura de controle da execucao do programa da JVM
 */
typedef struct jvm{
	classesCarregadas * classes; 
	pilha_frames *frames; 
	u4 pc; 
	u2 excecao; 
	Lista_Objetos * objetos; 
	char *excecao_nome; 
}JVM;

// Declara as variaveis globais que serao utilizadas por outros arquivos na execucao
extern JVM *jvm;
extern instrucao* instrucoes;

/**
 * @brief Cria uma estrutura JVM vazia
 * @return Retorna a estrutura JVM vazia
 */
JVM* CriarJVM();

/**
 * @brief Funcao para apresentar o menu do sistema para o usuario
 * @return Retorna a opcao escolhida pelo usuario
 */
int menu();

/**
 * @brief Funcao para inicializar a estrutura JVM, sentando seus valores iniciais
 * @return Retorna a estrutura JVM inicializada
 */
JVM* InicializarJVM();

/**
 * @brief Funcao para criar um frame com os valores default
 * @param *classeCorrente Ponteiro da Classe Corrente do frame
 * @param max_locals Tamanho do vetor de variaveis locais do frame
 * @return Retorna um ponteiro para uma estrutura frame
 */
frame* criarFrame(char *classeCorrente, u2 max_locals);

/**
 * @brief Funcao inicia a interpretacao do arquivo .class
 */
void executarJVM();

/**
 * @brief Funcao para atualiar o PC de acordo com ultima instrucao executada
 * @param *code Ponteiro para o codigo da instrucao atual
 * @param length Tamanho do codigo que deve ser verificado
 * @return Retorna o novo valor para PC
 */
u1 * atualizarPCMetodoAtual (u1 * code, u4 length);

/**
 * @brief Funcao para imprimir o Atribute Code de um metodo
 * @param *code Ponteiro para o codigo que deve ser impresso
 * @param length Tamanho do codigo que deve ser analisado
 */
void ImprimeCode (u1 * code, u4 length);

/**
 * @brief Avalia se a instrucao fornecida como parametro é do tipo branch
 * @param *nomeInstrucao Nome da Instrucao a ser analisada
 */
bool instrucaoBranch (char * nomeInstrucao);

/**
 * @brief Funcao para executar o codigo de um metodo
 * @param *m Estrutura method_info representando o metodo que sera executado
 * @param *classeCorrente Ponteiro para a classe corrente do metodo
 * @param chamador Inteiro que é 1 se o chamador foi o arquivo da JVM e 2 se foi umma instrucao do tipo invoke
 */
void executarMetodo(method_info *m, char *classeCorrente, int chamador);

/**
 * @brief Funcao para interpretar as instrucoes do codigo de um metodo
 * @param *code Ponteiro para o codigo que deve ser interpretado
 * @param length Tamanho do codigo que deve ser interpretado
 * @param *m Estrutura method_info representando o metodo que deve ser interpretado
 */
void interpretaCode(u1 *code, u4 length,method_info *m);

/**
 * @brief Funcao para liberar o vetor de variaveis locais da memoria
 * @param *v Vetor do tipo vetor_locais que sera desalocado
 * @param vetor_length Tamanho do vetor que sera desalocado
 */
void freeVetorLocais(vetor_locais *v, u2 vetor_length);

/**
 * @brief Funcao para encontrar o PC do handler da excecao lancada na JVM
 * @param *m Estrutura method_info no qual o handler da excecucao sera buscado
 * @return Retorna o PC do handler da excecao lancada na JVM
 */
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
