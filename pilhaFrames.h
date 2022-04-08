#ifndef PILHA_FRAMES_H
#define PILHA_FRAMES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaFrames.h"

typedef struct pilha_frames{
	struct lista_frames *topo; 
}pilha_frames;

pilha_frames* CriaPilhaFrames();

pilha_frames* PushFrames(pilha_frames *pilha, frame *f);

pilha_frames* PopFrames(pilha_frames *pilha);

pilha_frames* TopoFrames(pilha_frames *pilha);

void ImprimePilhaFrames(pilha_frames *pilha);

#endif