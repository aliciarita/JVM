/**
 * @file pilhaFrames.h
 * @brief Cabeçalho da implementaçao da Pilha de Frames
 */

// Definicao das macros
#ifndef PILHA_FRAMES_H
#define PILHA_FRAMES_H

// Bibliotecas utilizadas
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaFrames.h"

/**
 * @brief Definicao do tipo de estrutura 'pilha_frames'
 */
typedef struct pilha_frames{
	struct lista_frames *topo; 
}pilha_frames;

/**
 * @brief Cria uma nova Pilha de Frames
 * @details Esta funçao cria uma nova Pilha de Frames para uso posterior
 * @return Pilha de Frames vazia
 */
pilha_frames* CriaPilhaFrames();

/**
 * @brief Insere um novo frame no topo da Pilha de Frames
 * @param *pilha Ponteiro da Pilha de Frames
 * @param *f Novo Frame a ser inserido
 * @return Pilha de Frames com o novo frame no topo
 */
pilha_frames* PushFrames(pilha_frames *pilha, frame *f);

/**
 * @brief Remove o frame no topo da Pilha de Frames
 * @param *pilha Ponteiro da Pilha de Frames
 * @return Pilha de Frames sem o frame do topo
 */
pilha_frames* PopFrames(pilha_frames *pilha);

/**
 * @brief Acessa o topo da Pilha de Frames
 * @param *pilha Ponteiro da Pilha de Frames
 * @return Ponteiro do no do topo da Pilha de Frames
 */
pilha_frames* TopoFrames(pilha_frames *pilha);

/**
 * @brief Imprime na tela o conteudo da Pilha de Frames
 * @param *pilha Ponteiro da Pilha de Frames
 */
void ImprimePilhaFrames(pilha_frames *pilha);

#endif
