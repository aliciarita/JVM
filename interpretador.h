/*
 interpretador.h
 Arquivo cabecalho das instrucoes e funcoes auxiliares
 */
#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

// Bibliotecas utilizadas
#include "instrucoes.h"
#include "classFile.h"
#include "jvm.h"
#include <stdbool.h>

// Macro para normalizar os incides da pool de constantes
#define normaliza_indice(x,y) (x << 8) | y

/**
 *  função para resolver (analisar e alocar) uma classe
 *  *nome_classe Nome da classe para ser resolvida
 * retorna Ponteiro para classe resolvida
 */
ClassFile* resolverClasse(char *nome_classe);

/**
 *  função para resolver (analisar e alocar) um metodo
 *  *cp Ponteiro para a pool de constantes
 *  indice_cp Indice para acesso na pool de constantes
 *  interface Flag para sinalizar metodo de interface
 * retorna Booleano representando o sucesso ou fracasso da execucao da resolucao
 */
bool resolverMetodo(cp_info *cp, u2 indice_cp, u1 interface);

/**
 *  função para obter o nome de um metodo na pool de constantes
 *  *cp Ponteiro para a pool de constante
 *  indice_cp Indice de acesso na pool de constante
 *  interface Flag para sinalizar metodo de interface
 * retorna Ponteiro de char representando o nome do metodo
 */
char* obterNomeMetodo(cp_info *cp, u2 indice_cp, u1 interface);

/**
 *  função para obter o descriptor de um metodo
 *  *cp Ponteiro para a pool de constante
 *  indice_cp Indice de acesso na pool de constante
 *  interface Flag para sinalizar metodo de interface
 * retorna Ponteiro de char representando o descriptor do metodo
 */
char* obterDescriptorMetodo(cp_info *cp, u2 indice_cp, u1 interface);

/**
 *  função para obter o tipo do descriptor do metodo
 *  *descriptor Ponteiro de char representando o descriptor do metodo
 * retorna Valor inteiro representando o tipo do descriptor do metodo
 */
int descriptorFieldValidate (char * descriptor);

/**
 *  função para obter a classe executante de um metodo
 *  *cp Ponteiro para pool de constante
 *  indice_cp Indice de acesso na pool de constante
 */
char* obterClasseDoMetodo(cp_info *cp, u2 indice_cp);

/**
 *  função para transferir os valores da Pilha de Operandos de um frame para o vetor de variaveis locais do proximo frame
 *  *anterior Ponteiro para o frame previamente executado
 *  *novo Ponteiro para o novo frame que sera locado para a execucao do metodo
 *  *parametros_cont Ponteiro para o contador de parametros a serem passados para o array de variaveis locais do novo frame
 * retorna Retorna um ponteiro para o novo frame alocado
 */
frame* transferePilhaVetor(frame *anterior, frame *novo, int *parametros_cont);

/**
 *  função para decodificar operandos em um valor double (64 bits)
 *  high Parte alta do valor double
 *  low Parte baixa do valor double
 * return Valor double decodificado
 */
double decodificaDoubleValor(u4 high, u4 low);

/**
 *  função para decodificar operando em valor float (32 bits)
 *  valor 4 bytes do tipo unsigned representando o valor float
 * retorna Valor float decodificado
 */
float decodificaFloatValor(u4 valor);

/**
 *  função para decodificar operando em valor long (64 bits)
 *  high Parte alta do valor long
 *  low Parte baixa do valor long
 * retorna Valor long decodificado
 */
long decodificaLongValor (u4 high, u4 low);

/**
 *  função para obter a quantidade de campos da classe que nao sao estaticos
 *  *classe Ponteiro para a classe alocada
 * retorna Valor inteiro com a quantidade de campos que nao sao do tipo static
 */
int getParametrosNaoStatic (ClassFile * classe);

/**
 *  função para inserir um objeto novo na lista de objetos
 *  *lis Ponteiro para o comeco da lista de objetos
 *  *classe Referencia para a classe a ser alocada na lista
 *  parametrosNaoStatic Quantidade de paramentros que nao sao static
 * retorna Ponteiro para a lista de objetos com um novo objeto inserido
 */
