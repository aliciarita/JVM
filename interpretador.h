/**
 * @file interpretador.h
 * @brief Arquivo cabecalho das instrucoes e funcoes auxiliares
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
 * @brief Funcao para resolver (analisar e alocar) uma classe
 * @param *nome_classe Nome da classe para ser resolvida
 * @return Ponteiro para classe resolvida
 */
ClassFile* resolverClasse(char *nome_classe);

/**
 * @brief Funcao para resolver (analisar e alocar) um metodo
 * @param *cp Ponteiro para a pool de constantes
 * @param indice_cp Indice para acesso na pool de constantes
 * @param interface Flag para sinalizar metodo de interface
 * @return Booleano representando o sucesso ou fracasso da execucao da resolucao
 */
bool resolverMetodo(cp_info *cp, u2 indice_cp, u1 interface);

/**
 * @brief Funcao para obter o nome de um metodo na pool de constantes
 * @param *cp Ponteiro para a pool de constante
 * @param indice_cp Indice de acesso na pool de constante
 * @param interface Flag para sinalizar metodo de interface
 * @return Ponteiro de char representando o nome do metodo
 */
char* obterNomeMetodo(cp_info *cp, u2 indice_cp, u1 interface);

/**
 * @brief Funcao para obter o descriptor de um metodo
 * @param *cp Ponteiro para a pool de constante
 * @param indice_cp Indice de acesso na pool de constante
 * @param interface Flag para sinalizar metodo de interface
 * @return Ponteiro de char representando o descriptor do metodo
 */
char* obterDescriptorMetodo(cp_info *cp, u2 indice_cp, u1 interface);

/**
 * @brief Funcao para obter o tipo do descriptor do metodo
 * @param *descriptor Ponteiro de char representando o descriptor do metodo
 * @return Valor inteiro representando o tipo do descriptor do metodo
 */
int descriptorFieldValidate (char * descriptor);

/**
 * @brief Funcao para obter a classe executante de um metodo
 * @param *cp Ponteiro para pool de constante
 * @param indice_cp Indice de acesso na pool de constante
 */
char* obterClasseDoMetodo(cp_info *cp, u2 indice_cp);

/**
 * @brief Funcao para transferir os valores da Pilha de Operandos de um frame para o vetor de variaveis locais do proximo frame
 * @param *anterior Ponteiro para o frame previamente executado
 * @param *novo Ponteiro para o novo frame que sera locado para a execucao do metodo
 * @param *parametros_cont Ponteiro para o contador de parametros a serem passados para o array de variaveis locais do novo frame
 * @return Retorna um ponteiro para o novo frame alocado
 */
frame* transferePilhaVetor(frame *anterior, frame *novo, int *parametros_cont);

/**
 * @brief Funcao para decodificar operandos em um valor double (64 bits)
 * @param high Parte alta do valor double
 * @param low Parte baixa do valor double
 * return Valor double decodificado
 */
double decodificaDoubleValor(u4 high, u4 low);

/**
 * @brief Funcao para decodificar operando em valor float (32 bits)
 * @param valor 4 bytes do tipo unsigned representando o valor float
 * @return Valor float decodificado
 */
float decodificaFloatValor(u4 valor);

/**
 * @brief Funcao para decodificar operando em valor long (64 bits)
 * @param high Parte alta do valor long
 * @param low Parte baixa do valor long
 * @return Valor long decodificado
 */
long decodificaLongValor (u4 high, u4 low);

/**
 * @brief Funcao para obter a quantidade de campos da classe que nao sao estaticos
 * @param *classe Ponteiro para a classe alocada
 * @return Valor inteiro com a quantidade de campos que nao sao do tipo static
 */
int getParametrosNaoStatic (ClassFile * classe);

/**
 * @brief Funcao para inserir um objeto novo na lista de objetos
 * @param *lis Ponteiro para o comeco da lista de objetos
 * @param *classe Referencia para a classe a ser alocada na lista
 * @param parametrosNaoStatic Quantidade de paramentros que nao sao static
 * @return Ponteiro para a lista de objetos com um novo objeto inserido
 */
