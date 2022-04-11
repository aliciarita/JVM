/*
 pilhaOperandos.h
 Cabeçalho da implementação da Pilha de Operandos
 */
#ifndef PILHA_OPERANDOS_H
#define PILHA_OPERANDOS_H

// Bibliotecas
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaOperandos.h"

// Definição dos tipos de inteiros sem sinal utilizados
typedef uint8_t u1; 	
typedef uint16_t u2; 	
typedef uint32_t u4; 	
typedef uint64_t u8; 	

/*
 struct pilha_operandos
 Estrutura de dados da Pilha de Operandos
 */
typedef struct pilha_operandos{
	struct lista_operandos *topo; 
}pilha_operandos;

/*
 - Cria uma nova Pilha de Operandos
 Cria uma nova Pilha de Operandos vazia para uso posterior
 retorna Pilha de Operandos vazia
 */
pilha_operandos* CriaPilhaOperandos();

/*
 Insere um operando no topo da Pilha de Operando
 aponta *pilha Ponteiro para a Pilha de Operandos
 operando Valor direto do Operando a ser inserido na Pilha de Operandos
 referência Operando de referência a ser inserido na Pilha de Operandos
 tipo_operando Definição do tipo de operando a ser inserido na Pilha de Operandos
 retorna o Ponteiro para a Pilha de Operandos com o novo operando inserido
 */
pilha_operandos* PushOperandos(pilha_operandos *pilha, u4 operando, void *referencia, u1 tipo_operando);

/*
 Remove o operando do topo da Pilha de Operandos 
 aponta *pilha Ponteiro para a Pilha de Operandos
 retorna Pilha de operandos com operando do topo removido
 */
pilha_operandos* PopOperandos(pilha_operandos *pilha);

/**
 Acessa o topo da Pilha de Operandos
 aponta *pilha Ponteiro para a Pilha de Operandos
 retorna Ponteiro para o topo da Pilha de Operandos
 */
pilha_operandos* TopoOperandos(pilha_operandos *pilha);

/*
Verifica se a Pilha de Operandos esta vazia
Verifica se o topo da Pilha de Operandos contem o valor NULL
aponta *pilha Ponteiro para a Pilha de Operandos
retorna o Booleano TRUE se a pilha esta vazia ou FALSE caso contrario
 */
bool pilhaVazia (pilha_operandos * pilha);

/*
 Printa o conteudo da Pilha de Operandos
 aponta *pilha Ponteiro para a Pilha de Operandos
 */
void ImprimePilhaOperandos(pilha_operandos *pilha);

/*
 Verifica se o topo da Pilha de operandos é um salto de linha
 aponta *pilha Ponteiro para a Pilha de Operandos
 */
bool printVazio (pilha_operandos * pilha);

#endif