Lista_Objetos * InsereObjeto (Lista_Objetos * lis, ClassFile * classe, int parametrosNaoStatic);

/**
 *  função para verificar se um metodo é do tipo static
 *  *acessFlags Array de char ccontendo o acessflags do metodo
 * retorna Booleano indicando se o metodo é do tipo static ou nao
 */
bool buscaStaticFlags (char * accessFlags);

/**
 *  Função para obter a quantidade de parâmetros a serem passados para um método
 *  *descriptor Array de char contendo o descriptor do método
 * retorna Valor inteiro representando a quantidade de parâmetros a serem passados para o método
 */
int getParametrosCount (char * descriptor);

/**
 *  Função para buscar um objeto na lista de objetos
 *  *p Ponteiro de classe contendo a referência do objeto
 * retorna Retorna um ponteiro para a lista de objetos
 */
Lista_Objetos * buscaObjetoViaReferencia (ClassFile * p);

/**
 *  Função para obter a posição do field no array de dados de instância
 *  *obj Ponteiro para o objeto alocado
 *  *nomeField Nome do field pertencente ao objeto
 * retorna Posição do field no array de dados não estáticos
 */
int getPositionField (ClassFile * obj, char * nomeField);

/**
 *  Função para instanciar um novo objeto
 *  *nomeClasse Array de char contendo o nome da classe
 * retorna Ponteiro para o objeto alocado
 */
ClassFile * instanciarClasse (char * nomeClasse);

/**
 *  Função para obter o tipo de operando que sera alocado na pilha de operandos
 *  *descriptorRetorno Array de char contendo o descriptor do método
 * retorna Valor inteiro representando o tipo do operando que sera alocado na pilha de operandos
 */
int getTipoOperandoSaida(char * descriptorRetorno);

/**
 *  Função para obter o modo de empilhamento do valor de retorno de um método
 * 
 *  descriptor Ponteiro de char contendo a string que representa o descriptor do método
 * retorna Valor inteiro representando o modo de empilhamento do valor de retorno de um método na pilha de operandos
 */
int getTipoRetorno (char * descriptor);

/**
 *  Não executa nada
 *  *par0 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void nop_impl(frame *par0,u1 par1, u1 par2);

/**
 *  Faz o push de um valor nulo para a pilha de operandos 
 *  f Ponteiro para o frame]
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void aconst_null_impl(frame *f, u1 par1, u1 par2);

/**
 *  [brief description]
 *  *f [description]
 *  par1 [description]
 *  par2 [description]
 */
void iconst_m1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 0 (int)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 1 (int)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 2 (int)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 3 (int) 
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iconst_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 4 (int)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iconst_4_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 5 (int)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iconst_5_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 0 (long)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 1 (long)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 0 (float)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 1 (float)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 2 (float)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 0 (double)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Faz o push para a pilha de operando da constante 1 (double)
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * Faz o push de um byte na pilha de operandos
 * *f Ponteiro para o frame
 * Byte a ser empilhado
 * par1 Parâmetro não utilizado
 */

void bipush_impl(frame *f, u1 byte, u1 par1);

/**
 *  Faz o push de um short na pilha de operandos
 *  *f Ponteiro para o frame
 *  byte Primeiro byte do short
 *  par1 Segundo byte do short
 */
void sipush_impl(frame *f,u1 byte1, u1 byte2);

/**
 *  Faz o push de um valor presente na constant pool
 *  *f Ponteiro para o frame
 *  indexbyte1 Índice da constant pool
 *  par2 Parâmetro não utilizado
 */
void ldc_impl(frame *f,u1 indexbyte1,u1 par2);

/**
 *  Faz o push de um valor presente na constant pool
 *  *f Ponteiro para o frame
 *  indexbyte1 Índice da constant pool (primeira parte)
 *  indexbyte2 Índice da constant pool (segunda parte)
 */
