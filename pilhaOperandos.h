/**
 * @file pilhaOperandos.h
 * @brief Cabecalho da implementacao da Pilha de Operandos
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

// Definicao dos tipos de inteiros sem sinal utilizados
typedef uint8_t u1; 	
typedef uint16_t u2; 	
typedef uint32_t u4; 	
typedef uint64_t u8; 	

/**
 * @ struct pilha_operandos
 * @ brief Estrutura de dados da Pilha de Operandos
 */
typedef struct pilha_operandos{
	struct lista_operandos *topo; 
}pilha_operandos;

/**
 * @brief Cria uma nova Pilha de Operandos
 * @details Cria uma nova Pilha de Operandos vazia para uso posterior
 * @return Pilha de Operandos vazia
 */
pilha_operandos* CriaPilhaOperandos();

/**
 * @brief Insere um operando no topo da Pilha de Operando
 * @param *pilha Ponteiro para a Pilha de Operandos
 * @param operando Valor direto do Operando a ser inserido na Pilha de Operandos
 * @param referencia Operando de referencia a ser inserido na Pilha de Operandos
 * @param tipo_operando Definicao do tipo de operando a ser inserido na Pilha de Operandos
 * @return Ponteiro para a Pilha de Operandos com o novo operando inserido
 */
pilha_operandos* PushOperandos(pilha_operandos *pilha, u4 operando, void *referencia, u1 tipo_operando);

/**
 * @brief Remove o operando do topo da Pilha de Operandos 
 * @param *pilha Ponteiro para a Pilha de Operandos
 * @return Pilha de operandos com operando do topo removido
 */
pilha_operandos* PopOperandos(pilha_operandos *pilha);

/**
 * @brief Acessa o topo da Pilha de Operandos
 * @param *pilha Ponteiro para a Pilha de Operandos
 * @return Ponteiro para o topo da Pilha de Operandos
 */
pilha_operandos* TopoOperandos(pilha_operandos *pilha);

/**
 * @brief Verifica se a Pilha de Operandos esta vazia
 * @details Verifica se o topo da Pilha de Operandos contem o valor NULL
 * @param *pilha Ponteiro para a Pilha de Operandos
 * @return Booleano TRUE se a pilha esta vazia ou FALSE caso contrario
 */
bool pilhaVazia (pilha_operandos * pilha);

/**
 * @brief Imprime o conteudo da Pilha de Operandos
 * @param *pilha Ponteiro para a Pilha de Operandos
 */
void ImprimePilhaOperandos(pilha_operandos *pilha);

/**
 * @brief Verifica se o topo da Pilha de operandos é um salto de linha
 * @param *pilha Ponteiro para a Pilha de Operandos
 */
bool printVazio (pilha_operandos * pilha);

#endif
