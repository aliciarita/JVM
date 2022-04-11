/*
 pilhaOperandos.c
 Arquivo de implementação das funcões utilizadas pela Pilha de Operandos
 */

#include "pilhaOperandos.h"

/*
 Cria uma Pilha de Operandos
 */
pilha_operandos* CriaPilhaOperandos(){
	pilha_operandos *pilha = malloc(sizeof(pilha_operandos));
	pilha->topo = NULL;
	return (pilha);
}

/*
  coloca um novo operando na Pilha de Operandos
 */
pilha_operandos* PushOperandos(pilha_operandos *pilha, u4 operando, void *referencia, u1 tipo_operando){
	pilha->topo = InsereInicioOperandos(pilha->topo,operando,referencia,tipo_operando);
	return (pilha);
}

/*
  Remove o operando do topo da Pilha de Operandos
 */
pilha_operandos* PopOperandos(pilha_operandos *pilha){
	pilha_operandos *aux = CriaPilhaOperandos();
	aux = PushOperandos(aux, pilha->topo->operando,pilha->topo->referencia, pilha->topo->tipo_operando);
	pilha->topo = RemoveInicioOperandos(pilha->topo);
	return (aux);
}

/*
  verifica se Pilha de Operandos esta vazia ou não
 */
bool pilhaVazia (pilha_operandos * pilha){
	if (pilha->topo == NULL)
		return true;
	else
		return false;
}

/*
  Informa se o opernando no topo da Pilha de Operandos é um salto de linha
 */
bool printVazio (pilha_operandos * pilha) {
	int contador = 0;
	u1 tipoOpAux;
	for (lista_operandos * lo = pilha->topo; lo != NULL; lo = lo->prox) {
		contador++;
		tipoOpAux = lo->tipo_operando;
	}
	if (contador == 1 && tipoOpAux == 10)
		return true;
	else 
		return false;
}

/*
  Acessa o topo da Pilha de Operandos
 */
pilha_operandos* TopoOperandos(pilha_operandos *pilha){
	return (pilha);
}

/*
 Printa na tela o conteudo da Pilha de Operandos
 */
void ImprimePilhaOperandos(pilha_operandos *pilha){
	ImprimeListaOperandos(pilha->topo);
}
