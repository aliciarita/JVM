

/* 
 lista_operandos.c
 Funções de controle da lista de operadores
 */

#include "listaOperandos.h"

lista_operandos* CriarLista_operandos(){
	return NULL;
}

lista_operandos* InsereInicioOperandos(lista_operandos* lista, i4 operando, void* referencia, u1 tipo_operando){
	lista_operandos *novo= malloc(sizeof(lista_operandos));
	if(tipo_operando<=8){
		novo->operando = operando;
		novo->referencia = NULL;
	}
	else{
		novo->operando = -INT_MAX;
		novo->referencia = referencia;
	}
	novo->tipo_operando = tipo_operando;
	novo->prox = lista;
	novo->ant = NULL;
	if (lista!=NULL){
		lista->ant = novo;
	}
	return novo;
}

lista_operandos* InsereFimOperandos(lista_operandos* lista, i4 operando, void *referencia, u1 tipo_operando){
	lista_operandos *novo; 
	lista_operandos *ant = NULL; 
	lista_operandos *p = lista;
	while(p!=NULL){
		ant = p;
		p=p->prox;
	}
	novo = malloc(sizeof(lista_operandos));
	if(tipo_operando <= 8){
	novo->operando = operando;
	novo->referencia = NULL;
	}
	else {
		novo->operando = -INT_MAX;
		novo->referencia = referencia;
	}
	novo->tipo_operando = tipo_operando;
	novo->prox = NULL;
	novo->ant = ant;
	ant->prox = novo;
	return (lista);
}

lista_operandos* RemoveInicioOperandos(lista_operandos* lista){
	lista_operandos* p=lista;
	lista = p->prox;
	if(lista!=NULL){
		lista->ant = NULL;
	}
	free(p);
	return (lista);
}

lista_operandos* RemoveFimOperandos(lista_operandos* lista){
	lista_operandos *p=lista, *ant=NULL;
	while (p!=NULL){
		ant = p;
		p=p->prox;
	}
	ant->ant->prox = NULL;
	free(ant);
	return (lista);
}

lista_operandos* RemoveElementoOperandos(lista_operandos* lista,i4 operando,u1 tipo_operando){
	lista_operandos *p = BuscaElementoOperandos(lista, operando, tipo_operando);
	if(p==NULL){
		return lista;
	}
	if (lista == p){
		lista=p->prox;
	}else{
		p->ant->prox = p->prox;
	}
	if (p->prox!=NULL){
		p->prox->ant = p->ant;
	}
	free(p);
	return lista;
}

lista_operandos* BuscaElementoOperandos(lista_operandos* lista, i4 operando, u1 tipo_operando){
	lista_operandos *p;
	for(p=lista;p!=NULL;p=p->prox){
		if(p->operando == operando && p->tipo_operando == tipo_operando)
			return p;
	}
	return(NULL);
}

lista_operandos* BuscaPosicaoOperandos(lista_operandos *lista, int posicao){
	lista_operandos *p;
	int cont;
	for(p=lista, cont=0; p!=NULL && cont<posicao;){
		p=p->prox;
		cont++;
	}
	return(p);
}

lista_operandos* InserePosicaoOperandos(lista_operandos *lista, i4 operando, u1 tipo_operando, int posicao){
	lista_operandos *novo; 
	lista_operandos *ant=NULL; 
	lista_operandos *p;
	int cont;
	for(p=lista,cont=0;p!=NULL && cont<posicao;){
		ant=p;
		p=p->prox;
		cont++;
	}
	novo = malloc(sizeof(lista_operandos));
	novo->operando = operando;
	novo->tipo_operando = tipo_operando;

	if (ant==NULL){
		novo->prox = lista;
		novo->ant = NULL;
		lista = novo;
	}else{
		ant->prox = novo;
		novo->ant = ant;
		novo->prox = p;
		if(p!=NULL)
			p->ant = novo;
	}
	return(lista);
}

lista_operandos* RemovePosicaoOperandos(lista_operandos *lista, int posicao){
	lista_operandos *p = BuscaPosicaoOperandos(lista, posicao);
	if(p==NULL)
		return lista;
	if (lista == p){
		lista=p->prox;
	}else{
		p->ant->prox = p->prox;
	}
	if (p->prox!=NULL){
		p->prox->ant = p->ant;
	}
	free(p);
	return (lista);
}

void ImprimeListaOperandos(lista_operandos *lista){
	lista_operandos *p;
	for(p=lista;p!=NULL;p=p->prox){
		printf("Tipo: %d\t",p->tipo_operando);
		if(p->tipo_operando<=8){
			printf("Operando: 0x%08x\n\n",p->operando);
		}
		else{
			switch(p->tipo_operando){
				case RETURN_ADDRESS_OP:
					printf("Operando: %s\n\n",(char*) p->referencia);
				break;
				case REFERENCE_ARRAY_BOOLEAN_OP:
					printf("[Z@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_CHAR_OP:
					printf("Operando: %s\n\n",(char*) p->referencia);
				break;
				case REFERENCE_ARRAY_FLOAT_OP:
					printf("[F@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_DOUBLE_OP:
					printf("[D@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_BYTE_OP:
					printf("[B@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_SHORT_OP:
					printf("[S@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_INT_OP:
					printf("[I@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_ARRAY_LONG_OP:
					printf("[J@%p\n",(u4*) p->referencia);
				break;
				case REFERENCE_STRING_OP:
					printf("String: %s\n", (char *) p->referencia);
				break;
				case REFERENCE_OP:
					printf("Operando: 0x%p\n\n", (u4*) p->referencia);
				break;
			}
		}
	}
}

void LiberaListaOperandos(lista_operandos *lista){
	lista_operandos *p=lista;
	while(p!=NULL){
		lista_operandos *t = p->prox;
		free(p);
		p=t;
	}
}
