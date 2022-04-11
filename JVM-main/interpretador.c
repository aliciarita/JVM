/*
 interpretador.c
 Arquivo de implementação das instrucõess e funcoes auxiliares
 */
#include "interpretador.h"
#include "jvm.h"
#include "leitor.h"

/*
 Função para resolver (analisar e alocar) uma classe
 */
ClassFile* resolverClasse(char* nome_classe){
	classesCarregadas *c = BuscarElemento_classes(jvm->classes,nome_classe);
	ClassFile *classe = NULL;

	if(c!=NULL){
		return c->arquivoClass;
	}
	else{
		char *nomearquivo = malloc((strlen(nome_classe)+7)*sizeof(char));
		strcpy(nomearquivo,nome_classe);
		strcat(nomearquivo,".class");
		classe = lerArquivo(nomearquivo);
		jvm->classes = InserirFim_classes(jvm->classes,classe);
	}

	return(classe);
}

/*
 Função para resolver (analisar e alocar) um metodo
 */
bool resolverMetodo(cp_info *cp, u2 indice_cp, u1 interface){

	cp_info *methodref = cp-1+indice_cp;
	char *nome_classe = NULL;
	if(!interface){
		nome_classe = decodificaNIeNT(cp,methodref->UnionCP.Methodref.class_index,NAME_INDEX);;
	}
	else{
		nome_classe = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.class_index,NAME_INDEX);
	}

	if(resolverClasse(nome_classe)!=NULL){
		return true;
	}
	else{
		return false;
	}
}

/*
 Função para obter o nome de um metodo na pool de constantes
 */
char* obterNomeMetodo(cp_info *cp, u2 indice_cp, u1 interface){
	cp_info *methodref = cp-1+indice_cp;
	char *descriptor = NULL;
	if(!interface){
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
	}
	else{
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.name_and_type_index,NAME_AND_TYPE);
	}
	char *pch = strtok(descriptor,":");

	return(pch);
}

/*
 Função para obter o descriptor de um método
 */
char* obterDescriptorMetodo(cp_info *cp, u2 indice_cp, u1 interface){
	cp_info *methodref = cp-1+indice_cp;
	char *descriptor = NULL;
	if(!interface){
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
	}
	else{
		descriptor = decodificaNIeNT(cp,methodref->UnionCP.InterfaceMethodref.name_and_type_index,NAME_AND_TYPE);
	}

	char *pch = strtok(descriptor,":");
	pch = strtok(NULL,":");

	return(pch);
}

/*
 Função para obter o tipo do descriptor do método
 */
int descriptorFieldValidate (char * descriptor) {

	if (*descriptor == 'L') {
		return 5;
	}

	if (strcmp(descriptor,"I") == 0) {
		return 0;
	} else if (strcmp(descriptor,"F") == 0) {
		return 1;
	} else if (strcmp(descriptor,"B") == 0) {
		return 2;
	} else if (strcmp(descriptor,"C") == 0) {
		return 3;
	} else if (strcmp(descriptor,"S") == 0) {
		return 4;
	} else if (strcmp(descriptor,"L") == 0) {
		return 5;
	} else if (strcmp(descriptor,"Z") == 0) {
		return 6;
	} else if (strcmp(descriptor,"D") == 0) {
		return 7;
	} else if (strcmp(descriptor,"J") == 0) {
		return 8;
	}
	return 0;
}

/*
Função para obter a classe executante de um método
 */
char* obterClasseDoMetodo(cp_info *cp, u2 indice_cp){
	cp_info *methodref = cp-1+indice_cp;
	char *nome_classe = NULL;

	nome_classe = decodificaNIeNT(cp,methodref->UnionCP.Methodref.class_index,NAME_INDEX);
	return(nome_classe);
}

/*
 Função para se uma classe possui a flag super
 */
bool isSuper(u2 flag){
	int super = false;
	while(flag){

		if(flag>=SYNTHETIC){
			flag-=SYNTHETIC;
		}

		if(flag>=ABSTRACT){
			flag-=ABSTRACT;
		}

		if(flag>=INTERFACE_FLAG){
			flag-=INTERFACE_FLAG;
		}

		if(flag>=TRANSIENT){
			flag-=TRANSIENT;
		}

		if(flag>=VOLATILE){
			flag-=VOLATILE;
		}

		if(flag>=SUPER){
			flag-=SUPER;
			super = true;
			break;
		}

		if(flag>=FINAL){
			flag-=FINAL;
		}

		if(flag>=STATIC){
			flag-=STATIC;
		}

		if(flag>=PROTECTED){
			flag-=PROTECTED;
		}

		if(flag>=PRIVATE){
			flag-=PRIVATE;
		}

		if(flag>=PUBLIC){
			flag-=PUBLIC;
		}
	}

	return(super);
}

/*
 Função para transferir os valores da Pilha de Operandos de um frame para o vetor de variaveis locais do próximo frame
 */
frame* transferePilhaVetor(frame *anterior, frame *novo, int *parametros_cont){

	pilha_operandos *aux = CriaPilhaOperandos();
	int cont = 0;
	while(anterior->p->topo!=NULL && cont<(*parametros_cont)){
		pilha_operandos *p = PopOperandos(anterior->p);
		aux = PushOperandos(aux,p->topo->operando,(void*)p->topo->referencia,p->topo->tipo_operando);
		cont++;
	}

	for(int i = 0; i < cont; i++) {
		pilha_operandos *p = PopOperandos(aux);
		novo->v[i].variavel = malloc(sizeof(u4));
		if(p->topo->tipo_operando<=8){
			*(novo->v[i].variavel) = (u4) p->topo->operando;
		}
		else{
			*(novo->v[i].variavel) = (intptr_t) p->topo->referencia;
		}
		novo->v[i].tipo_variavel = (u1) p->topo->tipo_operando;
	}

	*parametros_cont = cont;

	return(novo);
}

/*
 Função para transferir os valores da Pilha de Operandos de um frame para o vetor de variaveis locais do próximo frame
 */
frame *transferePilhaVetorCount(frame *f, frame *novo,int quantidade){
	pilha_operandos *aux = CriaPilhaOperandos();
	int cont = 0;
	while(cont<quantidade){
		pilha_operandos *p = PopOperandos(f->p);
		aux = PushOperandos(aux,p->topo->operando,(void*)p->topo->referencia,p->topo->tipo_operando);
		cont++;
	}

	novo->v = malloc(cont*sizeof(vetor_locais));

	for(int i=0;i<cont;i++){
		pilha_operandos *p = PopOperandos(aux);
		novo->v[i].variavel = malloc(sizeof(u4));
		if(p->topo->tipo_operando<=8){
			*(novo->v[i].variavel) = (u4) p->topo->operando;
		}
		else{
			*(novo->v[i].variavel) = (intptr_t) p->topo->referencia;
		}
		novo->v[i].tipo_variavel = (u1) p->topo->tipo_operando;
	}

	return(novo);
}

/*
Não executa nada
 */
void nop_impl(frame *par0, u1 par1, u1 par2){
	return;
}

/*
Faz o push de um valor nulo para a pilha de operandos 
 */
void aconst_null_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,0,NULL,REFERENCE_OP); 
}


void iconst_m1_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) -1;
	PushOperandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

/*
Faz o push para a pilha de operando da constante 0 (int)
 */
void iconst_0_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) 0;
	PushOperandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

/*
 Faz o push para a pilha de operando da constante 1 (int)
 */
void iconst_1_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) 1;
	PushOperandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

/*
 Faz o push para a pilha de operando da constante 2 (int)
 */
void iconst_2_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) 2;
	PushOperandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

/*
 Faz o push para a pilha de operando da constante 3 (int) 
 */
void iconst_3_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) 3;
	PushOperandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

/*
Faz o push para a pilha de operando da constante 4 (int)
 */
void iconst_4_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) 4;
	PushOperandos(f->p,inteiro_sinal,NULL,INTEGER_OP);
}

/*
 Faz o push para a pilha de operando da constante 5 (int)
 */
void iconst_5_impl(frame *f, u1 par1, u1 par2){
	i4 inteiro_sinal = (i4) 5;
	PushOperandos(f->p,(u4)inteiro_sinal,NULL,INTEGER_OP);
}

/*
 Faz o push para a pilha de operando da constante 0 (long)
 */
void lconst_0_impl(frame *f, u1 par1, u1 par2){
	i4 high_bytes = (i4) 0;
	PushOperandos(f->p,high_bytes,NULL,LONG_OP);
	i4 low_bytes = (i4) 0;
	PushOperandos(f->p,low_bytes,NULL,LONG_OP);
}

/*
 Faz o push para a pilha de operando da constante 1 (long)
 */
void lconst_1_impl(frame *f, u1 par1, u1 par2){
	i4 high_bytes = (i4) 0;
	PushOperandos(f->p,high_bytes,NULL,LONG_OP);
	i4 low_bytes = (i4) 1;
	PushOperandos(f->p,low_bytes,NULL,LONG_OP);
}

/*
 Faz o push para a pilha de operando da constante 0 (float)
 */
void fconst_0_impl(frame *f, u1 par1, u1 par2){
	i4 float_bytes = (i4) 0;
	PushOperandos(f->p,float_bytes,NULL,FLOAT_OP);
}

/*
Faz o push para a pilha de operando da constante 1 (float)
 */
void fconst_1_impl(frame *f, u1 par1, u1 par2){
	i4 float_bytes = 0x3f800000;
	PushOperandos(f->p,float_bytes,NULL,FLOAT_OP);
}

/*
 Faz o push para a pilha de operando da constante 2 (float)
 */
void fconst_2_impl(frame *f, u1 par1, u1 par2){
	i4 float_bytes = 0x40000000;
	PushOperandos(f->p,float_bytes,NULL,FLOAT_OP);
}

/*
 Faz o push para a pilha de operando da constante 0 (double)
 */
void dconst_0_impl(frame *f, u1 par1, u1 par2){
	i4 high_bytes = (i4) 0;
	PushOperandos(f->p,high_bytes,NULL,DOUBLE_OP);
	i4 low_bytes = (i4) 0;
	PushOperandos(f->p,low_bytes,NULL,DOUBLE_OP);
}

/*
 Faz o push para a pilha de operando da constante 1 (double)
 */
void dconst_1_impl(frame *f, u1 par1, u1 par2){
	i4 high_bytes = 0x3FF00000;
	PushOperandos(f->p,high_bytes,NULL,DOUBLE_OP);
	i4 low_bytes = (i4) 0;
	PushOperandos(f->p,low_bytes,NULL,DOUBLE_OP);
}

/*
 Faz o push de um byte na pilha de operandos
 */
