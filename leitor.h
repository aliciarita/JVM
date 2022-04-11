

/* leitor / exibidor */
#ifndef LEITURA_H
#define LEITURA_H

/** Inclusão de bibliotecas para leitura/escrita de dados,
manipualão de arquivos e definição de estruturas */
#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "classFile.h"
#include "instrucoes.h"

/**
 *  Função para ler um byte do arquivo
 fp Ponteiro do arquivo que deve ser lido
 A função retorna o u1 lido
 */
u1 u1Read(FILE *fp);

/**
 *  Função para ler dois bytes sem sinal do arquivo
 Ponteiro do arquivo que deve ser lido
 A função retorna o u2 lido
 */
u2 u2Read(FILE *fp);

/**
 *  Função para ler 4 bytes sem sinal do arquivo
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  A função retorna o u4 lido
 */
u4 u4Read(FILE *fp);

/**
 *  Função para ler um arquivo .class
 * 
 *  nomeArquivo Nome do arquivo que deve ser lido
 *  A função retorna um ponteiro de estrutura ClassFile, representando a classe que foi lida do arquivo
 * 
 * olhar ClassFile
 */
ClassFile * lerArquivo (char *nomeArquivo);

/**
 *  A função lê uma constant pool de um arquivo .class
 *
 *  fp Ponteiro do arquivo que deve ser lido
 *  constant_pool_count Variável que contém o tamanho da constant pool, para que a leitura seja delimitada
 * 
 *  A função retorna um ponteiro de cp_info, representando a constant pool lida
 * 
 * olhar cp_info
 */
cp_info * lerConstantPool (FILE * fp, u2 constant_pool_count);

/**
 *  Função que lê os métodos de uma classe
 * 
 *  fp Ponteiro do arquivo .class que deve ser lido
 *  methods_count Número de métodos que devem ser lidos
 *  cp constant_pool que deve ser passada como parâmetro para decodificação dos nomes dos métodos
 *  A função retorna a estrutura method_info, que representa a estrutura dos métodos da classe
 * 
 * olhar method_info
 */
method_info * lerMethod (FILE * fp, u2 methods_count, cp_info *cp);

/**
 *  Função para decodificar os argumentos de uma instrução
 * 
 *  cp Constant pool da classe
 *  index Índice da constant pool que deve ser decodificado
 *  sizeCP Tamanho da constant pool
 *  A função retorna um ponteiro de char, representando os argumentos decodificados
 */
char* decodificarOperandoInstrucao(cp_info *cp, u2 index, u2 sizeCP);

/**
 *  Função para decodificar o atributo Code de um método
 * 
 *  cp Constant pool da classe
 *  sizeCP Tamanho da constant pool da classe
 *  code Ponteiro apontando para o início do atributo code
 *  length Tamanho do atributo code
 *  instrucoes Vetor contendo todas as instruções
 *  A função retorna um ponteiro de char, contendo todo o Code de um método
 */
char* decodificarCode(cp_info *cp,u2 sizeCP,u1 *code, u4 length,instrucao *instrucoes);

/**
 *  Função para ler os fields do arquivo .class
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  fields_count Contagem de fields da classe.
 *  cp Constant Pool da classe
 *  A função retorna a estrutura contendo os fields lidos
 * 
 * olhar field_info
 */
field_info * lerField (FILE * fp, u2 fields_count, cp_info * cp);

/**
 *  Função para ler as interfaces de um arquivo .class
 * 
 *  fp Ponteiro do arquivo .class que deve ser lido
 *  size Número de bytes que devem ser lidos
 * 
 *  A função retorna um ponteiro para o array de interfaces indexadas pela entrada na Constant Pool que referencia a classe da interface
 */
u2 * lerInterfaces (FILE * fp, u2 size);

/**
 *  Função para ler as inner classes de uma classe
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  cp Ponteiro para a constant pool da classe sendo lida
 * 
 *  Retorna a estrutura que representa as inner classes de uma classe
 * 
 * olhar innerClasses_attribute
 */
innerClasses_attribute * lerInnerClasses (FILE * fp, cp_info * cp);

/**
 *  Função para ler o exception attribute
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  A função retorna a estrutura que representa as exceções
 * 
 * olhar exceptions_attribute
 */
exceptions_attribute * lerExceptionsAttribute (FILE * fp);

/**
 *  Função para ler atributos das inner classes da classe sendo lida
 * 
 *  fp Arquivo .class que deve ser lido
 *  A função retorna a estrutura representando os atributos lidos
 * 
 * olhar classes
 */
classes * lerClasses (FILE * fp);

/**
 *  Função para decodificar a major version de um arquivo .class
 * 
 *  major A major version lida do .class
 *  Retorna um número representando a versão
 */
double decodificaMajorVersion (u2 major);

/**
 *  Função para ler o atributo constant value de um field que apresenta modificador constante
 * 
 *  fp O arquivo .class que deve ser lido
 *  A função retorna a estrutura que representa o atributo lido
 */
constantValue_attribute * lerConstantValue (FILE * fp);

/**
 *  Função para decodificar uma entrada double na constant pool
 * 
 *  cp Entrada da constant pool apontando para o double
 *  Número double decodificado
 */
double decodificaDoubleInfo (cp_info * cp);

/**
 *  Função para decodificar uma entrada long na constant pool
 * 
 *  cp Entrada da constant pool apontando para o long
 *  Número long decodificado
 */
long long decodificaLongInfo (cp_info * cp);

