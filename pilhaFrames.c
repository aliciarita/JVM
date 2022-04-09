/**
 * @file pilhaFrames.c
 * @brief Funcoes para o controle da Pilha de Frames
 */

#include "pilhaFrames.h"

// Cria nova Pilha de Frames
pilha_frames* CriaPilhaFrames(){
	pilha_frames *pilha = malloc(sizeof(pilha_frames));
	pilha->topo = NULL;
	return pilha;
}

// Incere um novo frame na Pilha de Frames
pilha_frames* PushFrames(pilha_frames *pilha, frame *f){
	pilha->topo = InsereInicioFrames(pilha->topo,f);
	return pilha;
}

// Remove o frame do topo da Pilha de Frames
pilha_frames* PopFrames(pilha_frames *pilha){
	pilha_frames *aux = CriaPilhaFrames();
	aux = PushFrames(aux,pilha->topo->f);
	pilha->topo = RemoveInicioFrames(pilha->topo);
	return (aux);
}

// Retorna o frame do topo da Pilha de Frames
pilha_frames* TopoFrames(pilha_frames *pilha){
	return pilha;
}

// Imprime o conteudo da Pilha de Frames
void ImprimePilhaFrames(pilha_frames *pilha){
	ImprimeListaFrames(pilha->topo);
}