void ldc_w_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Faz o push de um valor long ou double presente na constant pool
 *  *f Ponteiro para o frame
 *  branchbyte1 Índice da constant pool (primeira parte)
 *  branchbyte2 Índice da constant pool (segunda parte)
 */
void ldc2_w_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Carrega um valor int do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  index Índice no vetor de variáveis locais
 *  par1 Parâmetro não utilizado
 */
void iload_impl(frame *f, u1 index, u1 par1);

/**
 *  Carrega um valor long do vetor de variáveis locais 
 *  *f Ponteiro para o frame
 *  index Índice no vetor de variáveis locais
 *  par1 Parâmetro não utilizado
 */
void lload_impl(frame *f, u1 index, u1 par1);

/**
 *  Carrega um valor float do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  index Índice no vetor de variáveis locais
 *  par1 Parâmetro não utilizado
 */
void fload_impl(frame *f, u1 index, u1 par1);

/**
 *  Carrega um valor double do vetor de variáveis locais 
 *  *f Ponteiro para o frame
 *  index Índice no vetor de variáveis locais
 *  par1 Parâmetro não utilizado
 */
void dload_impl(frame *par0, u1 par1,u1 par2);

/**
 *  Carrega uma referência do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  index Índice no vetor de variáveis locais
 *  par1 Parâmetro não utilizado
 */
void aload_impl(frame *f, u1 index, u1 par1);

/**
 *  Carrega um valor int da posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void iload_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor int da posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void iload_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor int da posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void iload_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor int da posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void iload_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor long da posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void lload_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor long da posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void lload_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor long da posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void lload_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor long da posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void lload_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor float da posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void fload_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor float da posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void fload_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor float da posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void fload_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor float da posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void fload_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor double da posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void dload_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor double da posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void dload_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor double da posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void dload_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um valor double da posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void dload_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega uma referência da posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void aload_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega uma referência da posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void aload_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega uma referência da posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void aload_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega uma referência da posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par2 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 */
void aload_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um int de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iaload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um long de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void laload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um float de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void faload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um double de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void daload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega uma referência de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void aaload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um byte de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void baload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um char de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void caload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Carrega um short de um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void saload_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um int no vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  index Índice do vetor de variáveis locais
 */
void istore_impl(frame *f, u1 index, u1 par1);

/**
 *  Armazena um long no vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  index Índice do vetor de variáveis locais
 */
void lstore_impl(frame *f, u1 index, u1 par1);

/**
 *  Armazena um float no vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  index Índice do vetor de variáveis locais
 */
void fstore_impl(frame *f, u1 index, u1 par1);

/**
 *  Armazena um double no vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  index Índice do vetor de variáveis locais
 */
void dstore_impl(frame *f, u1 index, u1 par1);

/**
 *  Armazena uma referência no vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  index Índice do vetor de variáveis locais
 */
void astore_impl(frame *f,u1 index, u1 par1);

/**
 *  Armazena um int na posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void istore_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um int na posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void istore_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um int na posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void istore_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um int na posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void istore_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um long na posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lstore_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um long na posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lstore_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um long na posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lstore_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um long na posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lstore_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um float na posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fstore_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um float na posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fstore_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um float na posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fstore_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um float na posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fstore_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um double na posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dstore_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um double na posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dstore_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um double na posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dstore_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um double na posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dstore_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena uma referência na posição 0 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void astore_0_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena uma referência na posição 1 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void astore_1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena uma referência na posição 2 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void astore_2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena uma referência na posição 3 do vetor de variáveis locais
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void astore_3_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor int em um array
 * 
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor long em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor float em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor double em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena uma referência em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void aastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor byte em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void bastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor char em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void castore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Armazena um valor short em um array
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void sastore_impl(frame *f, u1 par1, u1 par2);

/**
 *  Remove o valor de topo da pilha de operandos
 *  *f Ponteiro para o frame
 */
pilha_operandos* pop_impl(frame *f);

/**
 *  Remove os dois valores de topo da pilha de operandos
 *  *f Ponteiro para o frame
 */