void bipush_impl(frame *f, u1 byte, u1 par1){
	i1 aux = (i1)byte;
	i4 byte_int = (i4) aux;
	PushOperandos(f->p,byte_int,NULL,BYTE_OP);
}

/*
 Faz o push de um short na pilha de operandos
 */
void sipush_impl(frame *f, u1 byte1, u1 byte2){
	i2 byte_short = (i2)(byte1<<8) | (i2)byte2;
	i4 byte_int = (i4) byte_short;
	PushOperandos(f->p,byte_int,NULL,SHORT_OP);
}

/*
Faz o push de um valor presente na constant pool
 */
void ldc_impl(frame *f, u1 indexbyte1, u1 par2){
	cp_info *item = f->cp-1 + indexbyte1;
	void *valor=NULL;
	u4 num=0;
	void *classe=NULL;

	switch(item->tag){
		case CONSTANT_String:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.String.string_index);
			f->p = PushOperandos(f->p,-INT_MAX,valor,REFERENCE_OP);
		break;
		case CONSTANT_Float:
			num = item->UnionCP.Float.bytes;
			f->p = PushOperandos(f->p,num,NULL,FLOAT_OP);
		break;
		case CONSTANT_Integer:
			num = item->UnionCP.Integer.bytes;
			f->p = PushOperandos(f->p,num,NULL,INTEGER_OP);
		break;
		case CONSTANT_Class:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.Class.name_index);
			classe = resolverClasse(valor);
			f->p = PushOperandos(f->p,-INT_MAX,classe,REFERENCE_OP);
		break;
		default:
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.class_index,NAME_INDEX);
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);

		break;
	}

}

/*
 Faz o push de um valor presente na constant pool
 */
void ldc_w_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);
	cp_info *item = f->cp-1 + indice_cp;
	void *valor=NULL;
	u4 num=0;
	void *classe=NULL;

	switch(item->tag){
		case CONSTANT_String:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.String.string_index);
			f->p = PushOperandos(f->p,-INT_MAX,valor,REFERENCE_OP);
		break;
		case CONSTANT_Float:
			num = item->UnionCP.Float.bytes;
			f->p = PushOperandos(f->p,num,NULL,FLOAT_OP);
		break;
		case CONSTANT_Integer:
			num = item->UnionCP.Integer.bytes;
			f->p = PushOperandos(f->p,num,NULL,INTEGER_OP);
		break;
		case CONSTANT_Class:
			valor = (char*) decodificaStringUTF8(f->cp-1+item->UnionCP.Class.name_index);
			classe = resolverClasse(valor);
			f->p = PushOperandos(f->p,-INT_MAX,classe,REFERENCE_OP);
		break;
		default:
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.class_index,NAME_INDEX);
			valor = (char *) decodificaNIeNT(f->cp,item->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
		break;
	}

}

/*
 Faz o push de um valor long ou double presente na constant pool
 */
void ldc2_w_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	int8_t v1 = (int8_t)branchbyte1;
	int8_t v2 = (int8_t)branchbyte2;
	int16_t branchoffset = (v1 << 8) | v2;
	cp_info * doubleValue = f->cp-1+branchoffset;

	u4 high,low;

	if(doubleValue->tag == CONSTANT_Double){
		high = doubleValue->UnionCP.Double.high_bytes;
		low = doubleValue->UnionCP.Double.low_bytes;
	} else {
		high = doubleValue->UnionCP.Long.high_bytes;
		low = doubleValue->UnionCP.Long.low_bytes;
	}

	if(doubleValue->tag == CONSTANT_Double){
		PushOperandos(f->p,high,NULL,DOUBLE_OP);
		PushOperandos(f->p,low,NULL,DOUBLE_OP);
	} else {
		PushOperandos(f->p,high,NULL,LONG_OP);
		PushOperandos(f->p,low,NULL,LONG_OP);
	}
}

/*
  Carrega um valor int do vetor de variáveis locais
 */
void iload_impl(frame *f, u1 index, u1 par1){
	PushOperandos(f->p,(i4) *(f->v[index].variavel),NULL,INTEGER_OP);
}

/*
 Carrega um valor long do vetor de variáveis locais 
 */
void lload_impl(frame *f, u1 index, u1 par1){
	
	PushOperandos(f->p,(i4) *(f->v[index].variavel),NULL,LONG_OP);
	PushOperandos(f->p,(i4) *(f->v[index+1].variavel),NULL,LONG_OP);
}

/*
 Carrega um valor float do vetor de variáveis locais
 */
void fload_impl(frame *f, u1 index, u1 par1){
	PushOperandos(f->p, (i4) *(f->v[index].variavel),NULL,FLOAT_OP);
}

/*
 Carrega um valor double do vetor de variáveis locais 
 */
void dload_impl(frame *f, u1 index, u1 par2){
	
	PushOperandos(f->p,(i4) *(f->v[index].variavel),NULL,DOUBLE_OP);
	PushOperandos(f->p,(i4) *(f->v[index+1].variavel),NULL,DOUBLE_OP);
}

/*
 Carrega uma referência do vetor de variáveis locais
 */
void aload_impl(frame *f, u1 index, u1 par1){
	PushOperandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[index].variavel),f->v[index].tipo_variavel);
}

/*
 Carrega um valor int da posição 0 do vetor de variáveis locais
 */
void iload_0_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[0].variavel),NULL,INTEGER_OP);
}

/**
 * - Carrega um valor int da posição 1 do vetor de variáveis locais
 */
void iload_1_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[1].variavel),NULL,INTEGER_OP);
}

/**
 * - Carrega um valor int da posição 2 do vetor de variáveis locais
 */
void iload_2_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[2].variavel),NULL,INTEGER_OP);
}

/**
 * - Carrega um valor int da posição 3 do vetor de variáveis locais
 */
void iload_3_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[3].variavel),NULL,INTEGER_OP);
}

/**
 * - Carrega um valor long da posição 0 do vetor de variáveis locais
 */
void lload_0_impl(frame *f, u1 par, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[0].variavel),NULL,LONG_OP);
	PushOperandos(f->p,(i4) *(f->v[1].variavel),NULL,LONG_OP);
}

/**
 * - Carrega um valor long da posição 1 do vetor de variáveis locais
 */
void lload_1_impl(frame *f, u1 par, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[1].variavel),NULL,LONG_OP);
	PushOperandos(f->p,(i4) *(f->v[2].variavel),NULL,LONG_OP);
}

/**
 * - Carrega um valor long da posição 2 do vetor de variáveis locais
 */
void lload_2_impl(frame *f, u1 par, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[2].variavel),NULL,LONG_OP);
	PushOperandos(f->p,(i4) *(f->v[3].variavel),NULL,LONG_OP);
}

/**
 * - Carrega um valor long da posição 3 do vetor de variáveis locais
 */
void lload_3_impl(frame *f, u1 par, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[3].variavel),NULL,LONG_OP);
	PushOperandos(f->p,(i4) *(f->v[4].variavel),NULL,LONG_OP);
}

/**
 * - Carrega um valor float da posição 0 do vetor de variáveis locais
 */
void fload_0_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[0].variavel),NULL,FLOAT_OP);
}

/**
 * - Carrega um valor float da posição 1 do vetor de variáveis locais
 */
void fload_1_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[1].variavel),NULL,FLOAT_OP);
}

/**
 * - Carrega um valor float da posição 2 do vetor de variáveis locais
 */
void fload_2_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[2].variavel),NULL,FLOAT_OP);
}

/**
 * - Carrega um valor float da posição 3 do vetor de variáveis locais
 */
void fload_3_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[3].variavel),NULL,FLOAT_OP);
}

/**
 * - Carrega um valor double da posição 0 do vetor de variáveis locais
 */
void dload_0_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[0].variavel),NULL,DOUBLE_OP);
	PushOperandos(f->p,(i4) *(f->v[1].variavel),NULL,DOUBLE_OP);
}

/**
 * - Carrega um valor double da posição 1 do vetor de variáveis locais
 */
void dload_1_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[1].variavel),NULL,DOUBLE_OP);
	PushOperandos(f->p,(i4) *(f->v[2].variavel),NULL,DOUBLE_OP);
}

/**
 * - Carrega um valor double da posição 2 do vetor de variáveis locais
 */
void dload_2_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[2].variavel),NULL,DOUBLE_OP);
	PushOperandos(f->p,(i4) *(f->v[3].variavel),NULL,DOUBLE_OP);
}

/**
 * - Carrega um valor double da posição 3 do vetor de variáveis locais
 */
void dload_3_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,(i4) *(f->v[3].variavel),NULL,DOUBLE_OP);
	PushOperandos(f->p,(i4) *(f->v[4].variavel),NULL,DOUBLE_OP);
}

/**
 * - Carrega uma referência da posição 0 do vetor de variáveis locais
 */
void aload_0_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[0].variavel),f->v[0].tipo_variavel);
}

/*
  - Carrega uma referência da posição 1 do vetor de variáveis locais
 */
void aload_1_impl(frame *f, u1 par1, u1 par2){

	PushOperandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[1].variavel),f->v[1].tipo_variavel);
}

/**
 * - Carrega uma referência da posição 2 do vetor de variáveis locais
 */
void aload_2_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[2].variavel),f->v[2].tipo_variavel);
}

/**
 * - Carrega uma referência da posição 3 do vetor de variáveis locais
 */
void aload_3_impl(frame *f, u1 par1, u1 par2){
	PushOperandos(f->p,-INT_MAX,(void*)(intptr_t)*(f->v[3].variavel),f->v[3].tipo_variavel);
}

/**
 * - Carrega um int de um array
 */
void iaload_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	u4* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i4));

    PushOperandos(f->p,*endereco,NULL,INTEGER_OP);
}

/**
 * - Carrega um long de um array
 */
void laload_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	u4* endereco = (referencia->topo->referencia) + (indice->topo->operando * 2 * sizeof(i4));
	PushOperandos(f->p,*endereco,NULL,LONG_OP);
	endereco++;
	PushOperandos(f->p,*(endereco),NULL,LONG_OP);
}

/**
 * - Carrega um float de um array
 */
void faload_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	u4* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i4));

    PushOperandos(f->p,*endereco,NULL,FLOAT_OP);
}

/**
 * - Carrega um double de um array
 */
void daload_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	u4* endereco = ( referencia->topo->referencia) + (indice->topo->operando * 2 * sizeof(u4));
	PushOperandos(f->p,*endereco,NULL,DOUBLE_OP);
	endereco++;
	PushOperandos(f->p,*(endereco),NULL,DOUBLE_OP);
}

/**
 * - Carrega uma referência de um array
 */
void aaload_impl(frame *f, u1 par1, u1 par2){
}

