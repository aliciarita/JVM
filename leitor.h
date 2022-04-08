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
 * @brief Função para ler um byte do arquivo
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @return A função retorna o u1 lido
 */
u1 u1Read(FILE *fp);

/**
 * @brief Função para ler dois bytes sem sinal do arquivo
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @return A função retorna o u2 lido
 */
u2 u2Read(FILE *fp);

/**
 * @brief Função para ler 4 bytes sem sinal do arquivo
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @return A função retorna o u4 lido
 */
u4 u4Read(FILE *fp);

/**
 * @brief Função para ler um arquivo .class
 * 
 * @param nomeArquivo Nome do arquivo que deve ser lido
 * @return A função retorna um ponteiro de estrutura ClassFile, representando a classe que foi lida do arquivo
 * 
 * @see ClassFile
 */
ClassFile * lerArquivo (char *nomeArquivo);

/**
 * @brief A função lê uma constant pool de um arquivo .class
 *
 * @param fp Ponteiro do arquivo que deve ser lido
 * @param constant_pool_count Variável que contém o tamanho da constant pool, para que a leitura seja delimitada
 * 
 * @return A função retorna um ponteiro de cp_info, representando a constant pool lida
 * 
 * @see cp_info
 */
cp_info * lerConstantPool (FILE * fp, u2 constant_pool_count);

/**
 * @brief Função que lê os métodos de uma classe
 * 
 * @param fp Ponteiro do arquivo .class que deve ser lido
 * @param methods_count Número de métodos que devem ser lidos
 * @param cp constant_pool que deve ser passada como parâmetro para decodificação dos nomes dos métodos
 * @return A função retorna a estrutura method_info, que representa a estrutura dos métodos da classe
 * 
 * @see method_info
 */
method_info * lerMethod (FILE * fp, u2 methods_count, cp_info *cp);

/**
 * @brief Função para decodificar os argumentos de uma instrução
 * 
 * @param cp Constant pool da classe
 * @param index Índice da constant pool que deve ser decodificado
 * @param sizeCP Tamanho da constant pool
 * @return A função retorna um ponteiro de char, representando os argumentos decodificados
 */
char* decodificarOperandoInstrucao(cp_info *cp, u2 index, u2 sizeCP);

/**
 * @brief Função para decodificar o atributo Code de um método
 * 
 * @param cp Constant pool da classe
 * @param sizeCP Tamanho da constant pool da classe
 * @param code Ponteiro apontando para o início do atributo code
 * @param length Tamanho do atributo code
 * @param instrucoes Vetor contendo todas as instruções
 * @return A função retorna um ponteiro de char, contendo todo o Code de um método
 */
char* decodificarCode(cp_info *cp,u2 sizeCP,u1 *code, u4 length,instrucao *instrucoes);

/**
 * @brief Função para ler os fields do arquivo .class
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @param fields_count Contagem de fields da classe.
 * @param cp Constant Pool da classe
 * @return A função retorna a estrutura contendo os fields lidos
 * 
 * @see field_info
 */
field_info * lerField (FILE * fp, u2 fields_count, cp_info * cp);

/**
 * @brief Função para ler as interfaces de um arquivo .class
 * 
 * @param fp Ponteiro do arquivo .class que deve ser lido
 * @param size Número de bytes que devem ser lidos
 * 
 * @return A função retorna um ponteiro para o array de interfaces indexadas pela entrada na Constant Pool que referencia a classe da interface
 */
u2 * lerInterfaces (FILE * fp, u2 size);

/**
 * @brief Função para ler as inner classes de uma classe
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @param cp Ponteiro para a constant pool da classe sendo lida
 * 
 * @return Retorna a estrutura que representa as inner classes de uma classe
 * 
 * @see innerClasses_attribute
 */
innerClasses_attribute * lerInnerClasses (FILE * fp, cp_info * cp);

/**
 * @brief Função para ler o exception attribute
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @return A função retorna a estrutura que representa as exceções
 * 
 * @see exceptions_attribute
 */
exceptions_attribute * lerExceptionsAttribute (FILE * fp);

/**
 * @brief Função para ler atributos das inner classes da classe sendo lida
 * 
 * @param fp Arquivo .class que deve ser lido
 * @return A função retorna a estrutura representando os atributos lidos
 * 
 * @see classes
 */
classes * lerClasses (FILE * fp);

/**
 * @brief Função para decodificar a major version de um arquivo .class
 * 
 * @param major A major version lida do .class
 * @return Retorna um número representando a versão
 */
double decodificaMajorVersion (u2 major);

/**
 * @brief Função para ler o atributo constant value de um field que apresenta modificador constante
 * 
 * @param fp O arquivo .class que deve ser lido
 * @return A função retorna a estrutura que representa o atributo lido
 */
constantValue_attribute * lerConstantValue (FILE * fp);

/**
 * @brief Função para decodificar uma entrada double na constant pool
 * 
 * @param cp Entrada da constant pool apontando para o double
 * @return Número double decodificado
 */
double decodificaDoubleInfo (cp_info * cp);

/**
 * @brief Função para decodificar uma entrada long na constant pool
 * 
 * @param cp Entrada da constant pool apontando para o long
 * @return Número long decodificado
 */
long long decodificaLongInfo (cp_info * cp);

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param cp [description]
 * @return [description]
 */
float decodificaFloatInfo (cp_info * cp);