pilha_operandos** pop2_impl(frame *f);

/**
 *  PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop
 *  *par0 frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void pop_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop2
 *  *par0 frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void pop2_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  Duplica o valor no topo da pilha de operandos.
 *  *f frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dup_impl(frame *f, u1 par1, u1 par2);

/**
 *  Duplica o valor no topo da pilha de operandos e insere dois valores a baixo.
 *  *f frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dup_x1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Duplica o valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 *  *f frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dup_x2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere logo abaixo.
 *  *f frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dup2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois valores a baixo.
 *  *f frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dup2_x1_impl(frame *f, u1 par1, u1 par2);

/**
 *  Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 *  *f frame.
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dup2_x2_impl(frame *f, u1 par1, u1 par2);

/**
 *  Inverte os dois operandos do topo da pilha
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void swap_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma soma de int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iadd_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma soma de long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void ladd_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma soma de float
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fadd_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma soma de double
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dadd_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma subtração de int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void isub_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma subtração de long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lsub_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma subtração de float
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fsub_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma subtração de double
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dsub_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma multiplicação de int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void imul_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma multiplicação de long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lmul_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma multiplicação de float
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fmul_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma multiplicação de double
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dmul_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma divisão de int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void idiv_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma divisão de long
 *  ]*f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void ldiv_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma divisão de float
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fdiv_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa uma divisão de double
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void ddiv_impl(frame *f, u1 par1, u1 par2);

/**
 *  Calcula o resto int da divisão entre dois operandos
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void irem_impl(frame *f, u1 par1, u1 par2);

/**
 *  Calcula o resto long da divisão entre dois operandos
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lrem_impl(frame *f, u1 par1, u1 par2);

/**
 *  Calcula o resto float da divisão entre dois operandos
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void frem_impl(frame *f, u1 par1, u1 par2);

/**
 *  Calcula o resto double da divisão entre dois operandos
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void drem_impl(frame *f, u1 par1, u1 par2);

/**
 *  Inverte o sinal de um int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void ineg_impl(frame *f, u1 par1, u1 par2);

/**
 *  Inverte o sinal de um long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void lneg_impl(frame *f, u1 par1, u1 par2);

/**
 *  Inverte o sinal de um float
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void fneg_impl(frame *f, u1 par1, u1 par2);

/**
 *  Inverte o sinal de um double
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void dneg_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa um shift left em um int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void ishl_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa um shift left em um long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void lshl_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa um shift right em um int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void ishr_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa um shift right em um long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void lshr_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa um shift right lógico em um int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void iushr_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa um shift right lógico em um long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado		
 *  par2 Parâmetro não utilizado
 */
void lushr_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa a operação "AND" de dois int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void iand_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa a operação "AND" de dois long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void land_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa a operação "OR" de dois int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void ior_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa a operação "OR" de dois long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lor_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa a operação "XOR" de dois int
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void ixor_impl(frame *f, u1 par1, u1 par2);

/**
 *  Executa a operação "XOR" de dois long
 *  *f Ponteiro para o frame
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lxor_impl(frame *f, u1 par1, u1 par2);

/**
 *  Função para mapear a execução da instrução iinc
 *  *par0 Frame do método corrente que está executando a instrução atual
 *  par1 Byte para indexar o acesso no vetor de variáveis locais
 *  par2 Valor inteiro contendo o incremento a ser realizado
 */
void iinc_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  Instrução para incrementar um valor no vetor de variáveis locais do frame
 *  *f Frame do método corrente que está executando a instrução atual
 *  index Byte para indexar o acesso no vetor de variáveis locais
 *  ante Valor inteiro contendo o incremento a ser realizado
 */
void iinc_impl(frame *f, u1 index, i1 constante);

/**
 *  Função para mapear a execução da instrução iinc_wide
 *  *f Frame do método corrente que está executando a instrução atual
 *  indexbyte1 Byte high para indexar o acesso no vetor de variáveis locais
 *  indexbyte2 Byte low para indexar o acesso no vetor de variáveis locais
 *  byte1 Byte high a ser utilizado no incremento da variável no vetor de variáveis locais
 *  byte2 Byte low a ser utilizado no incremento da variável no vetor de variáveis locais
 */