/**
 * - Carrega um byte de um array
 */
void baload_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	i1* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i1));
	PushOperandos(f->p,*endereco,NULL,BYTE_OP);
}

/**
 * - Carrega um char de um array
 */
void caload_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	u2* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(u2));
	PushOperandos(f->p,*endereco,NULL,CHAR_OP);
}

/**
 * - Carrega um short de um array
 */
void saload_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *referencia = PopOperandos(f->p);

	i2* endereco = (referencia->topo->referencia) + (indice->topo->operando * sizeof(i2));

	PushOperandos(f->p,*endereco,NULL,SHORT_OP);
}

/**
 * - Armazena um int no vetor de variáveis locais
 */
void istore_impl(frame *f, u1 index,u1 par1){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[index].variavel) = (i4) valor->topo->operando;
}

/**
 * - Armazena um long no vetor de variáveis locais
 */
void lstore_impl(frame *f, u1 index, u1 par1){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[index].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[index+1].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um float no vetor de variáveis locais
 */
void fstore_impl(frame *f, u1 index,u1 par1){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[index].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um double no vetor de variáveis locais
 */
void dstore_impl(frame *f, u1 index, u1 par1){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[index].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[index+1].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena uma referência no vetor de variáveis locais
 */
void astore_impl(frame *f, u1 index,u1 par1){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[index].variavel) = (intptr_t) valor->topo->referencia;
	f->v[index].tipo_variavel = valor->topo->tipo_operando;
}

/**
 * - Armazena um int na posição 0 do vetor de variáveis locais
 */
void istore_0_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[0].variavel) = (i4) valor->topo->operando;
}

/**
 * - Armazena um int na posição 1 do vetor de variáveis locais
 */
void istore_1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[1].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um int na posição 2 do vetor de variáveis locais
 */
void istore_2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[2].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um int na posição 3 do vetor de variáveis locais
 */
void istore_3_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[3].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um long na posição 0 do vetor de variáveis locais
 */
void lstore_0_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[0].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[1].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um long na posição 1 do vetor de variáveis locais
 */
void lstore_1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[1].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[2].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um long na posição 2 do vetor de variáveis locais
 */
void lstore_2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[2].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[3].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um long na posição 3 do vetor de variáveis locais
 */
void lstore_3_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[3].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[4].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um float na posição 0 do vetor de variáveis locais
 */
void fstore_0_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[0].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um float na posição 1 do vetor de variáveis locais
 */
void fstore_1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[1].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um float na posição 2 do vetor de variáveis locais
 */
void fstore_2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[2].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um float na posição 3 do vetor de variáveis locais
 */
void fstore_3_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[3].variavel) = (u4) valor->topo->operando;
}

/**
 * - Armazena um double na posição 0 do vetor de variáveis locais
 */
void dstore_0_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[0].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[1].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um double na posição 1 do vetor de variáveis locais
 */
void dstore_1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[1].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[2].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um double na posição 2 do vetor de variáveis locais
 */
void dstore_2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[2].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[3].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena um double na posição 3 do vetor de variáveis locais
 */
void dstore_3_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	*(f->v[3].variavel) = (u4) high_bytes->topo->operando;
	*(f->v[4].variavel) = (u4) low_bytes->topo->operando;
}

/**
 * - Armazena uma referência na posição 0 do vetor de variáveis locais
 */
void astore_0_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[0].variavel) = (intptr_t) valor->topo->referencia;
	f->v[0].tipo_variavel = valor->topo->tipo_operando;
}

/**
 * - Armazena uma referência na posição 1 do vetor de variáveis locais
 */
void astore_1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);

	*(f->v[1].variavel) = (intptr_t) valor->topo->referencia;
	f->v[1].tipo_variavel = valor->topo->tipo_operando;

}

/**
 * - Armazena uma referência na posição 2 do vetor de variáveis locais
 */
void astore_2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[2].variavel) = (intptr_t) valor->topo->referencia;
	f->v[2].tipo_variavel = valor->topo->tipo_operando;
}

/**
 * - Armazena uma referência na posição 3 do vetor de variáveis locais
 */
void astore_3_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	*(f->v[3].variavel) = (intptr_t) valor->topo->referencia;
	f->v[3].tipo_variavel = valor->topo->tipo_operando;
}

/**
 * - Armazena um valor int em um array
 */
void iastore_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	i4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i4)));
	*endereco_array = valor->topo->operando;
}

/**
 * - Armazena um valor long em um array
 */
void lastore_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	i4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * 2 * sizeof(i4)));

	*endereco_array = high_bytes->topo->operando;

	endereco_array++;

	*endereco_array = low_bytes->topo->operando;
}

/**
 * - Armazena um valor float em um array
 */
void fastore_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	i4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i4)));
	*endereco_array = valor->topo->operando;
}

/**
 * - Armazena um valor double em um array
 */
void dastore_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	u4 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * 2 * sizeof(u4)));

	*endereco_array = high_bytes->topo->operando;

	endereco_array++;

	*endereco_array = low_bytes->topo->operando;
}

/**
 * - Armazena um valor byte em um array
 */
void bastore_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	i1 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i1)));
	*endereco_array = valor->topo->operando;
}

/**
 * - Armazena um valor char em um array
 */
void castore_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	u2 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(u2)));
	*endereco_array = valor->topo->operando;
}

/**
 * - Armazena um valor short em um array
 */
void sastore_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	pilha_operandos *indice = PopOperandos(f->p);
	pilha_operandos *array = PopOperandos(f->p);

	i2 *endereco_array =  ((array->topo->referencia) + (indice->topo->operando * sizeof(i2)));
	*endereco_array = valor->topo->operando;
}

/**
 * - Armazena uma referência em um array
 */
void aastore_impl(frame *f, u1 par1, u1 par2){
}

/**
 * - Remove o valor de topo da pilha de operandos
 */
pilha_operandos* pop_impl(frame *f){
	pilha_operandos *valor = PopOperandos(f->p);

	return(valor);
}

/**
 * - Remove os dois valores de topo da pilha de operandos
 */
pilha_operandos** pop2_impl(frame *f){
	PopOperandos(f->p);
	PopOperandos(f->p);
	return NULL;
}

/**
 * - PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop
 */
void pop_fantasma(frame *f, u1 par1, u1 par2){
	pop_impl(f);
}

/**
 * - PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop2
 */
void pop2_fantasma(frame *par0, u1 par1, u1 par2){

}

/**
 * - Duplica o valor no topo da pilha de operandos.
 */
void dup_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = TopoOperandos(f->p);
	f->p = PushOperandos(f->p,valor->topo->operando,valor->topo->referencia,valor->topo->tipo_operando);
}

/**
 * - Duplica o valor no topo da pilha de operandos e insere dois valores a baixo.
 */
void dup_x1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

/**
 * - Duplica o valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 */
void dup_x2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);
	pilha_operandos *valor3 = PopOperandos(f->p);

	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor3->topo->operando,valor3->topo->referencia,valor3->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

/**
 * - Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere logo abaixo.
 */
void dup2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

/**
 * - Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois valores a baixo.
 */
void dup2_x1_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);
	pilha_operandos *valor3 = PopOperandos(f->p);

	f->p = PushOperandos(f->p,valor3->topo->operando,valor3->topo->referencia,valor3->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

/**
 * - Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 */
void dup2_x2_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);
	pilha_operandos *valor3 = PopOperandos(f->p);
	pilha_operandos *valor4 = PopOperandos(f->p);

	f->p = PushOperandos(f->p,valor4->topo->operando,valor4->topo->referencia,valor4->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor3->topo->operando,valor3->topo->referencia,valor3->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

/**
 * - Inverte os dois operandos do topo da pilha
 */
void swap_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	f->p = PushOperandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = PushOperandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
}

/**
 * - Executa uma soma de int
 */
void iadd_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	pilha_operandos *valor3 = CriaPilhaOperandos();
	valor3 = PushOperandos(valor3,valor1->topo->operando+valor2->topo->operando,NULL,valor1->topo->tipo_operando);
	valor3->topo->tipo_operando = valor1->topo->tipo_operando;
	f->p = PushOperandos(f->p,valor3->topo->operando,NULL,valor3->topo->tipo_operando);

}

/**
 * - Executa uma soma de long
 */
void ladd_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long1 + long2;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, LONG_OP);

}

/**
 * - Executa uma soma de float
 */
void fadd_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);

	float f_sum = op1+op2;
	u4 sum = (u4)(*(u4*)&f_sum);
	u4 result = (sinal(sum)<<31) | (expoente(sum)<<23) | mantissa(sum);
	f->p = PushOperandos(f->p,result,NULL,FLOAT_OP);

}

/**
 * - Executa uma soma de double
 */
void dadd_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_sum = op1+op2;
	u8 sum = (u8)(*(u8*)&d_sum);
	u8 result = (sinal_d(sum)<<63) | (expoente_d(sum)<<52) | mantissa_d(sum);

	f->p = PushOperandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, DOUBLE_OP);

}

/**
 * - Executa uma subtração de int
 */
void isub_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 result = valor2->topo->operando-valor1->topo->operando;
	f->p = PushOperandos(f->p,result,NULL,INTEGER_OP);
}

/**
 * - Executa uma subtração de long
 */
void lsub_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long2 - long1;

	f->p = PushOperandos(f->p, (u4)(result>>32),NULL, LONG_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, LONG_OP);

}

/**
 * - Executa uma subtração de float
 */
void fsub_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	float f_sub = op2-op1;
	u4 sub = (u4)(*(u4*)&f_sub);
	u4 result = (sinal(sub)<<31) | (expoente(sub)<<23) | mantissa(sub);
	f->p = PushOperandos(f->p,result,NULL,FLOAT_OP);
}

/**
 * - Executa uma subtração de double
 */
void dsub_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_sub = op2-op1;
	u8 sub = (u8)(*(u8*)&d_sub);
	u8 result = (sinal_d(sub)<<63) | (expoente_d(sub)<<52) | mantissa_d(sub);

	f->p = PushOperandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, DOUBLE_OP);

}

/**
 * - Executa uma multiplicação de int
 */
void imul_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 result = valor1->topo->operando*valor2->topo->operando;
	f->p = PushOperandos(f->p,result,NULL,INTEGER_OP);
}

/**
 * - Executa uma multiplicação de long
 */
void lmul_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long1 * long2;

	f->p = PushOperandos(f->p, (u4)(result>>32),NULL, LONG_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, LONG_OP);

}

/**
 * - Executa uma multiplicação de float
 */
void fmul_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	float f_res = op1*op2;
	u4 res = (u4)(*(u4*)&f_res);
	u4 result = (sinal(res)<<31) | (expoente(res)<<23) | mantissa(res);
	f->p = PushOperandos(f->p,result,NULL,FLOAT_OP);
}