/**
 * @brief Função para decodificar uma entrada Integer na constant pool
 * 
 * @param cp Entrada na constant pool apontando para uma entrada do tipo Integer
 * @return A função retorna o inteiro decodificado
 */
int decodificaIntegerInfo (cp_info * cp);

/**
 * @brief Função para ler o atributo signature do arquivo .class sendo lido
 * 
 * @param fp Ponteiro de arquivo que deve ser lido
 * @return A função retorna a estrutura representando o atributo signature lido
 * 
 * @see signature_attribute
*/
signature_attribute * lerSignature (FILE * fp);

/**
 * @brief Função para colocar as flags de acesso na ordem correta
 * 
 * @param flagsOrdemInversa Ponteiro de char com as flags na ordem inversa
 * @return Retorna um ponteiro de char com a string na ordem correta
 */
char* organizandoFlags(char* flagsOrdemInversa);

/**
 * @brief Função para ler o atributo stack map table dentro de Code
 * 
 * @param fp Arquivo que deve ser lido
 * @return Retorna a estrutura que representa o atributo stack map table
 * 
 * @see stackMapTable_attribute
 */
stackMapTable_attribute * lerStackMapTable (FILE * fp);

/**
 * @brief Função para ler o atributo stack map frame
 * 
 * @param fp Ponteiro de arquivo que deve ser lido
 * @return Retorna a estrutura representando o atributo stack map frame
 * 
 * @see stack_map_frame
 */
stack_map_frame * lerStackMapFrame (FILE * fp);

/**
 * @brief Função para ajustar um offset do atributo interno da stack map frame
 * 
 * @param posicao Posição do atributo interno ao stack frame
 * @param offset Valor de ajuste presente no atributo do stack frame a ser avaliado para correta impressao
 * 
 * @return A função retorna a posição correta de impressão do atributo
 */
int setaOffsetImpressao (int posicao, u1 offset);

/**
 * @brief Função para ler o atributo verification_type_info
 * 
 * @param fp Arquivo que deve ser lido
 * @return Retorna a estrutura representando o atributo verification_type_info lido
 * 
 * @see verification_type_info
 */
verification_type_info * lerVerificationTypeInfo (FILE * fp);

/**
 * @brief Função para ler o atributo Code de um método
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @param cp Constant Pool do arquivo lido
 * 
 * @return A função retorna a estrutura representando o atributo code lido
 * 
 * @see code_attribute
 */
code_attribute * lerCode (FILE * fp, cp_info *cp);

/**
 * @brief Função para ler o atributo Line Number Table de um atributo Code de um método
 * 
 * @param fp Arquivo que deve ser lido
 * @param cp Ponteiro para a constant pool do arquivo lido
 * 
 * @return A função retorna a estrutura representando o atributo Line Number Table lido
 * 
 * @see line_number_table
 */
line_number_table * lerLineNumberTable(FILE * fp, cp_info *cp);

/**
 * @brief Função para ler a exception table de Code
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @param size Tamanho da exception table que deve ser lida
 * 
 * @return A função retorna a estrutura representando a tabela de exceções do Code
 * 
 * @see exception_table
 */
exception_table * lerExceptionTable (FILE * fp, u2 size);

/**
 * @brief Função para ler a estrutura attribute info de um arquivo
 * 
 * @param fp Ponteiro de arquivo que deve ser lido
 * @param cp Ponteiro para a constant pool do arquivo lido
 * 
 * @return A função retorna a estrutura representando o attribute info lido
 * 
 * @see attribute_info
 */
attribute_info * lerAttributes (FILE * fp, cp_info *cp);

/**
 * @brief Função para ler o atributo source file
 * 
 * @param fp Ponteiro do arquivo que deve ser lido
 * @return A função retorna a estrutura que representa o atributo source file
 * 
 * @see source_file_attribute
 */
source_file_attribute * lerSourceFile (FILE * fp);

/**
 * @brief Função para retornar a string representando uma tag
 * 
 * @param tag A tag que deve ser convertida para string
 * @return A função retorna a string equivalente à tag enviada como parâmetro
 */
char* buscaNomeTag(u1 tag);

/**
 * @brief Função que retorna a string UTF8 da entrada da constant pool recebida como parâmetro
 * 
 * @param cp Entrada da constant pool que aponta para uma estrutura do tipo UTF8
 * @return A função retorna a string UTF8 decodificada
 */
char* decodificaStringUTF8(cp_info *cp);

/**
 * @brief Função para decodificar uma entrada da Constant Pool
 * 
 * @param cp Ponteiro para a constant pool de uma classe
 * @param index Índice da constant pool que deve ser decodificado
 * @param tipo Tipo da entrada que deve ser decodificada
 * @return A função retorna a string, que é a entrada decodificada
 */
char* decodificaNIeNT(cp_info *cp, u2 index,u1 tipo);

/**
 * @brief Função para decodificar o access flag enviado como parâmetro
 * 
 * @param flag Inteiro representando as flags que devem ser decodificadas
 * @return A função retorna a string que representa os access flags decodificados
 */
char* decodificaAccessFlags(u2 flag);

/**
 * @brief Função para imprimir um arquivo .class lido
 * 
 * @param arquivoClass Estrutura de um arquivo .class que já foi lido
 * @param fp Ponteiro de arquivo em que o arquivo .class deve ser impresso
 * 
 * @see ClassFile
 */
void imprimirClassFile (ClassFile *arquivoClass, FILE *fp);

#endif