void iinc_wide_fantasma(frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2);

/**
 *  Instrução para incrementar um valor no vetor de variáveis locais do frame
 *  *f Frame do método corrente que está executando a instrução atual
 *  indexbyte Byte duplo para indexar o acesso no vetor de variáveis locais
 *  byte Byte duplo contendo o incremento no vetor de variáveis locais
 */
void iinc_wide(frame *f, u2 indexbyte, i2 constbyte);

/**
 *  Instrução para converter valor inteiro para long
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void i2l_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor inteiro para float
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void i2f_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor inteiro para double
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void i2d_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor long para inteiro
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void l2i_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor long para float
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void l2f_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor long para double
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void l2d_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor float para inteiro
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void f2i_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor float para long
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void f2l_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor float para double
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void f2d_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor double para inteiro
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void d2i_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor double para long
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void d2l_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor double para float
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void d2f_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor inteiro para byte
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void i2b_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor inteiro para char
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void i2c_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para converter valor inteiro para short
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void i2s_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que compara valores long oriundos da pilha de operandos
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void lcmp_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que compara valores float oriundos da pilha de operandos
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fcmpl_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que compara valores float oriundos da pilha de operandos
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void fcmpg_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que compara valores double oriundos da pilha de operandos
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dcmpl_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que compara valores double oriundos da pilha de operandos
 *  *f Frame do método corrente que está executando a instrução atual
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void dcmpg_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que compara valor da pilha de operandos com zero, verificando se são iguais
 *  *f Frame do método corrente que está executando a instrução atual
 *  branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 *  branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Instrução que compara valor da pilha de operandos com zero, verificando se são diferentes
 *  *f Frame do método corrente que está executando a instrução atual
 *  branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 *  branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Instrução que compara valor da pilha de operandos com zero, verificando se o valor da pilha de operandos é menor que zero
 *  *f Frame do método corrente que está executando a instrução atual
 *  branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 *  branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void iflt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for maior ou igual ao valor zero
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for maior do que o valor zero
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for menor do que o valor zero
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifle_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for igual ao próximo inteiro na pilha
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for diferente do próximo inteiro na pilha
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmplt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmple_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se as referências na pilha forem iguais
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Branch será efetuado se as referências na pilha não forem iguais
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o índice do branch
 *  branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  A instrução sempre efetuará um branch
 *  *f Frame corrente
 *  branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 *  branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void inst_goto_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 *  Instrução para efetuar um branch para um offset, dentro do código do mesmo método sendo executado
 *  *f Frame corrente
 *  branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 *  branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 *  Instrução para retornar de uma subrotina
 *  *f Frame corrente
 *  index Índice no vetor de variáveis locais do frame corrente
 *  par1 Esse parâmetro não será utilizado na prática
 */
void ret_impl(frame *f, u1 index, u1 par1);

/**
 *  Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *  *par0 Esse parâmetro não será utilizado na prática
 *  par1 Esse parâmetro não será utilizado na prática
 *  par2 Esse parâmetro não será utilizado na prática
 */
void tableswitch_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *  *par0 Esse parâmetro não será utilizado na prática
 *  par1 Esse parâmetro não será utilizado na prática
 *  par2 Esse parâmetro não será utilizado na prática
 */
void lookupswitch_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  Instrução para acessar uma jump table por index e efetuar um jump
 *  *f Frame corrente
 */
void tableswitch_impl(frame *f);

/**
 *  Instrução para acessar uma chave em um jump table e efetuar um jump
 *  *f Frame corrente
 */
void lookupswitch_impl(frame *f);