/**
 * - Executa uma multiplicação de double
 */
void dmul_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_res = op1*op2;
	u8 res = (u8)(*(u8*)&d_res);
	u8 result = (sinal_d(res)<<63) | (expoente_d(res)<<52) | mantissa_d(res);

	f->p = PushOperandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, DOUBLE_OP);

}

/**
 * - Executa uma divisão de int
 */
void idiv_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 result;
	void *messageErrorDiv = NULL;
	char *messageError = "java.lang.ArithmeticException: / by zero";


	if(valor1->topo->operando!=0){
		result = valor2->topo->operando/valor1->topo->operando;
		f->p = PushOperandos(f->p,result,messageErrorDiv,INTEGER_OP);

	}else{
		jvm->excecao = 1;
		result = valor2->topo->operando/valor2->topo->operando;
		strcpy(jvm->excecao_nome,"java/lang/ArithmeticException");

		messageErrorDiv = messageError;
		f->p = PushOperandos(f->p,result,messageErrorDiv,REFERENCE_OP);
	}
}

/**
 * - Executa uma divisão de long
 */
void ldiv_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long2 / long1;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Executa uma divisão de float
 */
void fdiv_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	float f_res = op2/op1;
	u4 res = (u4)(*(u4*)&f_res);
	u4 result = (sinal(res)<<31) | (expoente(res)<<23) | mantissa(res);
	f->p = PushOperandos(f->p,result,NULL,FLOAT_OP);
}

/**
 * - Executa uma divisão de double
 */
void ddiv_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double d_res = op2/op1;
	u8 res = (u8)(*(u8*)&d_res);
	u8 result = (sinal_d(res)<<63) | (expoente_d(res)<<52) | mantissa_d(res);

	f->p = PushOperandos(f->p, (u4)(result>>32),NULL, DOUBLE_OP);
	f->p = PushOperandos(f->p, (u4)result,NULL, DOUBLE_OP);

}

/**
 * - Calcula o resto int da divisão entre dois operandos
 */
void irem_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor1->topo->operando == 0){
	}

	i4 valor_push = valor2->topo->operando - (valor2->topo->operando/valor1->topo->operando) * valor1->topo->operando;

	f->p = PushOperandos(f->p,valor_push,NULL,INTEGER_OP);
}

/**
 * - Calcula o resto long da divisão entre dois operandos
 */
void lrem_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	i8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	i8 result = long2 - (long2/long1) * long1;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Calcula o resto float da divisão entre dois operandos
 */
void frem_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos * valor1 = PopOperandos(f->p);
	pilha_operandos * valor2 = PopOperandos(f->p);

	float v1 = decodificaFloatValor(valor1->topo->operando);
	float v2 = decodificaFloatValor(valor2->topo->operando);

	float resultAux = fmodf(v2,v1);
	u4 result = (u4)(*(u4*)&resultAux);

	f->p = PushOperandos(f->p,result,NULL,FLOAT_OP);

}

/**
 * - Calcula o resto double da divisão entre dois operandos
 */
void drem_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double v1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double v2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);

	double resultAux = fmod(v2,v1);

	u8 result = (u8)(*(u8*)&resultAux);

	f->p = PushOperandos(f->p,(u4)(result>>32),NULL,DOUBLE_OP);
	f->p = PushOperandos(f->p,(u4)result,NULL,DOUBLE_OP);

}

/**
 * - Inverte o sinal de um int
 */
void ineg_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);

	i4 result = -valor1->topo->operando;
	f->p = PushOperandos(f->p,result,NULL,INTEGER_OP);
}

/**
 * - Inverte o sinal de um long
 */
void lneg_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;

	i8 result = -long1;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Inverte o sinal de um float
 */
void fneg_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);

	u4 op1 = valor1->topo->operando;

	u4 result = op1 ^ 0X80000000;

	f->p = PushOperandos(f->p,result,NULL,FLOAT_OP);
}

/**
 * - Inverte o sinal de um double
 */
void dneg_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	u4 result = high_bytes ^ 0X80000000;

	f->p = PushOperandos(f->p, result,NULL, DOUBLE_OP);
	f->p = PushOperandos(f->p, low_bytes,NULL, DOUBLE_OP);
}

/**
 * - Executa um shift left em um int
 */
void ishl_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 s = (valor1->topo->operando << 27) >> 27;

	i4 resultado = valor2->topo->operando << s;

	f->p = PushOperandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

/**
 * - Executa um shift left em um long
 */
void lshl_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 s = (valor2->topo->operando << 27) >> 27;


	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	i8 long1 = ((i8)high_bytes->topo->operando << 32) | low_bytes->topo->operando;

	i8 result = long1 << s;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Executa um shift right em um int
 */
void ishr_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 s = (i4) (valor1->topo->operando << 27) >> 27;

	i4 resultado = (i4) valor2->topo->operando >> s;

	f->p = PushOperandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

/**
 * - Executa um shift right em um long
 */
void lshr_impl(frame *f, u1 par1, u1 par2){


	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 s = (valor2->topo->operando << 27) >> 27;


	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	i8 long1 = ((i8)high_bytes->topo->operando << 32) | low_bytes->topo->operando;

	i8 result = long1 >> s;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Executa um shift right lógico em um int
 */
void iushr_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	u4 s = (u4) (valor2->topo->operando << 27) >> 27;

	u4 resultado = (u4) valor1->topo->operando >> s;

	f->p = PushOperandos(f->p,(i4) resultado,NULL,valor1->topo->tipo_operando);
}

/**
 * - Executa um shift right lógico em um long
 */
void lushr_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor2 = PopOperandos(f->p);

	u4 s = (valor2->topo->operando << 27) >> 27;


	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	u8 long1 = ((u8)high_bytes->topo->operando << 32) | low_bytes->topo->operando;

	u8 result = long1 >> s;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Executa a operação "AND" de dois int
 */
void iand_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 resultado = valor2->topo->operando & valor1->topo->operando;

	f->p = PushOperandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

/**
 * - Executa a operação "AND" de dois long
 */
void land_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	u8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	u8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	u8 result = long2 & long1;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Executa a operação "OR" de dois int
 */
void ior_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 resultado = valor1->topo->operando | valor2->topo->operando;

	f->p = PushOperandos(f->p,resultado,NULL,valor1->topo->tipo_operando);
}

/**
 * - Executa a operação "OR" de dois long
 */
void lor_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	u8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	u8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	u8 result = long2 | long1;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Executa a operação "XOR" de dois int
 */
void ixor_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	i4 resultado = valor1->topo->operando ^ valor2->topo->operando;

	f->p = PushOperandos(f->p,resultado,NULL,INTEGER_OP);
}

/**
 * - Executa a operação "XOR" de dois long
 */
void lxor_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	u8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	u8 long2 = ((u8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;

	u8 result = long2 ^ long1;

	f->p = PushOperandos(f->p, (u4)(result>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)result, NULL,LONG_OP);

}

/**
 * - Instrução para incrementar um valor no vetor de variáveis locais do frame
 */
void iinc_impl(frame *f,u1 index, i1 constante){
	i4 inteiro_constante = (i4) constante;
	*(f->v[index].variavel) += inteiro_constante;
}

/**
 * - Função para mapear a execução da instrução iinc
 */
void iinc_fantasma(frame *par0, u1 par1, u1 par2){
	i1 valor = (i1)par2;
	iinc_impl(par0,par1,valor);
}

/**
 * - Função para mapear a execução da instrução iinc_wide
 */
void iinc_wide_fantasma(frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2){
	u2 indexbyte = normaliza_indice(indexbyte1,indexbyte2);
	i2 constbyte = (i2) ((i1) (constbyte1 << 8) | (i1) constbyte2);

	iinc_wide(f,indexbyte,constbyte);
}

/**
 * - Instrução para incrementar um valor no vetor de variáveis locais do frame
 */
void iinc_wide(frame *f, u2 indexbyte, i2 constbyte){
	i4 inteiro_constante = (i4) constbyte;
	f->v[indexbyte].variavel += inteiro_constante;
}

/**
 * - Instrução para converter valor inteiro para long
 */
void i2l_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	i4 valor = (i4)valor1->topo->operando;

	i8 valor_long = (i8)valor;

	f->p = PushOperandos(f->p, (u4)(valor_long>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)valor_long, NULL,LONG_OP);

}

/**
 * - Instrução para converter valor inteiro para float
 */
void i2f_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	float valor = (float)valor1->topo->operando;
	u4 flo = (u4)(*(u4*)&valor);
	f->p = PushOperandos(f->p,flo,NULL,FLOAT_OP);
}

/**
 * - Instrução para converter valor inteiro para double
 */
void i2d_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	double valor = (double)valor1->topo->operando;

	u8 doub = (u8)(*(u8*)&valor);

	f->p = PushOperandos(f->p,(u4)(doub>>32),NULL,DOUBLE_OP);
	f->p = PushOperandos(f->p,(u4)doub,NULL,DOUBLE_OP);

}

/**
 * - Instrução para converter valor long para inteiro
 */
void l2i_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	PopOperandos(f->p);

	f->p = PushOperandos(f->p, low_bytes1->topo->operando, NULL,INTEGER_OP);
}

/**
 * - Instrução para converter valor long para float
 */
void l2f_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;
	float flo = (float) long1;
	u4 valor = (u4)(*(u4*)&flo);
	f->p = PushOperandos(f->p, valor, NULL,FLOAT_OP);
}

/**
 * - Instrução para converter valor long para double
 */
void l2d_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((u8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;
	double valor = (double) long1;
	u8 doub = (u8)(*(u8*)&valor);

	f->p = PushOperandos(f->p,(u4)(doub>>32),NULL,DOUBLE_OP);
	f->p = PushOperandos(f->p,(u4)doub,NULL,DOUBLE_OP);

}

/**
 * - Instrução para converter valor float para inteiro
 */
void f2i_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	float valor = decodificaFloatValor(valor1->topo->operando);
	f->p = PushOperandos(f->p,(i4)valor,NULL,INTEGER_OP);
}

/**
 * - Instrução para converter valor float para long
 */
void f2l_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	float valor = decodificaFloatValor(valor1->topo->operando);

	i8 valor_long = (i8)valor;

	f->p = PushOperandos(f->p, (u4)(valor_long>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)valor_long, NULL,LONG_OP);
}

/**
 * - Instrução para converter valor float para double
 */
void f2d_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	double valor = (double)decodificaFloatValor(valor1->topo->operando);

	u8 doub = (u8)(*(u8*)&valor);

	f->p = PushOperandos(f->p,(u4)(doub>>32),NULL,DOUBLE_OP);
	f->p = PushOperandos(f->p,(u4)doub,NULL,DOUBLE_OP);
}

