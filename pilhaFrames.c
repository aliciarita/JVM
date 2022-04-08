#include "pilhaFrames.h"

pilha_frames* CriaPilhaFrames(){
	pilha_frames *pilha = malloc(sizeof(pilha_frames));
	pilha->topo = NULL;
	return pilha;
}

pilha_frames* PushFrames(pilha_frames *pilha, frame *f){
	pilha->topo = InsereInicioFrames(pilha->topo,f);
	return pilha;
}

pilha_frames* PopFrames(pilha_frames *pilha){
	pilha_frames *aux = CriaPilhaFrames();
	aux = PushFrames(aux,pilha->topo->f);
	pilha->topo = RemoveInicioFrames(pilha->topo);
	return (aux);
}

pilha_frames* TopoFrames(pilha_frames *pilha){
	return pilha;
}

void ImprimePilhaFrames(pilha_frames *pilha){
	ImprimeListaFrames(pilha->topo);
}