Lista_Objetos * InsereObjeto (Lista_Objetos * lis, ClassFile * classe, int parametrosNaoStatic);

/**
 * @brief Funcao para verificar se um metodo é do tipo static
 * @param *acessFlags Array de char ccontendo o acessflags do metodo
 * @return Booleano indicando se o metodo é do tipo static ou nao
 */
bool buscaStaticFlags (char * accessFlags);

/**
 * @brief Função para obter a quantidade de parâmetros a serem passados para um método
 * @param *descriptor Array de char contendo o descriptor do método
 * @return Valor inteiro representando a quantidade de parâmetros a serem passados para o método
 */
int getParametrosCount (char * descriptor);

/**
 * @brief Função para buscar um objeto na lista de objetos
 * @param *p Ponteiro de classe contendo a referência do objeto
 * @return Retorna um ponteiro para a lista de objetos
 */
Lista_Objetos * buscaObjetoViaReferencia (ClassFile * p);

/**
 * @brief Função para obter a posição do field no array de dados de instância
 * @param *obj Ponteiro para o objeto alocado
 * @param *nomeField Nome do field pertencente ao objeto
 * @return Posição do field no array de dados não estáticos
 */
int getPositionField (ClassFile * obj, char * nomeField);

/**
 * @brief Função para instanciar um novo objeto
 * @param *nomeClasse Array de char contendo o nome da classe
 * @return Ponteiro para o objeto alocado
 */
ClassFile * instanciarClasse (char * nomeClasse);

/**
 * @brief Função para obter o tipo de operando que sera alocado na pilha de operandos
 * @param *descriptorRetorno Array de char contendo o descriptor do método
 * @return Valor inteiro representando o tipo do operando que sera alocado na pilha de operandos
 */
int getTipoOperandoSaida(char * descriptorRetorno);

/**
 * @brief Função para obter o modo de empilhamento do valor de retorno de um método
 * 
 * @param descriptor Ponteiro de char contendo a string que representa o descriptor do método
 * @return Valor inteiro representando o modo de empilhamento do valor de retorno de um método na pilha de operandos
 */
int getTipoRetorno (char * descriptor);

/**
 * @brief Não executa nada
 * @param *par0 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void nop_impl(frame *par0,u1 par1, u1 par2);

/**
 * @brief Faz o push de um valor nulo para a pilha de operandos 
 * @param f Ponteiro para o frame]
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void aconst_null_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief [brief description]
 * @param *f [description]
 * @param par1 [description]
 * @param par2 [description]
 */
void iconst_m1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 (int)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 (int)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 2 (int)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 3 (int) 
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iconst_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 4 (int)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iconst_4_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 5 (int)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iconst_5_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 (long)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 (long)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 (float)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 (float)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 2 (float)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 (double)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 (double)
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push de um short na pilha de operandos
 * @param *f Ponteiro para o frame
 * @param byte Primeiro byte do short
 * @param par1 Segundo byte do short
 */
void sipush_impl(frame *f,u1 byte1, u1 byte2);

/**
 * @brief Faz o push de um valor presente na constant pool
 * @param *f Ponteiro para o frame
 * @param indexbyte1 Índice da constant pool
 * @param par2 Parâmetro não utilizado
 */
void ldc_impl(frame *f,u1 indexbyte1,u1 par2);

/**
 * @brief Faz o push de um valor presente na constant pool
 * @param *f Ponteiro para o frame
 * @param indexbyte1 Índice da constant pool (primeira parte)
 * @param indexbyte2 Índice da constant pool (segunda parte)
 */
void ldc_w_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Faz o push de um valor long ou double presente na constant pool
 * @param *f Ponteiro para o frame
 * @param branchbyte1 Índice da constant pool (primeira parte)
 * @param branchbyte2 Índice da constant pool (segunda parte)
 */