/**
 * - Instrução para converter valor double para inteiro
 */
void d2i_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	double valor = decodificaDoubleValor(high_bytes, low_bytes);
	f->p = PushOperandos(f->p,(i4)valor,NULL,INTEGER_OP);
}

/**
 * - Instrução para converter valor double para long
 */
void d2l_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	double valor = decodificaDoubleValor(high_bytes, low_bytes);

	i8 valor_long = (i8)valor;

	f->p = PushOperandos(f->p, (u4)(valor_long>>32), NULL,LONG_OP);
	f->p = PushOperandos(f->p, (u4)valor_long, NULL,LONG_OP);

}

/**
 * - Instrução para converter valor double para float
 */
void d2f_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);

	u4 high_bytes = valor1_high->topo->operando;
	u4 low_bytes = valor1_low->topo->operando;

	float flo = (float) decodificaDoubleValor(high_bytes, low_bytes);
	u4 valor = (u4)(*(u4*)&flo);
	f->p = PushOperandos(f->p, valor, NULL,FLOAT_OP);

}

/**
 * - Instrução para converter valor inteiro para byte
 */
void i2b_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);

	i1 truncado = (i1) valor1->topo->operando;

	i4 resultado = (i4) truncado;

	f->p = PushOperandos(f->p,resultado,NULL,BYTE_OP);
}

/**
 * - Instrução para converter valor inteiro para char
 */
void i2c_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);

	i1 truncado = (i1) valor1->topo->operando;

	i4 resultado = (i4) truncado;

	f->p = PushOperandos(f->p,resultado,NULL,CHAR_OP);
}

/**
 * - Instrução para converter valor inteiro para short
 */
void i2s_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);

	i2 truncado = (i2) valor1->topo->operando;

	i4 resultado = (i4) truncado;

	f->p = PushOperandos(f->p,resultado,NULL,SHORT_OP);
}

/**
 * - Instrução que compara valores long oriundos da pilha de operandos
 */
void lcmp_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes1 = PopOperandos(f->p);
	pilha_operandos *high_bytes1 = PopOperandos(f->p);

	i8 long1 = ((i8)high_bytes1->topo->operando << 32) | low_bytes1->topo->operando;


	pilha_operandos *low_bytes2 = PopOperandos(f->p);
	pilha_operandos *high_bytes2 = PopOperandos(f->p);

	i8 long2 = ((i8)high_bytes2->topo->operando << 32) | low_bytes2->topo->operando;
	i4 retorno;
	if(long2 > long1){
		retorno = 1;
	} else if(long2 < long1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = PushOperandos(f->p, retorno, NULL,INTEGER_OP);
}

/**
 * - Instrução que compara valores float oriundos da pilha de operandos
 */
void fcmpl_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = PushOperandos(f->p, retorno, NULL,INTEGER_OP);
}

/**
 * - Instrução que compara valores float oriundos da pilha de operandos
 */
void fcmpg_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	float op1 = decodificaFloatValor(valor1->topo->operando);
	float op2 = decodificaFloatValor(valor2->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = PushOperandos(f->p, retorno, NULL,INTEGER_OP);

}

/**
 * - Instrução que compara valores double oriundos da pilha de operandos
 */
void dcmpl_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = PushOperandos(f->p, retorno, NULL,INTEGER_OP);

}

/**
 * - Instrução que compara valores double oriundos da pilha de operandos
 */
void dcmpg_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor1_low = PopOperandos(f->p);
	pilha_operandos *valor1_high = PopOperandos(f->p);
	pilha_operandos *valor2_low = PopOperandos(f->p);
	pilha_operandos *valor2_high = PopOperandos(f->p);

	double op1 = decodificaDoubleValor(valor1_high->topo->operando,valor1_low->topo->operando);
	double op2 = decodificaDoubleValor(valor2_high->topo->operando,valor2_low->topo->operando);
	i4 retorno;
	if(op2 > op1){
		retorno = 1;
	} else if(op2 < op1){
		retorno = -1;
	} else{
		retorno = 0;
	}

	f->p = PushOperandos(f->p, retorno, NULL,INTEGER_OP);

}

/**
 * - Instrução que compara valor da pilha de operandos com zero, verificando se são iguais
 */
void ifeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor = PopOperandos(f->p);

	if(valor->topo->operando == 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Instrução que compara valor da pilha de operandos com zero, verificando se são diferentes
 */
void ifne_impl(frame *f, u1 branchbyte1, u1 branchbyte2){

	pilha_operandos *valor = PopOperandos(f->p);

	if(valor->topo->operando != 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Instrução que compara valor da pilha de operandos com zero, verificando se o valor da pilha de operandos é menor que zero
 */
void iflt_impl(frame *f, u1 branchbyte1, u1 branchbyte2){

	pilha_operandos *valor = PopOperandos(f->p);

	if(valor->topo->operando<0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for maior ou igual ao valor zero
 */
void ifge_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor = PopOperandos(f->p);

	if(valor->topo->operando >= 0){
		uint8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for maior do que o valor zero
 */
void ifgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor = PopOperandos(f->p);

	if(valor->topo->operando > 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for menor do que o valor zero
 */
void ifle_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor = PopOperandos(f->p);

	if(valor->topo->operando <= 0){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for igual ao próximo inteiro na pilha
 */
void if_icmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor1->topo->operando == valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for diferente do próximo inteiro na pilha
 */
void if_icmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor1->topo->operando != valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 */
void if_icmplt_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor2->topo->operando < valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 */
void if_icmpge_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor2->topo->operando >= valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 */
void if_icmpgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor2->topo->operando > valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 */
void if_icmple_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor2->topo->operando <= valor1->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se as referências na pilha forem iguais
 */
void acmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor1->topo->operando == valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - Branch será efetuado se as referências na pilha não forem iguais
 */
void acmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor1 = PopOperandos(f->p);
	pilha_operandos *valor2 = PopOperandos(f->p);

	if(valor1->topo->operando != valor2->topo->operando){
		int8_t v1 = (int8_t)branchbyte1;
		int8_t v2 = (int8_t)branchbyte2;
		int16_t branchoffset = (v1 << 8) | v2;
		jvm->pc += branchoffset;
	}
}

/**
 * - A instrução sempre efetuará um branch
 */
void inst_goto_impl(frame *f,u1 branchbyte1, u1 branchbyte2){
	int8_t bb1 = (int8_t)branchbyte1;
	int8_t bb2 = (int8_t)branchbyte2;
	int16_t branchoffset = (bb1 << 8) | bb2;

	jvm->pc += branchoffset;
}

/**
 * - Instrução para efetuar um branch para um offset, dentro do código do mesmo método sendo executado
 */
void jsr_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	int8_t v1 = (int8_t)branchbyte1;
	int8_t v2 = (int8_t)branchbyte2;
	int16_t branchoffset = (v1 << 8) | v2;
	jvm->pc += branchoffset;
}

/**
 * - Instrução para retornar de uma subrotina
 */
void ret_impl(frame *f,u1 index, u1 par){
}

/**
 * - Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 */
void tableswitch_fantasma(frame *par0, u1 par1, u1 par2){
}

/**
 * - Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 */
void lookupswitch_fantasma(frame *par0, u1 par1, u1 par2){
}

/**
 * - Instrução para retornar um inteiro de um método
 */
void ireturn_impl(frame *f, u1 par1, u1 par2){

	pilha_operandos *valor = PopOperandos(f->p);

	jvm->frames->topo->prox->f->p = PushOperandos(jvm->frames->topo->prox->f->p,(i4)valor->topo->operando,NULL,valor->topo->tipo_operando);
	PopFrames(jvm->frames);
}

/**
 * - Instrução para retornar um long de um método
 */
void lreturn_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);

	jvm->frames->topo->prox->f->p = PushOperandos(jvm->frames->topo->prox->f->p,(u4)high_bytes->topo->operando,NULL,LONG_OP);
	jvm->frames->topo->prox->f->p = PushOperandos(jvm->frames->topo->prox->f->p,(u4)low_bytes->topo->operando,NULL,LONG_OP);
	PopFrames(jvm->frames);

}

/**
 * - Instrução para retornar um float de um método
 */
void freturn_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *valor = PopOperandos(f->p);
	jvm->frames->topo->prox->f->p = PushOperandos(jvm->frames->topo->prox->f->p,(u4)valor->topo->operando,NULL,FLOAT_OP);
	PopFrames(jvm->frames);
}

/**
 * - Instrução para retornar um double de um método
 */
void dreturn_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *low_bytes = PopOperandos(f->p);
	pilha_operandos *high_bytes = PopOperandos(f->p);
	jvm->frames->topo->prox->f->p = PushOperandos(jvm->frames->topo->prox->f->p,(u4)high_bytes->topo->operando,NULL,DOUBLE_OP);
	jvm->frames->topo->prox->f->p = PushOperandos(jvm->frames->topo->prox->f->p,(u4)low_bytes->topo->operando,NULL,DOUBLE_OP);
	PopFrames(jvm->frames);
}

/**
 * - Instrução para retornar uma referência de um método
 */
void areturn_impl(frame *f, u1 par1, u1 par2){
	PopOperandos(f->p);
}

/**
 * - Instrução para retornar void de um método
 */
void inst_return_impl(frame *f, u1 par1, u1 par2){
	PopFrames(jvm->frames);
}

/**
 * - Instrução para obter um field estático de uma classe
 */
void getstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2){

	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;

	cp_info *aux = f->cp-1+indice_cp;
	char *classedoField = decodificaNIeNT(f->cp,aux->UnionCP.Fieldref.class_index,NAME_INDEX);
	if(strcmp(classedoField,"java/lang/System")==0){
		f->p = PushOperandos(f->p,-INT_MAX,"out",REFERENCE_OP);
	} else{
		classesCarregadas * nova = BuscarElemento_classes(jvm->classes,classedoField);
		if (nova == NULL) {
			if (resolverClasse(classedoField) == NULL) {
				printf("Falha ao abrir classe com field estático, encerrando.\n");
				exit(1);
			}
		} else {
			cp_info * nameTypeField = f->cp-1+aux->UnionCP.Fieldref.name_and_type_index;


			char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);


			field_info * fieldSaida = BuscarFieldClasseCorrente_classes(jvm->classes, classedoField, nomeField);
			if (fieldSaida != NULL) {
				char * accessF = decodificaAccessFlags(fieldSaida->access_flags);

				if (buscaStaticFlags(accessF)) {
					char * descriptorFieldAux = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
					if (descriptorFieldValidate(descriptorFieldAux) < 7) {
						if (descriptorFieldValidate(descriptorFieldAux) == 1) {
							i4 valorPushed = *fieldSaida->dadosStatics->low;
							f->p = PushOperandos(f->p,valorPushed,NULL,FLOAT_OP);
						} else if (descriptorFieldValidate(descriptorFieldAux) == 5) {
							u4 * valorPushed = fieldSaida->dadosStatics->low;
							f->p = PushOperandos(f->p,-INT_MAX,(void*)valorPushed,REFERENCE_STRING_OP);
						} else {
							i4 valorPushed = *fieldSaida->dadosStatics->low;
							f->p = PushOperandos(f->p,valorPushed,NULL,INTEGER_OP);
						}
					} else if (descriptorFieldValidate(descriptorFieldAux) == 7 || descriptorFieldValidate(descriptorFieldAux) == 8) {
						i4 valorPushedLow = *fieldSaida->dadosStatics->low;
						i4 valorPushedHigh = *fieldSaida->dadosStatics->high;
						if (descriptorFieldValidate(descriptorFieldAux) == 7) {
							f->p = PushOperandos(f->p,valorPushedHigh,NULL,DOUBLE_OP);
							f->p = PushOperandos(f->p,valorPushedLow,NULL,DOUBLE_OP);
						} else {
							f->p = PushOperandos(f->p,valorPushedHigh,NULL,LONG_OP);
							f->p = PushOperandos(f->p,valorPushedLow,NULL,LONG_OP);
						}
					}
				}
			}
		}
	}
}