/**
 *  Instrução para retornar um inteiro de um método
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void ireturn_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para retornar um long de um método
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void lreturn_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para retornar um float de um método
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void freturn_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para retornar um double de um método
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void dreturn_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para retornar uma referência de um método
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void areturn_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para retornar void de um método
 *  *f Frame corrente
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void inst_return_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para obter um field estático de uma classe
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução para setar um field estático em uma classe
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução para obter o field de um objeto
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução para setar um field em um objeto
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Função para invocar um método de instância, geralmente utilizada para impressão em tela
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokevirtual_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução para invocar um método de instância
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokespecial_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução para invocar um método estático (método de classe)
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokestatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *  *par0 Esse parâmetro não será utilizado na prática
 *  par1 Esse parâmetro não será utilizado na prática
 *  par2 Esse parâmetro não será utilizado na prática
 */
void invokeinterface_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  Instrução para invocar um método de interface
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 *  count Um byte que não pode ser zero
 */
void invokeinterface_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 count);

/**
 *  Instrução para invocar um método dinâmico. Essa instrução só existe para criar uma posição "falsa" no vetor de ponteiros de função
 *  *par0 Parâmetro não utilizado
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void invokedynamic_fantasma(frame *par0, u1 par1, u1 par2);

/**
 *  Instrução para criar um novo objeto
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 *  indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void inst_new_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Função para criar um novo array de um dado tipo
 *  *f Frame corrente
 *  atype Tipo do array que deve ser criado
 *  par1 Parâmetro não utilizado
 */
void newarray_impl(frame *f ,u1 atype, u1 par1);

/**
 *  Criar um novo array do tipo referência
 *  *f Frame corrente
 *  par1 Byte que será concatenado para acessar uma entrada na constant pool
 *  par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void anewarray_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para obter o tamanho de um array
 *  *f Frame corrente
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void arraylength_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para lançar uma exceção ou erro
 *  *f Frame corrente
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void athrow_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para verificar se um objeto é de um determinado tipo
 *  *f Frame corrente
 *  par1 Byte que será concatenado para acessar uma entrada na constant pool
 *  par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void checkcast_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para determinar se um objeto é de um eterminado tipo
 *  *f Frame corrente
 *  par1 Byte que será concatenado para acessar uma entrada na constant pool
 *  par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void instanceof_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para entrar no monitor de um objeto
 *  *f Frame corrente
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void monitorenter_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para sair de um monitor de um objeto
 *  *f Frame corrente
 *  par1 Parâmetro não utilizado
 *  par2 Parâmetro não utilizado
 */
void monitorexit_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para estender a próxima instrução para ter o dobro de bytes de índice. 
 *  *f Parâmetro não utilizado
 *  indexbyte1 Parâmetro não utilizado
 *  indexbyte2 Parâmetro não utilizado
 */
void wide_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 *  Instrução que irá criar um array multidimensional
 *  *f Frame corrente
 *  indexbyte1 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 *  indexbyte2 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 *  dimensions Número de dimensões do array
 */
void multianewarray_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions);

/**
 *  Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *  *f Esse parâmetro não será utilizado na prática
 *  par1 Esse parâmetro não será utilizado na prática
 *  par2 Esse parâmetro não será utilizado na prática
 */
void multianewarray_fantasma(frame *f, u1 par1, u1 par2);

/**
 *  Instrução que irá efetuar um branch caso a referência no topo da pilha seja nula
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o offset do branch
 *  branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  Instrução que irá efetuar um branch caso a referência no topo da pilha não seja nula
 *  *f Frame corrente
 *  branchbyte1 Byte que será concatenado para montar o offset do branch
 *  branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnonnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 *  A instrução sempre efetuará um branch, para um offset wide
 *  *f Frame corrente
 *  par1 Parâmetro que será concatenado para montar o índice do branch
 *  par2 Parâmetro que será concatenado para montar o índice do branch
 */
void goto_w_impl(frame *f, u1 par1, u1 par2);

/**
 *  Instrução para efetuar um branch para um offset wide, de 32 bits, dentro do código do mesmo método sendo executado
 *  *f Frame corrente
 *  par1 Parâmetro que será concatenado para montar o índice do branch
 *  par2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_w_impl(frame *f, u1 par1, u1 par2);

extern void (*func_ptr[202])(frame *,u1,u1);

#endif
