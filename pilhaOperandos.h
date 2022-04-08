#ifndef PILHA_OPERANDOS_H
#define PILHA_OPERANDOS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaOperandos.h"

typedef uint8_t u1; 	
typedef uint16_t u2; 	
typedef uint32_t u4; 	
typedef uint64_t u8; 	


typedef struct pilha_operandos{
	struct lista_operandos *topo; 
}pilha_operandos;

pilha_operandos* CriaPilhaOperandos();

pilha_operandos* PushOperandos(pilha_operandos *pilha, u4 operando, void *referencia, u1 tipo_operando);

pilha_operandos* PopOperandos(pilha_operandos *pilha);

pilha_operandos* TopoOperandos(pilha_operandos *pilha);

bool pilhaVazia (pilha_operandos * pilha);

void ImprimePilhaOperandos(pilha_operandos *pilha);

bool printVazio (pilha_operandos * pilha);

#endif