/**
 * - Instrução para setar um field estático em uma classe
 */
void putstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;

	cp_info * field = f->cp-1+indice_cp;
	char * nomeClasse = decodificaNIeNT(f->cp,field->UnionCP.Fieldref.class_index,NAME_INDEX);
	classesCarregadas * nova = BuscarElemento_classes(jvm->classes,nomeClasse);
	if (nova == NULL) {
		if (resolverClasse(nomeClasse) == NULL) {
			printf("Falha ao abrir classe com field estático, encerrando.\n");
			exit(1);
		}
	} else {
		cp_info * nameTypeField = f->cp-1+field->UnionCP.Fieldref.name_and_type_index;

		char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);


		field_info * fieldSaida = BuscarFieldClasseCorrente_classes(jvm->classes, nomeClasse, nomeField);
		if (fieldSaida != NULL) {
			char * accessF = decodificaAccessFlags(fieldSaida->access_flags);

			if (buscaStaticFlags(accessF)) {

				char * descriptorFieldAux = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
				if (descriptorFieldValidate(descriptorFieldAux) < 7) {
					pilha_operandos *valor = PopOperandos(f->p);
					fieldSaida->dadosStatics = (staticData*)malloc(sizeof(staticData));
					fieldSaida->dadosStatics->low = (u4*) malloc(sizeof(u4));
					fieldSaida->dadosStatics->high = NULL;
					if (valor->topo->tipo_operando == REFERENCE_OP) {
						fieldSaida->dadosStatics->low = (u4*)valor->topo->referencia;
					} else {
						*fieldSaida->dadosStatics->low = (u4)valor->topo->operando;
					}
				} else if (descriptorFieldValidate(descriptorFieldAux) == 7 || descriptorFieldValidate(descriptorFieldAux) == 8) {
					pilha_operandos *valorLow = PopOperandos(f->p);
					pilha_operandos *valorHigh = PopOperandos(f->p);
					fieldSaida->dadosStatics = (staticData*)malloc(sizeof(staticData));
					fieldSaida->dadosStatics->low = (u4*) malloc(sizeof(u4));
					fieldSaida->dadosStatics->high = (u4*) malloc(sizeof(u4));
					*fieldSaida->dadosStatics->low = (u4)valorLow->topo->operando;
					*fieldSaida->dadosStatics->high = (u4)valorHigh->topo->operando;
				}
			}
		}
	}
}

/**
 * - Funcao para verificar se um metodo é do tipo static
 */
bool buscaStaticFlags (char * accessFlags) {
	char * acc = (char*) malloc(100*sizeof(char));
	strcpy(acc,accessFlags);
	const char s[2] = " ";
  char * token;
  token = strtok(acc, s);
  while(token != NULL) {
  	if (strcmp(token,"STATIC") == 0) {
			return true;
		}
    token = strtok(NULL, s);
  }
	return false;
}

/**
 * - Instrução para obter o field de um objeto
 */
void getfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
    u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
    cp_info *aux = f->cp-1+indice_cp;
    char *classedoField = decodificaNIeNT(f->cp,aux->UnionCP.Fieldref.class_index,NAME_INDEX);
    if(strcmp(classedoField,"java/lang/System")==0){
        f->p = PushOperandos(f->p,-INT_MAX,"out",REFERENCE_OP);
    } else {
			cp_info * nameTypeField = f->cp-1+aux->UnionCP.Fieldref.name_and_type_index;
			char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);
			char * descriptorRetorno = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
			int tipoRetorno = getTipoRetorno(descriptorRetorno);
			if (tipoRetorno != -1) {
				pilha_operandos * objetoRef = PopOperandos(f->p);
				ClassFile * buscado = (ClassFile*)objetoRef->topo->referencia;
				Lista_Objetos * obj = buscaObjetoViaReferencia(buscado);
				if (obj != NULL) {
					int posicaoField = getPositionField(obj->obj, nomeField);
					if (posicaoField != -1) {
						if (tipoRetorno == 1) {
							u4 * valorLow = (u4*)malloc(sizeof(u4));
							*valorLow = obj->data[posicaoField];
							int tipoOperandoSaida = getTipoOperandoSaida(descriptorRetorno);
							f->p = PushOperandos(f->p,*valorLow,NULL,tipoOperandoSaida);
						} else {
							u4 * valorHigh = (u4*)malloc(sizeof(u4));
							*valorHigh = obj->data[posicaoField];
							u4 * valorLow = (u4*)malloc(sizeof(u4));
							*valorLow = obj->data[posicaoField+1];
							int tipoOperandoSaida = getTipoOperandoSaida(descriptorRetorno);
							f->p = PushOperandos(f->p,*valorHigh,NULL,tipoOperandoSaida);
							f->p = PushOperandos(f->p,*valorLow,NULL,tipoOperandoSaida);
						}
					} else {
						exit(1);
					}

				}
			}
		}
}

/**
 * - Função para obter o tipo de operando que sera alocado na pilha de operandos
 */
int getTipoOperandoSaida(char * descriptorRetorno) {
	switch (*descriptorRetorno) {
			case 'I':
				return INTEGER_OP;
			case 'F':
				return FLOAT_OP;
			case 'B':
				return BYTE_OP;
			case 'C':
				return CHAR_OP;
			case 'S':
				return SHORT_OP;
			case 'Z':
				return BOOLEAN_OP;
			case 'D':
				return DOUBLE_OP;
			case 'J':
				return LONG_OP;
			default:
				break;
	}
	return -1;
}

/**
 * - Função para obter o modo de empilhamento do valor de retorno de um método
 */
int getTipoRetorno (char * descriptor) {
	switch (*descriptor) {
			case 'I':
			case 'F':
			case 'B':
			case 'C':
			case 'S':
			case 'Z':
				return 1;
			case 'D':
			case 'J':
				return 2;
			default:
				break;
	}
	return -1;
}

/**
 * - Instrução para setar um field em um objeto
 */
void putfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
    u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
    cp_info * field = f->cp-1+indice_cp;
    char * nomeClasse = decodificaNIeNT(f->cp,field->UnionCP.Fieldref.class_index,NAME_INDEX);
    classesCarregadas * nova = BuscarElemento_classes(jvm->classes,nomeClasse);
    if (nova == NULL) {
        if (resolverClasse(nomeClasse) == NULL) {
            printf("Falha ao abrir classe com field estático, encerrando.\n");
            exit(1);
        }
    } else {
        cp_info * nameTypeField = f->cp-1+field->UnionCP.Fieldref.name_and_type_index;
        char * nomeField = decodificaNIeNT(f->cp,nameTypeField->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);
        pilha_operandos * valorPopedLow = PopOperandos(f->p);
				pilha_operandos * valorPopedHigh = NULL;
				if (valorPopedLow->topo->tipo_operando == DOUBLE_OP || valorPopedLow->topo->tipo_operando == LONG_OP) {
					valorPopedHigh = PopOperandos(f->p);
				}
				pilha_operandos * objetoRef = PopOperandos(f->p);
				ClassFile * buscado = (ClassFile*)objetoRef->topo->referencia;
				Lista_Objetos * obj = buscaObjetoViaReferencia(buscado);
				if (obj != NULL) {
					int posicaoField = getPositionField(obj->obj, nomeField);
					if (posicaoField != -1) {
						if (valorPopedHigh == NULL) {
							obj->data[posicaoField] = valorPopedLow->topo->operando;
						} else {
							obj->data[posicaoField] = valorPopedHigh->topo->operando;
							obj->data[posicaoField+1] = valorPopedLow->topo->operando;
						}
					} else {
						exit(1);
					}

				}
    }
}

/**
 * - Função para obter a posição do field no array de dados de instância
 */
int getPositionField (ClassFile * obj, char * nomeField) {
	ClassFile * aux = obj;
	int posicao = 0;
	for (field_info * auxF = aux->fields; auxF < aux->fields+aux->fields_count; auxF++) {
		char * nomeF = decodificaStringUTF8(aux->constant_pool-1+auxF->name_index);
		if (strcmp(nomeF,nomeField) == 0) {
			return posicao;
		} else {
			posicao++;
		}
	}
	return -1;
}

/**
 * - Função para buscar um objeto na lista de objetos
 */
Lista_Objetos * buscaObjetoViaReferencia (ClassFile * p) {
	Lista_Objetos * auxiliarObjetos;
	for (auxiliarObjetos = jvm->objetos; auxiliarObjetos != NULL; auxiliarObjetos = auxiliarObjetos->prox) {
		if (auxiliarObjetos->obj == p) {
			return auxiliarObjetos;
		}
	}
	return NULL;
}

/**
 * - Função para invocar um método de instância, geralmente utilizada para impressão em tela
 */