void ldc2_w_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Carrega um valor int do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 Parâmetro não utilizado
 */
void iload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor long do vetor de variáveis locais 
 * @param *f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 Parâmetro não utilizado
 */
void lload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor float do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 Parâmetro não utilizado
 */
void fload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor double do vetor de variáveis locais 
 * @param *f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 Parâmetro não utilizado
 */
void dload_impl(frame *par0, u1 par1,u1 par2);

/**
 * @brief Carrega uma referência do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 Parâmetro não utilizado
 */
void aload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor int da posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void iload_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor int da posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void iload_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor int da posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void iload_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor int da posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void iload_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor long da posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void lload_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor long da posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void lload_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor long da posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void lload_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor long da posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void lload_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor float da posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void fload_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor float da posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void fload_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor float da posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void fload_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor float da posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void fload_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor double da posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void dload_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor double da posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void dload_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor double da posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void dload_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um valor double da posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void dload_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega uma referência da posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void aload_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega uma referência da posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void aload_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega uma referência da posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void aload_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega uma referência da posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par2 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 */
void aload_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um int de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iaload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um long de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void laload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um float de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void faload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um double de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void daload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega uma referência de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void aaload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um byte de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void baload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um char de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void caload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Carrega um short de um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void saload_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um int no vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param index Índice do vetor de variáveis locais
 */
void istore_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Armazena um long no vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param index Índice do vetor de variáveis locais
 */
void lstore_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Armazena um float no vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param index Índice do vetor de variáveis locais
 */
void fstore_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Armazena um double no vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param index Índice do vetor de variáveis locais
 */
void dstore_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Armazena uma referência no vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param index Índice do vetor de variáveis locais
 */
void astore_impl(frame *f,u1 index, u1 par1);

/**
 * @brief Armazena um int na posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void istore_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um int na posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void istore_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um int na posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void istore_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um int na posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void istore_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um long na posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lstore_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um long na posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lstore_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um long na posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lstore_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um long na posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lstore_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um float na posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fstore_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um float na posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fstore_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um float na posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fstore_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um float na posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fstore_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um double na posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dstore_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um double na posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dstore_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um double na posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dstore_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um double na posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dstore_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena uma referência na posição 0 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void astore_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena uma referência na posição 1 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void astore_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena uma referência na posição 2 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void astore_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena uma referência na posição 3 do vetor de variáveis locais
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void astore_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor int em um array
 * 
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor long em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor float em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor double em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena uma referência em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void aastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor byte em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void bastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor char em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void castore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Armazena um valor short em um array
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void sastore_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Remove o valor de topo da pilha de operandos
 * @param *f Ponteiro para o frame
 */
pilha_operandos* pop_impl(frame *f);

/**
 * @brief Remove os dois valores de topo da pilha de operandos
 * @param *f Ponteiro para o frame
 */
pilha_operandos** pop2_impl(frame *f);

/**
 * @brief PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop
 * @param *par0 frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void pop_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop2
 * @param *par0 frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void pop2_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief Duplica o valor no topo da pilha de operandos.
 * @param *f frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dup_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois valores a baixo.
 * @param *f frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dup_x1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 * @param *f frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dup_x2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere logo abaixo.
 * @param *f frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dup2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois valores a baixo.
 * @param *f frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dup2_x1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 * @param *f frame.
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dup2_x2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Inverte os dois operandos do topo da pilha
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void swap_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma soma de int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iadd_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma soma de long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void ladd_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma soma de float
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fadd_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma soma de double
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dadd_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma subtração de int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void isub_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma subtração de long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lsub_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma subtração de float
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fsub_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma subtração de double
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dsub_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma multiplicação de int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void imul_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma multiplicação de long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lmul_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma multiplicação de float
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fmul_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma multiplicação de double
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dmul_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma divisão de int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void idiv_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma divisão de long
 * @param ]*f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void ldiv_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma divisão de float
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fdiv_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa uma divisão de double
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void ddiv_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Calcula o resto int da divisão entre dois operandos
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void irem_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Calcula o resto long da divisão entre dois operandos
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lrem_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Calcula o resto float da divisão entre dois operandos
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void frem_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Calcula o resto double da divisão entre dois operandos
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void drem_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Inverte o sinal de um int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void ineg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Inverte o sinal de um long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void lneg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Inverte o sinal de um float
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void fneg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Inverte o sinal de um double
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void dneg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa um shift left em um int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void ishl_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa um shift left em um long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void lshl_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa um shift right em um int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void ishr_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa um shift right em um long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void lshr_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa um shift right lógico em um int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void iushr_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa um shift right lógico em um long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado		
 * @param par2 Parâmetro não utilizado
 */
