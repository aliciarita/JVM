#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "instrucoes.h"
#include "classFile.h"
#include "jvm.h"
#include <stdbool.h>

#define normaliza_indice(x,y) (x << 8) | y

ClassFile* resolverClasse(char *nome_classe);

bool resolverMetodo(cp_info *cp, u2 indice_cp, u1 interface);

char* obterNomeMetodo(cp_info *cp, u2 indice_cp, u1 interface);

char* obterDescriptorMetodo(cp_info *cp, u2 indice_cp, u1 interface);

int descriptorFieldValidate (char * descriptor);

char* obterClasseDoMetodo(cp_info *cp, u2 indice_cp);

frame* transferePilhaVetor(frame *anterior, frame *novo, int *parametros_cont);

double decodificaDoubleValor(u4 high, u4 low);

float decodificaFloatValor(u4 valor);

long decodificaLongValor (u4 high, u4 low);

int getParametrosNaoStatic (ClassFile * classe);

Lista_Objetos * InsereObjeto (Lista_Objetos * lis, ClassFile * classe, int parametrosNaoStatic);

bool buscaStaticFlags (char * accessFlags);

int getParametrosCount (char * descriptor);

Lista_Objetos * buscaObjetoViaReferencia (ClassFile * p);

int getPositionField (ClassFile * obj, char * nomeField);


ClassFile * instanciarClasse (char * nomeClasse);


int getTipoOperandoSaida(char * descriptorRetorno);

int getTipoRetorno (char * descriptor);


void nop_impl(frame *par0,u1 par1, u1 par2);

void aconst_null_impl(frame *f, u1 par1, u1 par2);

void iconst_m1_impl(frame *f, u1 par1, u1 par2);

void iconst_0_impl(frame *f, u1 par1, u1 par2);

void iconst_1_impl(frame *f, u1 par1, u1 par2);

void iconst_2_impl(frame *f, u1 par1, u1 par2);

void iconst_3_impl(frame *f, u1 par1, u1 par2);

void iconst_4_impl(frame *f, u1 par1, u1 par2);

void iconst_5_impl(frame *f, u1 par1, u1 par2);

void lconst_0_impl(frame *f, u1 par1, u1 par2);

void lconst_1_impl(frame *f, u1 par1, u1 par2);

void fconst_0_impl(frame *f, u1 par1, u1 par2);

void fconst_1_impl(frame *f, u1 par1, u1 par2);

void fconst_2_impl(frame *f, u1 par1, u1 par2);

void dconst_0_impl(frame *f, u1 par1, u1 par2);

void dconst_1_impl(frame *f, u1 par1, u1 par2);

void bipush_impl(frame *f, u1 byte, u1 par1);

void sipush_impl(frame *f,u1 byte1, u1 byte2);

void ldc_impl(frame *f,u1 indexbyte1,u1 par2);

void ldc_w_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void ldc2_w_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void iload_impl(frame *f, u1 index, u1 par1);

void lload_impl(frame *f, u1 index, u1 par1);

void fload_impl(frame *f, u1 index, u1 par1);

void dload_impl(frame *par0, u1 par1,u1 par2);

void aload_impl(frame *f, u1 index, u1 par1);

void iload_0_impl(frame *f, u1 par1, u1 par2);

void iload_1_impl(frame *f, u1 par1, u1 par2);

void iload_2_impl(frame *f, u1 par1, u1 par2);

void iload_3_impl(frame *f, u1 par1, u1 par2);

void lload_0_impl(frame *f, u1 par1, u1 par2);

void lload_1_impl(frame *f, u1 par1, u1 par2);

void lload_2_impl(frame *f, u1 par1, u1 par2);

void lload_3_impl(frame *f, u1 par1, u1 par2);

void fload_0_impl(frame *f, u1 par1, u1 par2);

void fload_1_impl(frame *f, u1 par1, u1 par2);

void fload_2_impl(frame *f, u1 par1, u1 par2);

void fload_3_impl(frame *f, u1 par1, u1 par2);

void dload_0_impl(frame *f, u1 par1, u1 par2);

void dload_1_impl(frame *f, u1 par1, u1 par2);

void dload_2_impl(frame *f, u1 par1, u1 par2);

void dload_3_impl(frame *f, u1 par1, u1 par2);

void aload_0_impl(frame *f, u1 par1, u1 par2);

void aload_1_impl(frame *f, u1 par1, u1 par2);

void aload_2_impl(frame *f, u1 par1, u1 par2);

void aload_3_impl(frame *f, u1 par1, u1 par2);

void iaload_impl(frame *f, u1 par1, u1 par2);

void laload_impl(frame *f, u1 par1, u1 par2);

