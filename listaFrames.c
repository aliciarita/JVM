
/* 
 lista_frames.c
Funções de controle da lista de frames
 */


#include "listaFrames.h"

lista_frames* CriaListaFrames(){
	return NULL;
}

lista_frames* InsereInicioFrames(lista_frames* lista,frame *f){
	lista_frames *novo = (lista_frames*) malloc(sizeof(lista_frames));
	novo->f = f;
	novo->prox = lista;
	novo->ant = NULL;
	
	if (lista!=NULL){
		lista->ant = novo;
	}

	
	return novo;
}

lista_frames* InsereFimFrames(lista_frames* lista,frame *f){
	lista_frames *novo = malloc(sizeof(lista_frames));
	lista_frames *ant = NULL; 
	lista_frames *p = lista;
	while(p!=NULL){
		ant = p;
		p=p->prox;
	}
	novo->f = f;
	novo->prox = NULL;
	novo->ant = ant;
	ant->prox = novo;
	return (lista);
}

lista_frames* RemoveInicioFrames(lista_frames* lista){
	lista_frames* p=lista;
	lista = p->prox;
	if(lista!=NULL){
		lista->ant = NULL;
	}
	free(p);
	return (lista);
}

lista_frames* RemoveFimFrames(lista_frames* lista){
	lista_frames *p=lista;
	lista_frames *ant=NULL;
	while (p!=NULL){
		ant = p;
		p=p->prox;
	}
	ant->ant->prox = NULL;
	free(ant);
	return (lista);
}

void ImprimeListaFrames(lista_frames *lista){
	lista_frames *f;
	for(f=lista; f!=NULL; f=f->prox){
		printf("%04x\n\n", f->f->end_retorno);
	}
}

void LiberaListaFrames(lista_frames *lista){
	lista_frames *p=lista;
	while(p!=NULL){
		lista_frames *t = p->prox;
		free(p);
		p=t;
	}
}