/**
 *  [brief description]
 * @details [long description]
 * 
 *  cp [description]
 *  [description]
 */
float decodificaFloatInfo (cp_info * cp);

/**
 *  Função para decodificar uma entrada Integer na constant pool
 * 
 *  cp Entrada na constant pool apontando para uma entrada do tipo Integer
 *  A função retorna o inteiro decodificado
 */
int decodificaIntegerInfo (cp_info * cp);

/**
 *  Função para ler o atributo signature do arquivo .class sendo lido
 * 
 *  fp Ponteiro de arquivo que deve ser lido
 *  A função retorna a estrutura representando o atributo signature lido
 * 
 * olhar signature_attribute
*/
signature_attribute * lerSignature (FILE * fp);

/**
 *  Função para colocar as flags de acesso na ordem correta
 * 
 *  flagsOrdemInversa Ponteiro de char com as flags na ordem inversa
 *  Retorna um ponteiro de char com a string na ordem correta
 */
char* organizandoFlags(char* flagsOrdemInversa);

/**
 *  Função para ler o atributo stack map table dentro de Code
 * 
 *  fp Arquivo que deve ser lido
 *  Retorna a estrutura que representa o atributo stack map table
 * 
 * olhar stackMapTable_attribute
 */
stackMapTable_attribute * lerStackMapTable (FILE * fp);

/**
 *  Função para ler o atributo stack map frame
 * 
 *  fp Ponteiro de arquivo que deve ser lido
 *  Retorna a estrutura representando o atributo stack map frame
 * 
 * olhar stack_map_frame
 */
stack_map_frame * lerStackMapFrame (FILE * fp);

/**
 *  Função para ajustar um offset do atributo interno da stack map frame
 * 
 *  posicao Posição do atributo interno ao stack frame
 *  offset Valor de ajuste presente no atributo do stack frame a ser avaliado para correta impressao
 * 
 *  A função retorna a posição correta de impressão do atributo
 */
int setaOffsetImpressao (int posicao, u1 offset);

/**
 *  Função para ler o atributo verification_type_info
 * 
 *  fp Arquivo que deve ser lido
 *  Retorna a estrutura representando o atributo verification_type_info lido
 * 
 * olhar verification_type_info
 */
verification_type_info * lerVerificationTypeInfo (FILE * fp);

/**
 *  Função para ler o atributo Code de um método
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  cp Constant Pool do arquivo lido
 * 
 *  A função retorna a estrutura representando o atributo code lido
 * 
 * olhar code_attribute
 */
code_attribute * lerCode (FILE * fp, cp_info *cp);

/**
 *  Função para ler o atributo Line Number Table de um atributo Code de um método
 * 
 *  fp Arquivo que deve ser lido
 *  cp Ponteiro para a constant pool do arquivo lido
 * 
 *  A função retorna a estrutura representando o atributo Line Number Table lido
 * 
 * olhar line_number_table
 */
line_number_table * lerLineNumberTable(FILE * fp, cp_info *cp);

/**
 *  Função para ler a exception table de Code
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  size Tamanho da exception table que deve ser lida
 * 
 *  A função retorna a estrutura representando a tabela de exceções do Code
 * 
 * olhar exception_table
 */
exception_table * lerExceptionTable (FILE * fp, u2 size);

/**
 *  Função para ler a estrutura attribute info de um arquivo
 * 
 *  fp Ponteiro de arquivo que deve ser lido
 *  cp Ponteiro para a constant pool do arquivo lido
 * 
 *  A função retorna a estrutura representando o attribute info lido
 * 
 * olhar attribute_info
 */
attribute_info * lerAttributes (FILE * fp, cp_info *cp);

/**
 *  Função para ler o atributo source file
 * 
 *  fp Ponteiro do arquivo que deve ser lido
 *  A função retorna a estrutura que representa o atributo source file
 * 
 * olhar source_file_attribute
 */
source_file_attribute * lerSourceFile (FILE * fp);

/**
 *  Função para retornar a string representando uma tag
 * 
 *  tag A tag que deve ser convertida para string
 *  A função retorna a string equivalente à tag enviada como parâmetro
 */
char* buscaNomeTag(u1 tag);

/**
 *  Função que retorna a string UTF8 da entrada da constant pool recebida como parâmetro
 * 
 *  cp Entrada da constant pool que aponta para uma estrutura do tipo UTF8
 *  A função retorna a string UTF8 decodificada
 */
char* decodificaStringUTF8(cp_info *cp);

/**
 *  Função para decodificar uma entrada da Constant Pool
 * 
 *  cp Ponteiro para a constant pool de uma classe
 *  index Índice da constant pool que deve ser decodificado
 *  tipo Tipo da entrada que deve ser decodificada
 *  A função retorna a string, que é a entrada decodificada
 */
char* decodificaNIeNT(cp_info *cp, u2 index,u1 tipo);

/**
 *  Função para decodificar o access flag enviado como parâmetro
 * 
 *  flag Inteiro representando as flags que devem ser decodificadas
 *  A função retorna a string que representa os access flags decodificados
 */
char* decodificaAccessFlags(u2 flag);

/**
 *  Função para imprimir um arquivo .class lido
 * 
 *  arquivoClass Estrutura de um arquivo .class que já foi lido
 *  fp Ponteiro de arquivo em que o arquivo .class deve ser impresso
 * 
 * olhar ClassFile
 */
void imprimirClassFile (ClassFile *arquivoClass, FILE *fp);

#endif