void faload_impl(frame *f, u1 par1, u1 par2);

void daload_impl(frame *f, u1 par1, u1 par2);

void aaload_impl(frame *f, u1 par1, u1 par2);

void baload_impl(frame *f, u1 par1, u1 par2);

void caload_impl(frame *f, u1 par1, u1 par2);

void saload_impl(frame *f, u1 par1, u1 par2);

void istore_impl(frame *f, u1 index, u1 par1);

void lstore_impl(frame *f, u1 index, u1 par1);

void fstore_impl(frame *f, u1 index, u1 par1);

void dstore_impl(frame *f, u1 index, u1 par1);

void astore_impl(frame *f,u1 index, u1 par1);

void istore_0_impl(frame *f, u1 par1, u1 par2);

void istore_1_impl(frame *f, u1 par1, u1 par2);

void istore_2_impl(frame *f, u1 par1, u1 par2);

void istore_3_impl(frame *f, u1 par1, u1 par2);

void lstore_0_impl(frame *f, u1 par1, u1 par2);

void lstore_1_impl(frame *f, u1 par1, u1 par2);

void lstore_2_impl(frame *f, u1 par1, u1 par2);

void lstore_3_impl(frame *f, u1 par1, u1 par2);

void fstore_0_impl(frame *f, u1 par1, u1 par2);

void fstore_1_impl(frame *f, u1 par1, u1 par2);

void fstore_2_impl(frame *f, u1 par1, u1 par2);

void fstore_3_impl(frame *f, u1 par1, u1 par2);

void dstore_0_impl(frame *f, u1 par1, u1 par2);

void dstore_1_impl(frame *f, u1 par1, u1 par2);

void dstore_2_impl(frame *f, u1 par1, u1 par2);

void dstore_3_impl(frame *f, u1 par1, u1 par2);

void astore_0_impl(frame *f, u1 par1, u1 par2);

void astore_1_impl(frame *f, u1 par1, u1 par2);

void astore_2_impl(frame *f, u1 par1, u1 par2);

void astore_3_impl(frame *f, u1 par1, u1 par2);

void iastore_impl(frame *f, u1 par1, u1 par2);

void lastore_impl(frame *f, u1 par1, u1 par2);

void fastore_impl(frame *f, u1 par1, u1 par2);

void dastore_impl(frame *f, u1 par1, u1 par2);

void aastore_impl(frame *f, u1 par1, u1 par2);

void bastore_impl(frame *f, u1 par1, u1 par2);

void castore_impl(frame *f, u1 par1, u1 par2);

void sastore_impl(frame *f, u1 par1, u1 par2);

pilha_operandos* pop_impl(frame *f);

pilha_operandos** pop2_impl(frame *f);

void pop_fantasma(frame *par0, u1 par1, u1 par2);

void pop2_fantasma(frame *par0, u1 par1, u1 par2);

void dup_impl(frame *f, u1 par1, u1 par2);

void dup_x1_impl(frame *f, u1 par1, u1 par2);

void dup_x2_impl(frame *f, u1 par1, u1 par2);

void dup2_impl(frame *f, u1 par1, u1 par2);

void dup2_x1_impl(frame *f, u1 par1, u1 par2);

void dup2_x2_impl(frame *f, u1 par1, u1 par2);

void swap_impl(frame *f, u1 par1, u1 par2);

void iadd_impl(frame *f, u1 par1, u1 par2);

void ladd_impl(frame *f, u1 par1, u1 par2);

void fadd_impl(frame *f, u1 par1, u1 par2);

void dadd_impl(frame *f, u1 par1, u1 par2);

void isub_impl(frame *f, u1 par1, u1 par2);

void lsub_impl(frame *f, u1 par1, u1 par2);

void fsub_impl(frame *f, u1 par1, u1 par2);

void dsub_impl(frame *f, u1 par1, u1 par2);

void imul_impl(frame *f, u1 par1, u1 par2);

void lmul_impl(frame *f, u1 par1, u1 par2);

void fmul_impl(frame *f, u1 par1, u1 par2);

void dmul_impl(frame *f, u1 par1, u1 par2);

void idiv_impl(frame *f, u1 par1, u1 par2);

void ldiv_impl(frame *f, u1 par1, u1 par2);

void fdiv_impl(frame *f, u1 par1, u1 par2);

void ddiv_impl(frame *f, u1 par1, u1 par2);

void irem_impl(frame *f, u1 par1, u1 par2);

void lrem_impl(frame *f, u1 par1, u1 par2);

void frem_impl(frame *f, u1 par1, u1 par2);

void drem_impl(frame *f, u1 par1, u1 par2);

void ineg_impl(frame *f, u1 par1, u1 par2);

void lneg_impl(frame *f, u1 par1, u1 par2);

