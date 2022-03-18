#ifndef LEITOR_H
#define LEITOR_H

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "classFile.h"
#include "instrucoes.h"

//Assinatura dos métodos

u1 u1Read(FILE *fp);
u2 u2Read(FILE *fp);
u4 u4Read(FILE *fp);

ClassFile * readFile (char *);
cp_info * readConstantPool (FILE * fp, u2 constantpoolcount);
method_info * readMethod (FILE * fp, u2 methodscount, cp_info *cp);
char* decodificarOperandoInstrucao(cp_info *cp, u2 index, u2 sizeCP);
char* decodificarCode(cp_info *cp,u2 sizeCP,u1 *code, u4 length,instrucao *instrucoes);
field_info * readField (FILE * fp, u2 fields_count, cp_info * cp);
u2 * readInterfaces (FILE * fp, u2 size);
innerClasses_attribute * readInnerClasses (FILE * fp, cp_info * cp);
exceptions_attribute * readExceptionsAttribute (FILE * fp);
classes * readClasses (FILE * fp);
constantValue_attribute * readConstantValue (FILE * fp);
double decodificaDoubleInfo (cp_info * cp);
uint64_t decodificaLongInfo (cp_info * cp);
float decodificaFloatInfo (cp_info * cp);
int decodificaIntegerInfo (cp_info * cp);
signature_attribute * readSignature (FILE * fp);
char* organizandoFlags(char* flagsOrdemInversa);
stackMapTable_attribute * readStackMapTable (FILE * fp);
stack_map_frame * readStackMapFrame (FILE * fp);
int setaOffsetImpressao (int posicao, u1 offset);
verification_type_info * readVerificationTypeInfo (FILE * fp);
code_attribute * readCode (FILE * fp, cp_info *cp);
line_number_table * readLineNumberTable(FILE * fp, cp_info *cp);
exception_table * readExceptionTable (FILE * fp, u2 size);
attribute_info * readAttributes (FILE * fp, cp_info *cp);
source_file_attribute * readSourceFile (FILE * fp);
char* buscaNomeTag(u1 tag);
char* decodificaStringUTF8(cp_info *cp);
char* decodificaNIeNT(cp_info *cp, u2 index,u1 tipo);
char* decodificaAccessFlags(u2 flag);
void imprimirClassFile (ClassFile *, FILE *);

#endif
