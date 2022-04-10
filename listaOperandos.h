
/* *
* @file lista_operandos.h
@brief Estrutura e * cabeçalho da implementação da pilha de operações
 */

#ifndef LISTA_OPERANDOS_H
#define LISTA_OPERANDOS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>



typedef uint8_t u1;   
typedef uint16_t u2;  
typedef uint32_t u4;  
typedef uint64_t u8;  

typedef int8_t i1;   
typedef int16_t i2;  
typedef int32_t i4;  
typedef int64_t i8;  

typedef struct lista_operandos{
	i4 operando;               	
	void *referencia;    		
	u1 tipo_operando; 				
	struct lista_operandos *prox;  
	struct lista_operandos *ant; 	
}lista_operandos;

/* *
* @struct lista_operandos
*
* @brief Estrutura de dados da lista de operações.
* @consulte https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-2.html#jvms-2.6.2
*
 */


enum tipos_operandos{
	BOOLEAN_OP = 1,
	BYTE_OP,
	CHAR_OP,
	SHORT_OP,
	INTEGER_OP,
	FLOAT_OP,
	LONG_OP,
	DOUBLE_OP,
	RETURN_ADDRESS_OP,
	REFERENCE_OP,
	REFERENCE_ARRAY_BOOLEAN_OP,
	REFERENCE_ARRAY_CHAR_OP,
	REFERENCE_ARRAY_FLOAT_OP,
	REFERENCE_ARRAY_DOUBLE_OP,
	REFERENCE_ARRAY_BYTE_OP,
	REFERENCE_ARRAY_SHORT_OP,
	REFERENCE_ARRAY_INT_OP,
	REFERENCE_ARRAY_LONG_OP,
	REFERENCE_STRING_OP,
};

lista_operandos* CriaListaOperandos();

lista_operandos* InsereInicioOperandos(lista_operandos *lista, i4 operando, void *referencia, u1 tipo_operando);

lista_operandos* InsereFimOperandos(lista_operandos *lista, i4 operando, void *referencia, u1 tipo_operando);

lista_operandos* RemoveInicioOperandos(lista_operandos *lista);

lista_operandos* RemoveFimOperandos(lista_operandos *lista);

void ImprimeListaOperandos(lista_operandos *lista);

void LiberaListaOperandos(lista_operandos *lista);

lista_operandos* RemoveElementoOperandos(lista_operandos *lista,i4 operando, u1 tipo_operando);

lista_operandos* BuscaElementoOperandos(lista_operandos *lista, i4 operando, u1 tipo_operando);

lista_operandos* BuscaPosicaoOperandos(lista_operandos *lista, int posicao);

lista_operandos* InserePosicaoOperandos(lista_operandos *lista, i4 operando, u1 tipo_operando, int posicao);

lista_operandos* RemovePosicaoOperandos(lista_operandos *lista, int posicao);

#endif


/* *
* @brief Inicializa uma lista de operandos
* @details A função cria uma lista de operadores sem valor e retorna o ponteiro para ela.
* @return Lista de operações com valor nulo
 */
lista_operandos* CriarLista_operandos ();
/* *
* @brief Insere um operando (valor ou referência) no início da lista de operandos.
* @details O tipo do operando é checado para que seja feita a inserção correta, e então
* é alocado um espaço na memória correspondente, atribuindo ao operando enviado.
* Apenas um tipo de operação deve ser enviado nesta função.
*
* @param lis Ponteiro para a lista de operandos
* @param operando Operando de valor direto a ser inserido na lista
* @param Operando de referência a ser inserido na lista
* @param tipo_operando Definição do tipo de operação a ser inserido
* @return Ponteiro para a lista de operandos após a inserção do novo operando
 */
lista_operandos* InserirInicio_operandos (lista_operandos *lis, i4 operando, void *referencia, u1 tipo_operando);
/* *
* @brief Insere um operando (valor ou referência) no final da lista de operandos.
* @details O tipo do operando é checado para que seja feita a inserção correta, e então
* é alocado um espaço na memória correspondente, atribuindo ao operando enviado.
* Apenas um tipo de operação deve ser enviado nesta função.
*
* @param lis Ponteiro para a lista de operandos
* @param operando Operando de valor direto a ser inserido na lista
* @param Operando de referência a ser inserido na lista
* @param tipo_operando Definição do tipo de operação a ser inserido
* @return Ponteiro para a lista de operandos após a inserção do novo operando
 */