void fneg_impl(frame *f, u1 par1, u1 par2);

void dneg_impl(frame *f, u1 par1, u1 par2);

void ishl_impl(frame *f, u1 par1, u1 par2);

void lshl_impl(frame *f, u1 par1, u1 par2);

void ishr_impl(frame *f, u1 par1, u1 par2);

void lshr_impl(frame *f, u1 par1, u1 par2);

void iushr_impl(frame *f, u1 par1, u1 par2);

void lushr_impl(frame *f, u1 par1, u1 par2);

void iand_impl(frame *f, u1 par1, u1 par2);

void land_impl(frame *f, u1 par1, u1 par2);

void ior_impl(frame *f, u1 par1, u1 par2);

void lor_impl(frame *f, u1 par1, u1 par2);

void ixor_impl(frame *f, u1 par1, u1 par2);

void lxor_impl(frame *f, u1 par1, u1 par2);

void iinc_fantasma(frame *par0, u1 par1, u1 par2);

void iinc_impl(frame *f, u1 index, i1 constante);

void iinc_wide_fantasma(frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2);

void iinc_wide(frame *f, u2 indexbyte, i2 constbyte);

void i2l_impl(frame *f, u1 par1, u1 par2);

void i2f_impl(frame *f, u1 par1, u1 par2);

void i2d_impl(frame *f, u1 par1, u1 par2);

void l2i_impl(frame *f, u1 par1, u1 par2);

void l2f_impl(frame *f, u1 par1, u1 par2);

void l2d_impl(frame *f, u1 par1, u1 par2);

void f2i_impl(frame *f, u1 par1, u1 par2);

void f2l_impl(frame *f, u1 par1, u1 par2);

void f2d_impl(frame *f, u1 par1, u1 par2);

void d2i_impl(frame *f, u1 par1, u1 par2);

void d2l_impl(frame *f, u1 par1, u1 par2);

void d2f_impl(frame *f, u1 par1, u1 par2);

void i2b_impl(frame *f, u1 par1, u1 par2);

void i2c_impl(frame *f, u1 par1, u1 par2);

void i2s_impl(frame *f, u1 par1, u1 par2);

void lcmp_impl(frame *f, u1 par1, u1 par2);

void fcmpl_impl(frame *f, u1 par1, u1 par2);

void fcmpg_impl(frame *f, u1 par1, u1 par2);

void dcmpl_impl(frame *f, u1 par1, u1 par2);

void dcmpg_impl(frame *f, u1 par1, u1 par2);

void ifeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void ifne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void iflt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void ifge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void ifgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void ifle_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void if_icmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void if_icmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void if_icmplt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void if_icmpge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void if_icmpgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void if_icmple_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void acmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void acmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void inst_goto_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

void jsr_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

void ret_impl(frame *f, u1 index, u1 par1);

void tableswitch_fantasma(frame *par0, u1 par1, u1 par2);

void lookupswitch_fantasma(frame *par0, u1 par1, u1 par2);

void tableswitch_impl(frame *f);

void lookupswitch_impl(frame *f);

void ireturn_impl(frame *f, u1 par1, u1 par2);

void lreturn_impl(frame *f, u1 par1, u1 par2);

void freturn_impl(frame *f, u1 par1, u1 par2);

void dreturn_impl(frame *f, u1 par1, u1 par2);

void areturn_impl(frame *f, u1 par1, u1 par2);

void inst_return_impl(frame *f, u1 par1, u1 par2);

void getstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void putstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void getfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void putfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void invokevirtual_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void invokespecial_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void invokestatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void invokeinterface_fantasma(frame *par0, u1 par1, u1 par2);

void invokeinterface_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 count);

void invokedynamic_fantasma(frame *par0, u1 par1, u1 par2);

void inst_new_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void newarray_impl(frame *f ,u1 atype, u1 par1);

void anewarray_impl(frame *f, u1 par1, u1 par2);

void arraylength_impl(frame *f, u1 par1, u1 par2);

void athrow_impl(frame *f, u1 par1, u1 par2);

void checkcast_impl(frame *f, u1 par1, u1 par2);

void instanceof_impl(frame *f, u1 par1, u1 par2);

void monitorenter_impl(frame *f, u1 par1, u1 par2);

void monitorexit_impl(frame *f, u1 par1, u1 par2);

void wide_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

void multianewarray_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions);

void multianewarray_fantasma(frame *f, u1 par1, u1 par2);

void ifnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void ifnonnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

void goto_w_impl(frame *f, u1 par1, u1 par2);

void jsr_w_impl(frame *f, u1 par1, u1 par2);

extern void (*func_ptr[202])(frame *,u1,u1);

#endif