void lushr_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa a operação "AND" de dois int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void iand_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa a operação "AND" de dois long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void land_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa a operação "OR" de dois int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void ior_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa a operação "OR" de dois long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lor_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa a operação "XOR" de dois int
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void ixor_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Executa a operação "XOR" de dois long
 * @param *f Ponteiro para o frame
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lxor_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Função para mapear a execução da instrução iinc
 * @param *par0 Frame do método corrente que está executando a instrução atual
 * @param par1 Byte para indexar o acesso no vetor de variáveis locais
 * @param par2 Valor inteiro contendo o incremento a ser realizado
 */
void iinc_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para incrementar um valor no vetor de variáveis locais do frame
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param index Byte para indexar o acesso no vetor de variáveis locais
 * @param ante Valor inteiro contendo o incremento a ser realizado
 */
void iinc_impl(frame *f, u1 index, i1 constante);

/**
 * @brief Função para mapear a execução da instrução iinc_wide
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param indexbyte1 Byte high para indexar o acesso no vetor de variáveis locais
 * @param indexbyte2 Byte low para indexar o acesso no vetor de variáveis locais
 * @param byte1 Byte high a ser utilizado no incremento da variável no vetor de variáveis locais
 * @param byte2 Byte low a ser utilizado no incremento da variável no vetor de variáveis locais
 */
void iinc_wide_fantasma(frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2);

/**
 * @brief Instrução para incrementar um valor no vetor de variáveis locais do frame
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param indexbyte Byte duplo para indexar o acesso no vetor de variáveis locais
 * @param byte Byte duplo contendo o incremento no vetor de variáveis locais
 */
void iinc_wide(frame *f, u2 indexbyte, i2 constbyte);

/**
 * @brief Instrução para converter valor inteiro para long
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void i2l_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor inteiro para float
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void i2f_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor inteiro para double
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void i2d_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor long para inteiro
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void l2i_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor long para float
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void l2f_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor long para double
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void l2d_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor float para inteiro
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void f2i_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor float para long
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void f2l_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor float para double
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void f2d_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor double para inteiro
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void d2i_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor double para long
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void d2l_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor double para float
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void d2f_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor inteiro para byte
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void i2b_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor inteiro para char
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void i2c_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para converter valor inteiro para short
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void i2s_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores long oriundos da pilha de operandos
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lcmp_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores float oriundos da pilha de operandos
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fcmpl_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores float oriundos da pilha de operandos
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fcmpg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores double oriundos da pilha de operandos
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dcmpl_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores double oriundos da pilha de operandos
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dcmpg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se são iguais
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se são diferentes
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se o valor da pilha de operandos é menor que zero
 * @param *f Frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void iflt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior ou igual ao valor zero
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o valor zero
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o valor zero
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifle_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for igual ao próximo inteiro na pilha
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for diferente do próximo inteiro na pilha
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmplt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmple_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se as referências na pilha forem iguais
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se as referências na pilha não forem iguais
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief A instrução sempre efetuará um branch
 * @param *f Frame corrente
 * @param branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 * @param branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void inst_goto_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução para efetuar um branch para um offset, dentro do código do mesmo método sendo executado
 * @param *f Frame corrente
 * @param branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 * @param branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução para retornar de uma subrotina
 * @param *f Frame corrente
 * @param index Índice no vetor de variáveis locais do frame corrente
 * @param par1 Esse parâmetro não será utilizado na prática
 */
