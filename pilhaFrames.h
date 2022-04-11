/*
pilhaFrames.h
Cabeçalho da implementação da Pilha de Frames
 */

// Definição das macros
#ifndef PILHA_FRAMES_H
#define PILHA_FRAMES_H

// Bibliotecas utilizadas
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaFrames.h"

/*
Definição do tipo de estrutura 'pilha_frames'
 */
typedef struct pilha_frames{
	struct lista_frames *topo; 
}pilha_frames;

/*
 Cria uma nova Pilha de Frames
 Esta funçao cria uma nova Pilha de Frames para uso posterior
 retorna Pilha de Frames vazia
 */
pilha_frames* CriaPilhaFrames();

/*
 Insere um novo frame no topo da Pilha de Frames
 aponta *pilha Ponteiro da Pilha de Frames
 aponta *f Novo Frame a ser inserido
 retorna Pilha de Frames com o novo frame no topo
 */
pilha_frames* PushFrames(pilha_frames *pilha, frame *f);

/*
 Remove o frame no topo da Pilha de Frames
 *pilha Ponteiro da Pilha de Frames
 retorna Pilha de Frames sem o frame do topo
 */
pilha_frames* PopFrames(pilha_frames *pilha);

/*
 Acessa o topo da Pilha de Frames
 aponta *pilha Ponteiro da Pilha de Frames
 retorna Ponteiro do no do topo da Pilha de Frames
 */
pilha_frames* TopoFrames(pilha_frames *pilha);

/*
Imprime na tela o conteudo da Pilha de Frames
aponta *pilha Ponteiro da Pilha de Frames
 */
void ImprimePilhaFrames(pilha_frames *pilha);

#endif
