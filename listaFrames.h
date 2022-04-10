
/* *
* @arquivo lista_frames.h
@brief Estrutura e cabeçalho da implementação da lista de frames
 */


#ifndef LISTA_FRAMES_H
#define LISTA_FRAMES_H

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"


/* *
* @struct lista_frames
*
* @brief Estrutura de dados da lista de frames
*
* @ver estruturas.h
 */

typedef struct lista_frames{
	frame *f; 						 	
	struct lista_frames *prox;          
	struct lista_frames *ant;  	       
}lista_frames;

lista_frames* CriaListaFrames();

lista_frames* InsereInicioFrames(lista_frames *lis, frame *f); 

lista_frames* InsereFimFrames(lista_frames *lis, frame *f);

lista_frames* RemoveInicioFrames(lista_frames *lis);

lista_frames* RemoveFimFrames(lista_frames *lis);

void ImprimeListaFrames(lista_frames *lis);

void LiberaListaFrames(lista_frames *lis);

#endif