void invokevirtual_impl(frame *f, u1 indexbyte1, u1 indexbyte2){

	u4 * end;
    u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
	char *classeDoMetodo = NULL;
	int *parametros_cont = malloc(sizeof(int));

    char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,0);
    char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,0);
		char *descriptorcopia = malloc(strlen(descriptormetodo)*sizeof(char));
		strcpy(descriptorcopia,descriptormetodo);

    if(strcmp(nomemetodo,"println")==0){
        double valorSaida_double;
        float valorSaida_float;
        long valorSaida_long;

        if (!pilhaVazia(f->p)) {
            if (!printVazio(f->p)) {
                pilha_operandos *string = PopOperandos(f->p);
                pilha_operandos * v2;
                if (string->topo->tipo_operando == DOUBLE_OP || string->topo->tipo_operando == LONG_OP) {
                    v2 = PopOperandos(f->p);
                }
                PopOperandos(f->p);
                switch(string->topo->tipo_operando){

                    case BOOLEAN_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case BYTE_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case CHAR_OP:
                        printf("%c\n",(char)string->topo->operando);
                    break;
                    case SHORT_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case INTEGER_OP:
                        printf("%d\n",(i4)string->topo->operando);
                    break;
                    case FLOAT_OP:
                        valorSaida_float = decodificaFloatValor(string->topo->operando);
                        printf("%g\n",valorSaida_float);
                    break;
                    case LONG_OP:
                    	valorSaida_long = decodificaLongValor(v2->topo->operando,string->topo->operando);
                        printf("%ld\n",valorSaida_long);
                    break;
                    case DOUBLE_OP:
                        valorSaida_double = decodificaDoubleValor(v2->topo->operando, string->topo->operando);
                        printf("%g\n",valorSaida_double);
                    break;
                    case RETURN_ADDRESS_OP:
                        printf("[@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_BOOLEAN_OP:
                        printf("[Z@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_CHAR_OP:
                        printf("%s\n",(char*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_FLOAT_OP:
                        printf("[F@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_DOUBLE_OP:
                        printf("[D@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_BYTE_OP:
                        printf("[B@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_SHORT_OP:
                        printf("[S@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_INT_OP:
                        printf("[I@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_ARRAY_LONG_OP:
                        printf("[J@%p\n",(u4*) string->topo->referencia);
                    break;
                    case REFERENCE_OP:
                        printf("[@%p\n",(u4*) string->topo->referencia);
                    break;
					case REFERENCE_STRING_OP:
						end = (u4*)string->topo->referencia;
						printf("%s\n",(char*)(end));
					break;
                }

            } else {
                PopOperandos(f->p);
                printf("\n");
            }
        }
    }
    else{
					classeDoMetodo = obterClasseDoMetodo(f->cp,indice_cp);

					method_info * methodAux = BuscarMethodClasseCorrente_classes(jvm->classes,classeDoMetodo, nomemetodo);
					attribute_info *aux;
					int posicao;
					*parametros_cont = 1 + getParametrosCount (descriptormetodo);

					for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
						aux = (*(methodAux->attributes+posicao));
						classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeDoMetodo);
						char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
						if(strcmp(nameindex,"Code")==0){
							code_attribute *c = (code_attribute *) aux->info;
							frame *f_novo = criarFrame(classeDoMetodo,c->max_locals);
							f_novo = transferePilhaVetor(f,f_novo,parametros_cont);
							jvm->frames = PushFrames(jvm->frames,f_novo);

							classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeDoMetodo);
							method_info *metodos = classe->arquivoClass->methods;
							for(method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
								char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
								char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

								if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptorcopia,descriptorMetodoAux) == 0){
									jvm->pc +=3;
									f->end_retorno = jvm->pc;					
									jvm->pc = 0;
									executarMetodo(aux,classeDoMetodo,2);
									jvm->pc = f->end_retorno;

								}
							}
						}
					}


				}

}

/**
 * - Instrução para invocar um método de instância
 */
void invokespecial_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);

	char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,0);
	char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,0);
	char *descriptorcopia = malloc(strlen(descriptormetodo)*sizeof(char));
	strcpy(descriptorcopia,descriptormetodo);
	char *classeDoMetodo = NULL;
	char *classePaiDaCorrente = NULL;
	classesCarregadas *classeCorrente = NULL;
	int *parametros_cont = malloc(sizeof(int));

	if(resolverMetodo(f->cp,indice_cp,0)){

		classeDoMetodo = obterClasseDoMetodo(f->cp,indice_cp);

		classeCorrente = BuscarElemento_classes(jvm->classes,jvm->frames->topo->f->classeCorrente);

		classePaiDaCorrente = decodificaNIeNT(classeCorrente->arquivoClass->constant_pool,classeCorrente->arquivoClass->super_class,NAME_INDEX);
		if(isSuper(classeCorrente->arquivoClass->access_flags) && strcmp(classeDoMetodo,classePaiDaCorrente)==0 && (strcmp(nomemetodo,"init")!=0 || strcmp(nomemetodo,"clinit")!=0)){

		}
		else{
			method_info * methodAux = BuscarMethodClasseCorrente_classes(jvm->classes,classeDoMetodo, nomemetodo);
			attribute_info *aux;
			int posicao;
			*parametros_cont = 1 + getParametrosCount (descriptormetodo);

			for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
				aux = (*(methodAux->attributes+posicao));
				classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeDoMetodo);
				char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
				if(strcmp(nameindex,"Code")==0){
					code_attribute *c = (code_attribute *) aux->info;
					frame *f_novo = criarFrame(classeDoMetodo,c->max_locals);
					f_novo = transferePilhaVetor(f,f_novo,parametros_cont);
					jvm->frames = PushFrames(jvm->frames,f_novo);

					classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeDoMetodo);
					method_info *metodos = classe->arquivoClass->methods;
					for(method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
						char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
						char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

						if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptorcopia,descriptorMetodoAux) == 0){
							jvm->pc +=3;
							f->end_retorno = jvm->pc;
							jvm->pc = 0;
							executarMetodo(aux,classeDoMetodo,2);
							jvm->pc = f->end_retorno;

						}
					}
				}
			}

		}
	}
}

/**
 * - Função para obter a quantidade de parâmetros a serem passados para um método
 */
int getParametrosCount (char * descriptor) {
	int sizeChar = strlen(descriptor);
	int contador = 0;
	char * descriptorAuxiliar = descriptor;
	while (sizeChar > 0) {
		switch (*descriptorAuxiliar) {
			case '(':
				break;
			case ')':
				return contador;
				break;
			case 'I':
			case 'F':
			case 'B':
			case 'C':
			case 'S':
			case 'Z':
				contador++;
				break;
			case 'D':
			case 'J':
				contador += 2;
				break;
			default:
				break;
		}
		descriptorAuxiliar++;
		sizeChar--;
	}
	return contador;
}

/**
 * - Instrução para invocar um método estático (método de classe)
 */
void invokestatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2){

	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
	char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,0);
	char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,0);
	char *descriptorcopia = malloc(strlen(descriptormetodo)*sizeof(char));
	strcpy(descriptorcopia,descriptormetodo);
	int *parametros_cont = malloc(sizeof(int));
	*parametros_cont = 0;

	char *pch = strtok(descriptormetodo,"()");
	*parametros_cont += strlen(pch);

	if(resolverMetodo(f->cp,indice_cp,0)){
		char *classeNova = obterClasseDoMetodo(f->cp,indice_cp);
		method_info * methodAux = BuscarMethodClasseCorrente_classes(jvm->classes, classeNova, nomemetodo);
		attribute_info *aux;
		int posicao;
		for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
			aux = (*(methodAux->attributes+posicao));
			classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeNova);
			char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
			if(strcmp(nameindex,"Code")==0){
				code_attribute *c = (code_attribute *) aux->info;
				frame *f_novo = criarFrame(classeNova,c->max_locals);

				f_novo = transferePilhaVetor(f,f_novo,parametros_cont);
				jvm->frames = PushFrames(jvm->frames,f_novo);

				classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeNova);
				method_info *metodos = classe->arquivoClass->methods;
				for(method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
					char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
					char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

					if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptorcopia,descriptorMetodoAux) == 0){
						jvm->pc +=3;
						f->end_retorno = jvm->pc;
						jvm->pc = 0;
						executarMetodo(aux,classeNova,2);
						jvm->pc = f->end_retorno;

					}
				}
			}
		}
	}
}

/**
 * - Instrução para invocar um método de interface
 */
void invokeinterface_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 count){
	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);

	char *nomemetodo = obterNomeMetodo(f->cp,indice_cp,1);
	char *descriptormetodo = obterDescriptorMetodo(f->cp,indice_cp,1);

	if(resolverMetodo(f->cp,indice_cp,1)){
		char *classeNova = malloc(100*sizeof(char));
		strcpy(classeNova,jvm->frames->topo->f->classeCorrente);
		method_info *methodAux = BuscarMethodClasseCorrente_classes(jvm->classes, classeNova, nomemetodo);
		attribute_info *aux;
		int posicao;
		for(posicao = 0; posicao < methodAux->attributes_count; posicao++) {
			aux = (*(methodAux->attributes+posicao));
			classesCarregadas *classeAtual = BuscarElemento_classes(jvm->classes,classeNova);
			char *nameindex = decodificaStringUTF8(classeAtual->arquivoClass->constant_pool-1+aux->attribute_name_index);
			if(strcmp(nameindex,"Code")==0){
				code_attribute *c = (code_attribute *) aux->info;
				frame *f_novo = criarFrame(classeNova,c->max_locals);
				f_novo = transferePilhaVetorCount(f,f_novo,count);
				jvm->frames = PushFrames(jvm->frames,f_novo);

				classesCarregadas *classe = BuscarElemento_classes(jvm->classes,classeNova);
				method_info *metodos = classe->arquivoClass->methods;
				for(method_info *aux=metodos;aux<metodos+classe->arquivoClass->methods_count;aux++){
					char * nomeMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->name_index);
					char * descriptorMetodoAux = decodificaStringUTF8(classe->arquivoClass->constant_pool-1+aux->descriptor_index);

					if(strcmp(nomemetodo,nomeMetodoAux) == 0 && strcmp(descriptormetodo,descriptorMetodoAux) == 0){
						executarMetodo(aux,classeNova,2);
					}
				}
			}
		}
	}
}

/**
 * - Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 */
void invokeinterface_fantasma(frame *par0, u1 par1, u1 par2){

}

/**
 * - Instrução para invocar um método dinâmico. Essa instrução só existe para criar uma posição "falsa" no vetor de ponteiros de função
 */
void invokedynamic_fantasma(frame *par0, u1 par1, u1 par2){

}

/**
 * - Instrução para criar um novo objeto
 */
void inst_new_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
	u2 indice_cp = (indexbyte1 << 8) | indexbyte2;
	char *nomeClasse = decodificaNIeNT(f->cp,indice_cp,NAME_INDEX);
	ClassFile *classe = instanciarClasse(nomeClasse);
	int numFields = getParametrosNaoStatic(classe);
	jvm->objetos = InsereObjeto(jvm->objetos, classe,numFields);
	f->p = PushOperandos(f->p,-INT_MAX,classe,REFERENCE_OP);
}