void ret_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 * @param *par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void tableswitch_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 * @param *par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void lookupswitch_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para acessar uma jump table por index e efetuar um jump
 * @param *f Frame corrente
 */
void tableswitch_impl(frame *f);

/**
 * @brief Instrução para acessar uma chave em um jump table e efetuar um jump
 * @param *f Frame corrente
 */
void lookupswitch_impl(frame *f);

/**
 * @brief Instrução para retornar um inteiro de um método
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void ireturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um long de um método
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void lreturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um float de um método
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void freturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um double de um método
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void dreturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar uma referência de um método
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void areturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar void de um método
 * @param *f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void inst_return_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para obter um field estático de uma classe
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para setar um field estático em uma classe
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para obter o field de um objeto
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para setar um field em um objeto
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Função para invocar um método de instância, geralmente utilizada para impressão em tela
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokevirtual_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para invocar um método de instância
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokespecial_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para invocar um método estático (método de classe)
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokestatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 * @param *par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void invokeinterface_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para invocar um método de interface
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 * @param count Um byte que não pode ser zero
 */
void invokeinterface_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 count);

/**
 * @brief Instrução para invocar um método dinâmico. Essa instrução só existe para criar uma posição "falsa" no vetor de ponteiros de função
 * @param *par0 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void invokedynamic_fantasma(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para criar um novo objeto
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void inst_new_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Função para criar um novo array de um dado tipo
 * @param *f Frame corrente
 * @param atype Tipo do array que deve ser criado
 * @param par1 Parâmetro não utilizado
 */
void newarray_impl(frame *f ,u1 atype, u1 par1);

/**
 * @brief Criar um novo array do tipo referência
 * @param *f Frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void anewarray_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para obter o tamanho de um array
 * @param *f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void arraylength_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para lançar uma exceção ou erro
 * @param *f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void athrow_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para verificar se um objeto é de um determinado tipo
 * @param *f Frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void checkcast_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para determinar se um objeto é de um eterminado tipo
 * @param *f Frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void instanceof_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para entrar no monitor de um objeto
 * @param *f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void monitorenter_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para sair de um monitor de um objeto
 * @param *f Frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void monitorexit_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para estender a próxima instrução para ter o dobro de bytes de índice. 
 * @param *f Parâmetro não utilizado
 * @param indexbyte1 Parâmetro não utilizado
 * @param indexbyte2 Parâmetro não utilizado
 */
void wide_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução que irá criar um array multidimensional
 * @param *f Frame corrente
 * @param indexbyte1 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 * @param indexbyte2 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 * @param dimensions Número de dimensões do array
 */
void multianewarray_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 * @param *f Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void multianewarray_fantasma(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que irá efetuar um branch caso a referência no topo da pilha seja nula
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o offset do branch
 * @param branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que irá efetuar um branch caso a referência no topo da pilha não seja nula
 * @param *f Frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o offset do branch
 * @param branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnonnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief A instrução sempre efetuará um branch, para um offset wide
 * @param *f Frame corrente
 * @param par1 Parâmetro que será concatenado para montar o índice do branch
 * @param par2 Parâmetro que será concatenado para montar o índice do branch
 */
void goto_w_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para efetuar um branch para um offset wide, de 32 bits, dentro do código do mesmo método sendo executado
 * @param *f Frame corrente
 * @param par1 Parâmetro que será concatenado para montar o índice do branch
 * @param par2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_w_impl(frame *f, u1 par1, u1 par2);

extern void (*func_ptr[202])(frame *,u1,u1);

#endif