/**
 * - Função para instanciar um novo objeto
 */
ClassFile * instanciarClasse (char * nomeClasse) {
	char * saida = (char*)malloc((strlen(nomeClasse)+8)*sizeof(char));
	strcpy(saida,nomeClasse);
	strcat(saida,".class");
	ClassFile * cf = lerArquivo(saida);
	return cf;
}

/**
 * - Funcao para inserir um objeto novo na lista de objetos
 */
Lista_Objetos * InsereObjeto (Lista_Objetos * lis, ClassFile * classe, int parametrosNaoStatic) {
	Lista_Objetos * objeto = (Lista_Objetos*)malloc(sizeof(Lista_Objetos));
	objeto->obj = classe;
	objeto->sizeData = parametrosNaoStatic;
	objeto->data = (u4*)malloc(parametrosNaoStatic*sizeof(u4));
	objeto->prox = NULL;
	objeto->ant = NULL;

	Lista_Objetos * aux;
	Lista_Objetos * ultimo;
	if (lis != NULL) {
		aux = lis;
		while (aux != NULL) {
			ultimo = aux;
			aux = aux->prox;
		}
		ultimo->prox = objeto;
		objeto->ant = ultimo;
		return lis;
	} else {
		return objeto;
	}
}

/**
 * - Funcao para obter a quantidade de campos da classe que nao sao estaticos
 */
int getParametrosNaoStatic (ClassFile * classe) {
	ClassFile * classeAux = classe;
	int contador = 0;
	if (classeAux->fields_count > 0) {
		for (field_info * fieldAux = classeAux->fields; fieldAux < classeAux->fields+classeAux->fields_count; fieldAux++) {
			if (fieldAux->access_flags != 0x0008) {
				contador++;
			}
		}
	}
	return contador;
}

/**
 * - Função para criar um novo array de um dado tipo
 */
void newarray_impl(frame *f, u1 atype, u1 par1){
	pilha_operandos *count = PopOperandos(f->p);
	i4 countnum = count->topo->operando;
	int ref_size = 0;
	void *endereco = NULL;
	int i = 0;
	if(count<0){
	}
	else{
		switch(atype){
			case T_BOOLEAN:
				endereco = (u1*) malloc((countnum+1)*sizeof(u1));
				ref_size = sizeof(u1);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(u1 *)p = CHAR_MAX;
					}
					else{
						*(u1 *)p=0;
					}
				}
			break;

			case T_CHAR:
				endereco = (i2*) malloc((countnum+1)*sizeof(i2));
				ref_size = sizeof(i2);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(i2 *)p = SHRT_MAX;
					}
					else{
						*(i2 *)p=0;
					}
				}
			break;

			case T_FLOAT:
				endereco = (u4*) malloc((countnum+1)*sizeof(u4));
				ref_size = sizeof(u4);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(u4 *)p = UINT32_MAX;
					}
					else{
						*(u4 *)p=0;
					}
				}
			break;

			case T_DOUBLE:
				endereco = (u4*) malloc((2*(countnum)+1)*sizeof(u4));
				ref_size = 2*sizeof(u4);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(u4 *)p = UINT32_MAX;
					}
					else{
						*(u4 *)p=0;
					}
				}
				break;

			case T_BYTE:
				endereco = (i1*) malloc((countnum+1)*sizeof(i1));
				ref_size = sizeof(i1);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(i1 *)p = CHAR_MAX;
					}
					else{
						*(i1 *)p=0;
					}
				}
			break;

			case T_SHORT:
				endereco = (i2*) malloc((countnum+1)*sizeof(i2));
				ref_size = sizeof(i2);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(i2 *)p = SHRT_MAX;
					}
					else{
						*(i2 *)p=0;
					}
				}
			break;

			case T_INT:
				endereco = (i4*) malloc((countnum+1)*sizeof(i4));
				ref_size = sizeof(i4);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i==countnum){
						*(i4 *)p = INT_MAX;
					}
					else{
						*(i4 *)p=0;
					}
				}
			break;

			case T_LONG:
				endereco = (i4*) malloc((2*(countnum)+1)*sizeof(i4));
				ref_size = 2*sizeof(i4);
				for(void *p=endereco;i<=countnum;i++,p+=ref_size){
					if(i == countnum){
						*(i4 *)p = INT32_MAX;
					}
					else{
						*(i4 *)p=0;
					}
				}
			break;
		}

		f->p = PushOperandos(f->p,-INT_MAX,endereco,atype + 7);
	}
}

/**
 * - Criar um novo array do tipo referência
 */
void anewarray_impl(frame *f, u1 par1, u1 par2){
}

/**
 * - Instrução para obter o tamanho de um array
 */
void arraylength_impl(frame *f, u1 par1, u1 par2){
	pilha_operandos *array_ref = PopOperandos(f->p);
	void *referencia = array_ref->topo->referencia;
	u1 tipo_referencia = array_ref->topo->tipo_operando;

	int tamanho = 0;
	int ref_size = 0;
	switch(tipo_referencia){
		case REFERENCE_ARRAY_BOOLEAN_OP:
			ref_size = sizeof(u1);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(u1 *)p == CHAR_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_CHAR_OP:
				ref_size = sizeof(i2);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i2 *)p == SHRT_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_FLOAT_OP:
			ref_size = sizeof(u4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(u4 *)p == UINT32_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_DOUBLE_OP:
			ref_size = 2*sizeof(u4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(u4 *)p == UINT32_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_BYTE_OP:
			ref_size = sizeof(i1);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i1 *)p == CHAR_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_SHORT_OP:
			ref_size = sizeof(i2);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i2 *)p == SHRT_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_INT_OP:
			ref_size = sizeof(i4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i4 *)p == INT_MAX){
					break;
				}
			}
		break;
		case REFERENCE_ARRAY_LONG_OP:
			ref_size = 2*sizeof(i4);
			for (void *p = referencia;;p+=ref_size,tamanho++){
				if (*(i4 *)p == INT32_MAX){
					break;
				}
			}
		break;
	}

	f->p = PushOperandos(f->p,tamanho,NULL,INTEGER_OP);

}

/**
 * - Instrução para lançar uma exceção ou erro
 */
void athrow_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Instrução para verificar se um objeto é de um determinado tipo
 */
void checkcast_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Instrução para determinar se um objeto é de um eterminado tipo
 */
void instanceof_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Instrução para entrar no monitor de um objeto
 */
void monitorenter_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Instrução para sair de um monitor de um objeto
 */
void monitorexit_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Instrução para estender a próxima instrução para ter o dobro de bytes de índice. 
 */
void wide_impl(frame *f, u1 indexbyte1, u1 indexbyte2){
	return;
}

/**
 * - Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 */
void multianewarray_fantasma(frame *f, u1 par1, u1 par2){

}

void* inicializa_multiarray_recursivo(void* endereco, i4* counts, char* tipos){
	switch(tipos[1]){
		case '[':{
			endereco = (void**)malloc(counts[0]*sizeof(void*));
			void ** endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux = inicializa_multiarray_recursivo(endereco_aux, &counts[1], &tipos[1]);
				endereco_aux++;
			}
			break;
		}
		case 'I':{
			endereco = (i4*)malloc(counts[0]*sizeof(i4));
			i4* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'B':{
			endereco = (i1*)malloc(counts[0]*sizeof(i1));
			i1* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'S':{
			endereco = (i2*)malloc(counts[0]*sizeof(i2));
			i2* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'J':{//LONG
			endereco = (i8*)malloc(counts[0]*sizeof(i8));
			i8* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'F':{
			endereco = (float*)malloc(counts[0]*sizeof(float));
			float* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		case 'D':{
			endereco = (double*)malloc(counts[0]*sizeof(double));
			double* endereco_aux = endereco;
			for(u4 i = 0; i < counts[0]; i++){
				*endereco_aux=0;
				endereco_aux++;
			}
			break;
		}
		default:
		break;
	}
	return endereco;
}

/**
 * - Instrução que irá criar um array multidimensional
 */
void multianewarray_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions){

	u2 indice_cp = normaliza_indice(indexbyte1,indexbyte2);
	cp_info *item = f->cp-1 + indice_cp;
	char* tipos = decodificaStringUTF8(f->cp-1+item->UnionCP.Class.name_index);
	i4* counts = (i4*)malloc(dimensions*sizeof(i4));
	void *endereco = NULL;
	for(u1 i = dimensions; i > 0; i--){
		pilha_operandos *valor = PopOperandos(f->p);
		counts[i-1] = valor->topo->operando;
		if(counts[i] < 0){
		}
	}

	endereco = inicializa_multiarray_recursivo(endereco, counts, tipos);

	f->p = PushOperandos(f->p,-INT_MAX,endereco,REFERENCE_OP);
	free(counts);

}

/**
 * - Instrução que irá efetuar um branch caso a referência no topo da pilha seja nula
 */
void ifnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor = PopOperandos(f->p);

	i2 offset = 0;

	if (valor->topo->referencia == NULL) {
		i1 v1 = (i1)branchbyte1;
		i1 v2 = (i1)branchbyte2;
		offset = (v1 << 8) | v2;
		jvm->pc += offset;
	}
}

/**
 * - Instrução que irá efetuar um branch caso a referência no topo da pilha não seja nula
 */
void ifnonnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2){
	pilha_operandos *valor = PopOperandos(f->p);

	i2 offset = 0;

	if(valor->topo->referencia != NULL) {
		i1 v1 = (i1)branchbyte1;
		i1 v2 = (i1)branchbyte2;
		offset = (v1 << 8) | v2;
		jvm->pc += offset;
	}
}

/**
 * - A instrução sempre efetuará um branch, para um offset wide
 */
void goto_w_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Instrução para efetuar um branch para um offset wide, de 32 bits, dentro do código do mesmo método sendo executado
 */
void jsr_w_impl(frame *f, u1 par1, u1 par2){

}

/**
 * - Funcao para decodificar operandos em um valor double (64 bits)
 */
double decodificaDoubleValor (u4 high, u4 low) {
	u8 valor = ((u8)(high)<<32) | (u8)low;

	double retorno = (double)(*(double*)&valor);
	return retorno;
}

/**
 * - Funcao para decodificar operando em valor long (64 bits)
 */
long decodificaLongValor (u4 high, u4 low) {
	u8 valor = ((u8)(high)<<32) | (u8)low;
	long retorno = (long)(*(long*)&valor);
	return retorno;
}

/**
 * - Funcao para decodificar operando em valor float (32 bits)
 */
float decodificaFloatValor (u4 valor) {
	float retorno = (float)(*(float*)&valor);
	return retorno;
}
