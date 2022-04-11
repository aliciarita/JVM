#include <stdio.h>
#include "leitor.h"
#include <string.h>
#include <math.h>
#include "instrucoes.h"


u1 u1Read(FILE *fp){
u1 destino;
	if (fread(&destino,sizeof(u1),1,fp) != 1) {
		return MAXIMO_U1;
	} else {
		return destino;
	}
}

u2 u2Read(FILE *fp){
u2 destino;
u1 lowByte, highByte;
highByte = u1Read(fp);
lowByte = u1Read(fp);

if(highByte != MAXIMO_U1 && lowByte != MAXIMO_U1) {
		destino = ((highByte)<<8) | ((lowByte));
		return destino;
	} else {
		return MAXIMO_U2;
	}
}

u4 u4Read(FILE *fp){
	u4 destino;
	u1 byteZero, byteUm, byteDois, byteTres;
	byteTres = u1Read(fp);
	byteDois = u1Read(fp);
	byteUm = u1Read(fp);
	byteZero = u1Read(fp);
	if(byteTres != MAXIMO_U1 && byteDois != MAXIMO_U1 && byteUm != MAXIMO_U1 && byteZero != MAXIMO_U1){
		destino = ((byteTres)<<24) | ((byteDois)<<16) | ((byteUm)<<8) | ((byteZero));
	return destino;
	} else {
		return MAXIMO_U4;
	}
}
ClassFile* readFile (char * filename) {
	ClassFile *classfile = NULL;
	FILE * fp = fopen(filename,"rb");
	if (fp == NULL) {
		printf("Erro ao abrir o arquivo.\n");
		return NULL;
	} else {
		classfile = (ClassFile*) malloc(sizeof(ClassFile));

		classfile->magic = u4Read(fp);
		classfile->minor_version = u2Read(fp);
		classfile->major_version = u2Read(fp);
		classfile->constant_pool_count = u2Read(fp);
		classfile->constant_pool = NULL;
		classfile->constant_pool = readConstantPool(fp, classfile->constant_pool_count);

		classfile->access_flags = u2Read(fp);
		classfile->this_class = u2Read(fp);
		classfile->super_class = u2Read(fp);
		classfile->interfaces_count = u2Read(fp);

		if (classfile->interfaces_count > 0) {
			classfile->interfaces = readInterfaces(fp, classfile->interfaces_count);
		}

		classfile->fields_count = u2Read(fp);
		if (classfile->fields_count > 0) {			
			classfile->fields = readField(fp, classfile->fields_count, classfile->constant_pool);
		}
		classfile->methods_count = u2Read(fp);

		if (classfile->methods_count > 0) {
			classfile->methods = readMethod(fp, classfile->methods_count,classfile->constant_pool);
		}
		classfile->attributes_count = u2Read(fp);
		if(classfile->attributes_count > 0){
			classfile->attributes = (attribute_info**)malloc(classfile->attributes_count*sizeof(attribute_info*));
			for (int posicao = 0; posicao < classfile->attributes_count; posicao++) {
				*(classfile->attributes+posicao) = readAttributes(fp, classfile->constant_pool);
			}
		}
		fclose(fp);

		return classfile;
	}
}

cp_info * readConstantPool (FILE * fp, u2 constant_pool_count) {
	cp_info * readConstantPool = (cp_info *) malloc((constant_pool_count-1)*sizeof(cp_info));
	cp_info * aux = NULL;
	for (aux = readConstantPool; aux < readConstantPool+constant_pool_count-1; aux++){
		aux->tag = u1Read(fp);
		switch(aux->tag) {
			case VALOR_CONSTANT_Class:
				aux->UnionCP.CONSTANT_Class.name_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_Fieldref:
				aux->UnionCP.CONSTANT_Fieldref.class_index = u2Read(fp);
				aux->UnionCP.CONSTANT_Fieldref.name_and_type_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_Methodref:
				aux->UnionCP.CONSTANT_Methodref.class_index = u2Read(fp);
				aux->UnionCP.CONSTANT_Methodref.name_and_type_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_InterfaceMethodref:
				aux->UnionCP.CONSTANT_InterfaceMethodref.class_index = u2Read(fp);
				aux->UnionCP.CONSTANT_InterfaceMethodref.name_and_type_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_String:
				aux->UnionCP.CONSTANT_String.string_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_Integer:
				aux->UnionCP.CONSTANT_Integer.bytes = u4Read(fp);
				break;
			case VALOR_CONSTANT_Float:
				aux->UnionCP.CONSTANT_Float.bytes = u4Read(fp);
				break;
			case VALOR_CONSTANT_Long:
				aux->UnionCP.CONSTANT_Long.high_bytes = u4Read(fp);
				aux->UnionCP.CONSTANT_Long.low_bytes = u4Read(fp);
				aux++;
				break;
			case VALOR_CONSTANT_Double:
				aux->UnionCP.CONSTANT_Double.high_bytes = u4Read(fp);
				aux->UnionCP.CONSTANT_Double.low_bytes = u4Read(fp);
				aux++;
				break;
			case VALOR_CONSTANT_NameAndType:
				aux->UnionCP.CONSTANT_NameAndType.name_index = u2Read(fp);
				aux->UnionCP.CONSTANT_NameAndType.descriptor_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_Utf8:
				aux->UnionCP.CONSTANT_UTF8.length = u2Read(fp);
				aux->UnionCP.CONSTANT_UTF8.bytes = malloc(aux->UnionCP.CONSTANT_UTF8.length*sizeof(u1));
				for (u1 * i = aux->UnionCP.CONSTANT_UTF8.bytes; i <aux->UnionCP.CONSTANT_UTF8.bytes+aux->UnionCP.CONSTANT_UTF8.length; i++){
					*i = u1Read(fp);
				}
				break;
			case VALOR_CONSTANT_MethodHandle:
				aux->UnionCP.CONSTANT_MethodHandle.reference_kind = u1Read(fp);
				aux->UnionCP.CONSTANT_MethodHandle.reference_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_MethodType:
				aux->UnionCP.CONSTANT_MethodType.descriptor_index = u2Read(fp);
				break;
			case VALOR_CONSTANT_InvokeDynamic:
				aux->UnionCP.CONSTANT_InvokeDynamicInfo.bootstrap_method_attr_index = u2Read(fp);
				aux->UnionCP.CONSTANT_InvokeDynamicInfo.name_and_type_index = u2Read(fp);
				break;
			default:
				printf("Default\n");
				break;
		}
	}
	return readConstantPool;
}

u2 * readInterfaces (FILE * fp, u2 size) {
	u2 * interfaces = malloc(size*sizeof(u2));
	for (u2 * auxInterfaces = interfaces; auxInterfaces < interfaces+size; auxInterfaces++) {
		*auxInterfaces = u2Read(fp);
	}
	return interfaces;
}

field_info * readField (FILE * fp, u2 fields_count, cp_info * cp) {
	field_info * fields = (field_info*)malloc(fields_count*sizeof(field_info));
	for (field_info * i = fields; i < fields + fields_count; i++) {
		i->access_flags = u2Read(fp);
		i->name_index = u2Read(fp);
		i->descriptor_index = u2Read(fp);
		i->attributes_count = u2Read(fp);

		if (i->attributes_count > 0) {
			i->attributes = (attribute_info**)malloc(i->attributes_count*sizeof(attribute_info*));
			for (int posicao = 0; posicao < i->attributes_count; posicao++) {
				*(i->attributes+posicao) = readAttributes(fp, cp);
			}
		}
	}

	return fields;
}
method_info * readMethod (FILE * fp, u2 methods_count, cp_info *cp) {
	method_info * methods = (method_info*) malloc(methods_count*sizeof(method_info));

	for (method_info * i = methods; i < methods + methods_count; i++) {
		i->access_flags = u2Read(fp);
		i->name_index = u2Read(fp);
		i->descriptor_index = u2Read(fp);
		i->attributes_count = u2Read(fp);
		if (i->attributes_count > 0) {
			i->attributes = (attribute_info**)malloc(i->attributes_count*sizeof(attribute_info*));
			for (int posicao = 0; posicao < i->attributes_count; posicao++) {
				*(i->attributes+posicao) = readAttributes(fp, cp);
			}
		}
	}

	return methods;
}

char* decodificarCode(cp_info *cp, u2 sizeCP, u1 *code, u4 length,instrucao *instrucoes){
	u1 *aux;

	char *retorno = (char*)malloc(1000*sizeof(char));
	char *stringaux = (char*)malloc(100*sizeof(char));
	u2 *aux2;
	char *stringargs;
	char *stringdecod;
		strcpy(retorno,"");

	for(aux=code;aux<code+length;){
		int numarg = instrucoes[*aux].numarg;
		strcat(retorno,instrucoes[*aux].inst_nome);
		switch(numarg){
			case 0:
				strcat(retorno,"\n");
				aux++;
			break;
			case 1:
				strcat(retorno," #");
				sprintf(stringaux,"%d",*(++aux));
				strcat(retorno,stringaux);
				strcat(retorno," ");
				stringdecod = decodificarOperandoInstrucao(cp,*aux,sizeCP);
				strcat(retorno,stringdecod);
				strcat(retorno,"\n");
				aux++;
			break;

			case 2:
				aux2 = (u2*)malloc(sizeof(u2));
				*aux2 = *(++aux) << 8;
				*aux2 |= *(++aux);
			stringargs = decodificarOperandoInstrucao(cp,*aux2,sizeCP);
				strcat(retorno," #");
				sprintf(stringaux,"%d",(int)*aux2);
				strcat(retorno,stringaux);
				strcat(retorno," ");
				strcat(retorno,stringargs);
				strcat(retorno,"\n");
				aux++;
			break;

			default:
				strcat(retorno,"undefined");
				aux++;
			break;

		}
	}
	free(stringargs);
	free(aux2);
	return(retorno);
}

code_attribute * readCode (FILE * fp, cp_info *cp) {
	code_attribute * code_attributes = NULL;
	code_attributes = (code_attribute*) malloc(sizeof(code_attribute));
	code_attributes->max_stack = u2Read(fp);
	code_attributes->max_locals = u2Read(fp);
	code_attributes->code_length = u4Read(fp);
	if(code_attributes->code_length > 0) {
		code_attributes->code = malloc(code_attributes->code_length*sizeof(u1));
		for(u1 *p = code_attributes->code; p < code_attributes->code + code_attributes->code_length; p++){
			*p = u1Read(fp);
		}
	}

	code_attributes->exception_table_length = u2Read(fp);

	if(code_attributes->exception_table_length > 0){
		code_attributes->table = NULL;
		code_attributes->table = readExceptionTable(fp, code_attributes->exception_table_length);
	}
	code_attributes->attributes_count = u2Read(fp);
	
	if (code_attributes->attributes_count > 0){
		code_attributes->attributes = (attribute_info**)malloc(code_attributes->attributes_count*sizeof(attribute_info*));
		for (int posicao = 0; posicao < code_attributes->attributes_count; posicao++) {
			*(code_attributes->attributes+posicao) = readAttributes(fp, cp);
		}
	}

	return code_attributes;
}

line_number_table * readLineNumberTable(FILE * fp, cp_info * cp) {
	line_number_table * lnt = (line_number_table*)malloc(sizeof(line_number_table));
	lnt->line_number_table_length = u2Read(fp);
	if (lnt->line_number_table_length > 0) {
		lnt->info = (line_number_tableInfo*)malloc(lnt->line_number_table_length*sizeof(line_number_tableInfo));
		for (line_number_tableInfo * linfo = lnt->info; linfo < lnt->info+lnt->line_number_table_length; linfo++) {
			linfo->start_pc = u2Read(fp);
			linfo->line_number = u2Read(fp);
		}
	}
	return lnt;
}

exception_table * readExceptionTable (FILE * fp, u2 size) {
	exception_table * exception_tableAux = (exception_table*)malloc(size*sizeof(exception_table));
	for(exception_table * e = exception_tableAux; e < exception_tableAux + size; e++){
		e->start_pc = u2Read(fp);
		e->end_pc = u2Read(fp);
		e->handler_pc = u2Read(fp);
		e->catch_type = u2Read(fp);
	}
	return exception_tableAux;
}

attribute_info * readAttributes (FILE * fp, cp_info * cp) {
	attribute_info * attributes = (attribute_info*) malloc(sizeof(attribute_info));
	attributes->attribute_name_index = u2Read(fp);
	attributes->attribute_length = u4Read(fp);
	if (attributes->attribute_length > 0) {
			char * string_name_index;
					string_name_index = decodificaStringUTF8(cp+attributes->attribute_name_index-1);
			if(strcmp(string_name_index,"SourceFile") == 0){
				source_file_attribute * SourceFile = NULL;
				SourceFile = readSourceFile(fp);
				attributes->info = NULL;
				attributes->info = (source_file_attribute*) SourceFile;
			} else if (strcmp(string_name_index,"Code") == 0) {
				code_attribute * code_attr = NULL;
				code_attr = readCode(fp ,cp);
				attributes->info = (code_attribute*) code_attr;
			} else if (strcmp(string_name_index,"LineNumberTable") == 0) {
				line_number_table * lnt = NULL;
				lnt = readLineNumberTable(fp, cp);
				attributes->info = (line_number_table*)lnt;
			} else if (strcmp(string_name_index,"StackMapTable") == 0) {
				stackMapTable_attribute * stackMapTable = NULL;
				stackMapTable = readStackMapTable(fp);
				attributes->info = (stackMapTable_attribute*)stackMapTable;
			} else if (strcmp(string_name_index, "InnerClasses") == 0) {
				innerClasses_attribute * innerClasses = NULL;
				innerClasses = readInnerClasses(fp, cp);
				attributes->info = (innerClasses_attribute*)innerClasses;
			} else if (strcmp(string_name_index,"Signature") == 0) {
				signature_attribute * signatureR = NULL;
				signatureR = readSignature(fp);
				attributes->info = (signature_attribute*)signatureR;
			} else if (strcmp(string_name_index,"ConstantValue") == 0) {
				constantValue_attribute * constantV = NULL;
				constantV = readConstantValue(fp);
				attributes->info = (constantValue_attribute*)constantV;
			} else if (strcmp(string_name_index,"Exceptions") == 0) {
				exceptions_attribute * exceptions = NULL;
				exceptions = readExceptionsAttribute(fp);
				attributes->info = (exceptions_attribute*)exceptions;
			}
	}
	
	return attributes;
}

exceptions_attribute * readExceptionsAttribute (FILE * fp) {
	exceptions_attribute * exceptions = (exceptions_attribute*)malloc(sizeof(exceptions_attribute));
	exceptions->number_of_exceptions = u2Read(fp);
	exceptions->exception_index_table = NULL;
	if (exceptions->number_of_exceptions > 0) {
		exceptions->exception_index_table = (u2*)malloc(exceptions->number_of_exceptions*sizeof(u2));
		for (u2 * excpAux = exceptions->exception_index_table; excpAux < exceptions->exception_index_table + exceptions->number_of_exceptions; excpAux++) {
			*excpAux = u2Read(fp);
		}
	}
	return exceptions;
}

constantValue_attribute * readConstantValue (FILE * fp) {
	constantValue_attribute * cv = (constantValue_attribute*)malloc(sizeof(constantValue_attribute));
	cv->constantvalue_index = u2Read(fp);
	return cv;
}

signature_attribute * readSignature (FILE * fp) {
	signature_attribute * signature = (signature_attribute*)malloc(sizeof(signature_attribute));
	signature->signature_index = u2Read(fp);
	return signature;
}

innerClasses_attribute * readInnerClasses (FILE * fp, cp_info * cp) {
	innerClasses_attribute * innerClasses = (innerClasses_attribute*)malloc(sizeof(innerClasses_attribute));
	innerClasses->number_of_classes = u2Read(fp);
	if (innerClasses->number_of_classes > 0) {
		innerClasses->classes_vector = (classes**)malloc(innerClasses->number_of_classes*sizeof(classes*));
		for (int posicao = 0; posicao < innerClasses->number_of_classes; posicao++) {
			*(innerClasses->classes_vector+posicao) = readClasses(fp);
		}
	}
	return innerClasses;
}

classes * readClasses (FILE * fp) {
	classes * classeRetorno = (classes*)malloc(sizeof(classes));
	classeRetorno->inner_class_info_index = u2Read(fp);
	classeRetorno->outer_class_info_index = u2Read(fp);
	classeRetorno->inner_name_index = u2Read(fp);
	classeRetorno->inner_class_access_flags = u2Read(fp);

	return classeRetorno;
}

stackMapTable_attribute * readStackMapTable (FILE * fp) {
	stackMapTable_attribute * stackMapTable = (stackMapTable_attribute*)malloc(sizeof(stackMapTable_attribute));
	stackMapTable->number_of_entries = u2Read(fp);
	if (stackMapTable->number_of_entries > 0) {
		stackMapTable->entries = (stack_map_frame**)malloc(stackMapTable->number_of_entries*sizeof(stack_map_frame*));
		for (int posicao = 0; posicao < stackMapTable->number_of_entries; posicao++) {
			*(stackMapTable->entries+posicao) = readStackMapFrame(fp);
		}
	}
	return stackMapTable;
}

stack_map_frame * readStackMapFrame (FILE * fp) {
	stack_map_frame * StackMapFrame = (stack_map_frame*)malloc(sizeof(stack_map_frame));
	StackMapFrame->frame_type = u1Read(fp);
	if (StackMapFrame->frame_type >= 0 && StackMapFrame->frame_type <= 63) {
	} else if (StackMapFrame->frame_type >= 64 && StackMapFrame->frame_type <= 127) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack = (verification_type_info**)malloc(sizeof(verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack) = readVerificationTypeInfo(fp);
	} else if (StackMapFrame->frame_type == 247) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta = u2Read(fp);
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack = (verification_type_info**)malloc(sizeof(verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack) = readVerificationTypeInfo(fp);
	} else if (StackMapFrame->frame_type >= 248 && StackMapFrame->frame_type <= 250) {
		StackMapFrame->map_frame_type.chop_frame.offset_delta = u2Read(fp);
	} else if (StackMapFrame->frame_type == 251) {
		StackMapFrame->map_frame_type.same_frame_extended.offset_delta = u2Read(fp);
	} else if (StackMapFrame->frame_type >= 252 && StackMapFrame->frame_type <= 254) {
		StackMapFrame->map_frame_type.append_frame.offset_delta = u2Read(fp);
		u2 sizeMalloc = (StackMapFrame->frame_type-251);
		StackMapFrame->map_frame_type.append_frame.locals = (verification_type_info**)malloc(sizeMalloc*sizeof(verification_type_info*));
		for (int posicao = 0; posicao < sizeMalloc; posicao++) {
			*(StackMapFrame->map_frame_type.append_frame.locals+posicao) = readVerificationTypeInfo(fp);
		}
	} else if (StackMapFrame->frame_type == 255) {
		StackMapFrame->map_frame_type.full_frame.offset_delta = u2Read(fp);
		StackMapFrame->map_frame_type.full_frame.number_of_locals = u2Read(fp);
		if (StackMapFrame->map_frame_type.full_frame.number_of_locals > 0) {
			StackMapFrame->map_frame_type.full_frame.locals = (verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.number_of_locals*sizeof(verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.number_of_locals; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.locals+posicao) = readVerificationTypeInfo(fp);
				if ((*(StackMapFrame->map_frame_type.full_frame.locals+posicao))->tag == 7) {
				}
			}
		}
		StackMapFrame->map_frame_type.full_frame.number_of_stack_items = u2Read(fp);
		if (StackMapFrame->map_frame_type.full_frame.number_of_stack_items > 0) {
			StackMapFrame->map_frame_type.full_frame.stack = (verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.number_of_stack_items*sizeof(verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.number_of_stack_items; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.stack+posicao) = readVerificationTypeInfo(fp);
			}
		}
	}
	return StackMapFrame;
}

verification_type_info * readVerificationTypeInfo (FILE * fp) {
	verification_type_info * VTI = (verification_type_info*)malloc(sizeof(verification_type_info));
	VTI->tag = u1Read(fp);
	switch (VTI->tag) {
		case 7:
			VTI->type_info.object_variable_info.cpool_index = u2Read(fp);
			break;
		case 8:
			VTI->type_info.uninitialized_variable_info.offset = u2Read(fp);
			break;
		default:
			break;
	}

	return VTI;
}

source_file_attribute * readSourceFile (FILE * fp) {
	source_file_attribute * SourceFile = NULL;
	SourceFile = (source_file_attribute*)malloc(sizeof(source_file_attribute));
	SourceFile->source_file_index = u2Read(fp);
	return SourceFile;
}

char* buscaNomeTag(u1 tag){
	char *nometag = malloc(40*sizeof(char));
	switch(tag){
		case VALOR_CONSTANT_Class:
			strcpy(nometag,"CONSTANT_Class_Info");
			break;
		case VALOR_CONSTANT_Fieldref:
			strcpy(nometag,"CONSTANT_Fieldref_Info");
			break;
		case VALOR_CONSTANT_Methodref:
			strcpy(nometag,"CONSTANT_Methodref_Info");
			break;
		case VALOR_CONSTANT_InterfaceMethodref:
			strcpy(nometag,"CONSTANT_InterfaceMethodref_Info");
			break;
		case VALOR_CONSTANT_String:
			strcpy(nometag,"CONSTANT_String_Info");
			break;
		case VALOR_CONSTANT_Integer:
			strcpy(nometag,"CONSTANT_Integer_Info");
			break;
		case VALOR_CONSTANT_Float:
			strcpy(nometag,"CONSTANT_Float_Info");
			break;
		case VALOR_CONSTANT_Long:
			strcpy(nometag,"CONSTANT_Long_Info");
			break;
		case VALOR_CONSTANT_Double:
			strcpy(nometag,"CONSTANT_Double_Info");
			break;
		case VALOR_CONSTANT_NameAndType:
			strcpy(nometag,"CONSTANT_NameAndType_Info");
			break;
		case VALOR_CONSTANT_Utf8:
			strcpy(nometag,"CONSTANT_Utf8_Info");
			break;
		case VALOR_CONSTANT_MethodHandle:
			strcpy(nometag,"CONSTANT_MethodHandle_Info");
			break;
		case VALOR_CONSTANT_MethodType:
			strcpy(nometag,"CONSTANT_MethodType_Info");
			break;
		case VALOR_CONSTANT_InvokeDynamic:
			strcpy(nometag,"CONSTANT_InvokeDynamic_Info");
			break;
		default:
			return NULL;
			break;
	}

	return(nometag);
}

char* decodificaStringUTF8(cp_info *cp){
	char* retorno = malloc((cp->UnionCP.CONSTANT_UTF8.length+1)*sizeof(char));
	char *auxretorno = retorno;

	for (u1 *aux=cp->UnionCP.CONSTANT_UTF8.bytes;aux<cp->UnionCP.CONSTANT_UTF8.bytes+cp->UnionCP.CONSTANT_UTF8.length;aux++){
		*auxretorno = (char) *aux;
		auxretorno++;
	}

	*auxretorno = '\0';

	return(retorno);
}

char* decodificarOperandoInstrucao(cp_info *cp,u2 index, u2 sizeCP){

	char *retorno = malloc(100*sizeof(char));
	char *stringNomeClasse;
	char *stringNomeMetodo;
	char *stringGeral;
	char *ponteiro2pontos;
	cp_info *cp_aux = cp+index-1;


	if (index < sizeCP) {
		switch(cp_aux->tag){
			case VALOR_CONSTANT_Methodref:

				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.CONSTANT_Methodref.class_index,1);

				stringNomeMetodo = decodificaNIeNT(cp,cp_aux->UnionCP.CONSTANT_Methodref.name_and_type_index,2);

				ponteiro2pontos = strchr(stringNomeMetodo,':');
				*ponteiro2pontos = '\0';


				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringNomeMetodo);
				strcat(retorno,">");
			break;

			case VALOR_CONSTANT_Fieldref:

				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.CONSTANT_Fieldref.class_index,1);
				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.CONSTANT_Fieldref.name_and_type_index,2);
				
				ponteiro2pontos = strchr(stringGeral,':');
				*ponteiro2pontos = '\0';
				
				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case VALOR_CONSTANT_String:

				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.CONSTANT_String.string_index,3);

				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;
			default:
				strcpy(retorno,"undefined");
				break;
		}
	} else {
		sprintf(retorno,"%d",index);
	}
	return(retorno);
}

char* decodificaNIeNT(cp_info *cp, u2 index, u1 tipo){

	char *retorno = malloc(100*sizeof(u1));

	cp_info *aux;
	cp_info *aux2;
	cp_info *aux3;

// cp_info começa em 1, por isso subtraimos 1 na soma do cp com o index
	aux = cp+index-1;

	switch(tipo){
		case 1:

			aux2 = cp+(aux->UnionCP.CONSTANT_Class.name_index-1);

			retorno = decodificaStringUTF8(aux2);
		break;

		case 2:

			aux2 = cp+(aux->UnionCP.CONSTANT_NameAndType.name_index-1);
			aux3 = cp+(aux->UnionCP.CONSTANT_NameAndType.descriptor_index-1);

			strcpy(retorno,"");
			strcat(retorno,decodificaStringUTF8(aux2));
			strcat(retorno,":");
			strcat(retorno,decodificaStringUTF8(aux3));
		break;

		case 3:
		case 4:
			retorno = decodificaStringUTF8(aux);
		break;

		case 5:
			retorno = decodificaStringUTF8(aux);
		break;

		case 6:
			retorno = decodificaStringUTF8(aux);
		break;
	}

	return(retorno);
}

char* decodificaAccessFlags(u2 flag){

	char *retorno = malloc(100*sizeof(char));
	strcpy(retorno,"");

	while(flag){

		if(flag>=SYNTHETIC){
			flag-=SYNTHETIC;
			strcat(retorno,"SYNTHETIC;");
		}
		if(flag>=TRANSIENT){
			flag-=TRANSIENT;
			strcat(retorno,"TRANSIENT;");
		}
		if(flag>=VOLATILE){
			flag-=VOLATILE;
			strcat(retorno,"VOLATILE;");
		}

		if(flag>=FINAL){
			flag-=FINAL;
			strcat(retorno,"FINAL;");
		}

		if(flag>=STATIC){
			flag-=STATIC;
			strcat(retorno,"STATIC;");
		}

		if(flag>=PROTECTED){
			flag-=PROTECTED;
			strcat(retorno,"PROTECTED;");
		}

		if(flag>=PRIVATE){
			flag-=PRIVATE;
			strcat(retorno,"PRIVATE;");
		}

		if(flag>=PUBLIC){
			flag-=PUBLIC;
			strcat(retorno,"PUBLIC;");
		}
	}

	retorno = organizandoFlags(retorno);
	return(retorno);

}

char* organizandoFlags(char* flagsOrdemInversa){

	const char s[2] = ";"; 

	char* flags = strtok(flagsOrdemInversa,s);
	char* velho = (char*)malloc(100*sizeof(char));
	char* novo = (char*)malloc(100*sizeof(char));

	int contador = 0;

	strcpy(novo, "");
	strcpy(velho, "");

	while(flags != NULL){

		if(contador == 0)
			strcpy(velho, flags);

		if(contador > 0){

			strcpy(novo, flags);
			strcat(novo," ");
			strcat(novo, velho);
			strcpy(velho, novo);
			contador++;
		}
		else
			contador++;

		flags = strtok(NULL, s);
	}

	if(contador == 1){
		free(novo);
		return velho;
	}
	else{
		free(velho);
		return novo;
	}
}

double decodificaDoubleInfo (cp_info * cp) {
	 uint64_t valor = ((uint64_t)cp->UnionCP.CONSTANT_Double.high_bytes<<32) | (uint64_t)cp->UnionCP.CONSTANT_Double.low_bytes;
	 int sinal = ((valor>>63) == 0) ? 1 : -1;
	 int expon = ((valor>>52) & 0x7ffL);
	 long mant = (expon == 0) ? ((valor & 0xfffffffffffffL) << 1) : ((valor & 0xfffffffffffffL) | 0x10000000000000L);

	double retorno = sinal*mant*(pow(2,expon-1075));
	return retorno;
}

uint64_t decodificaLongInfo (cp_info * cp) {
	return ((((uint64_t)cp->UnionCP.CONSTANT_Long.high_bytes)<<32) | ((uint64_t)cp->UnionCP.CONSTANT_Long.low_bytes));
}

int decodificaIntegerInfo (cp_info * cp) {

	u4 valor = cp->UnionCP.CONSTANT_Integer.bytes;
	return valor;
}

float decodificaFloatInfo (cp_info * cp) {
	u4 valor = cp->UnionCP.CONSTANT_Float.bytes;
	float retorno;
	int sinal = ((valor>>31) == 0) ? 1 : -1;
	int expon = ((valor>>23) & 0xff);
	int mant = (expon == 0) ? (valor & 0x7fffff)<<1 : (valor & 0x7fffff) | 0x800000;
	retorno = (sinal)*(mant)*(pow(2,expon-150));
	return retorno;
}

void imprimirClassFile (ClassFile * classfile, FILE* fp) {
	if(fp == NULL)
		return;
	cp_info * aux;
	double valor;
	uint64_t longValue;
	method_info * auxMethod;
	field_info * auxField;
	attribute_info ** auxAttributeClasse;
	attribute_info ** fieldAttrAux;
	exception_table * exceptionTableAux;
	uint32_t contador = 1;
	// u1 * auxBytesCode;
	char *ponteiroprint;

	instrucao *instrucoes = construirInstrucoes();

	fprintf(fp, "\n************************ General Information ************************\n\n");
//	fprintf(fp, "Magic: %08x\n",classfile->magic);
	fprintf(fp, "Minor Version: 		%d\n",classfile->minor_version);
	fprintf(fp, "Major Version: 		%d\n",classfile->major_version);
	fprintf(fp, "Constant Pool Count: 	%d\n",classfile->constant_pool_count);
	fprintf(fp, "Access Flags: 		0x%04x\n",classfile->access_flags);
	fprintf(fp, "This Class: 		cp_info#%d\n",classfile->this_class);
	fprintf(fp, "Super Class: 		cp_info#%d\n",classfile->super_class);
	fprintf(fp, "Interfaces Count: 	%d\n",classfile->interfaces_count);
	fprintf(fp, "Fields Count: 		%d\n",classfile->fields_count);
	fprintf(fp, "Methods Count: 		%d\n",classfile->methods_count);
	fprintf(fp, "Atributes Count: 	%d\n",classfile->attributes_count);


	fprintf(fp, "\n\n************************ Constant Pool ************************\n\n");

	for (aux = classfile->constant_pool; aux < classfile->constant_pool+classfile->constant_pool_count-1; aux++) {
		fprintf(fp, ">>> [%02d] %s\n",contador,buscaNomeTag(aux->tag));
		contador++;
		switch(aux->tag) {
			case VALOR_CONSTANT_Class:
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_Class.name_index,4);
				fprintf(fp, "Class Name:	cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_Class.name_index,ponteiroprint);
				break;
			case VALOR_CONSTANT_Fieldref:
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_Fieldref.class_index,1);
				fprintf(fp, "Class Name:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_Fieldref.class_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_Fieldref.name_and_type_index,2);
				fprintf(fp, "Name and Type:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_Fieldref.name_and_type_index,ponteiroprint);
				break;
			case VALOR_CONSTANT_Methodref:
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_Methodref.class_index,1);
				fprintf(fp, "Class Name:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_Methodref.class_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_Methodref.name_and_type_index,2);
				fprintf(fp, "Name and Type:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_Methodref.name_and_type_index,ponteiroprint);
				break;
			case VALOR_CONSTANT_InterfaceMethodref:
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_String.string_index,1);
				fprintf(fp, "Class Name:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_InterfaceMethodref.class_index, ponteiroprint);
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_String.string_index,2);
				fprintf(fp, "Name and Type Index:	cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_InterfaceMethodref.name_and_type_index, ponteiroprint);
				break;
			case VALOR_CONSTANT_String:
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_String.string_index,3);
				fprintf(fp, "String:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_String.string_index,ponteiroprint);
				break;
			case VALOR_CONSTANT_Integer:
				fprintf(fp, "Bytes:		%04x\n",aux->UnionCP.CONSTANT_Integer.bytes);
				fprintf(fp,"Integer:		%d\n",aux->UnionCP.CONSTANT_Integer.bytes);
				break;
			case VALOR_CONSTANT_Float:
				fprintf(fp, "Bytes:		%04x\n",aux->UnionCP.CONSTANT_Float.bytes);
				fprintf(fp, "Float:		%d\n",aux->UnionCP.CONSTANT_Float.bytes);
				break;
			case VALOR_CONSTANT_Long:
				longValue = decodificaLongInfo(aux);
				fprintf(fp, "Long High Bytes:	0x%08x\n",aux->UnionCP.CONSTANT_Long.high_bytes);
				fprintf(fp, "Long Low Bytes:		0x%08x\n",aux->UnionCP.CONSTANT_Long.low_bytes);
				fprintf(fp, "Long:			%lu\n",longValue);
				break;
			case VALOR_CONSTANT_Double:
				valor = decodificaDoubleInfo(aux);
				fprintf(fp, "Double High Bytes:	0x%08x\n",aux->UnionCP.CONSTANT_Double.high_bytes);
				fprintf(fp, "Double Low Bytes:	0x%08x\n",aux->UnionCP.CONSTANT_Double.low_bytes);
				fprintf(fp, "Double:			%.2lf\n",valor);
				break;
			case VALOR_CONSTANT_NameAndType:
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_NameAndType.name_index,5);
				fprintf(fp, "Name:		cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_NameAndType.name_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(classfile->constant_pool,aux->UnionCP.CONSTANT_NameAndType.descriptor_index,6);
				fprintf(fp, "Descriptor:	cp_info#%d <%s>\n",aux->UnionCP.CONSTANT_NameAndType.descriptor_index,ponteiroprint);
				break;
			case VALOR_CONSTANT_Utf8:
				fprintf(fp, "Length of byte array:	%d\n",(int)aux->UnionCP.CONSTANT_UTF8.length);
				fprintf(fp, "Length of string:	%d\n",(int)aux->UnionCP.CONSTANT_UTF8.length);
				fprintf(fp, "String:			");
				for (u1 * i = aux->UnionCP.CONSTANT_UTF8.bytes; i < aux->UnionCP.CONSTANT_UTF8.bytes + aux->UnionCP.CONSTANT_UTF8.length; i++) {
					fprintf(fp, "%c",(char) (*i));
				}
				fprintf(fp, "\n");
				break;
			case VALOR_CONSTANT_MethodHandle:
				fprintf(fp, "MethodHandle Reference Kind: 	%02x\n",aux->UnionCP.CONSTANT_MethodHandle.reference_kind);
				fprintf(fp, "MethodHandle Reference Index: 	%04x\n",aux->UnionCP.CONSTANT_MethodHandle.reference_index);
				break;
			case VALOR_CONSTANT_MethodType:
				fprintf(fp, "MethodType Descriptor Index: 	%04x\n",aux->UnionCP.CONSTANT_MethodType.descriptor_index);
				break;
			case VALOR_CONSTANT_InvokeDynamic:
				fprintf(fp, "InvokeDynamic - Bootstrap Method Attr Index: 	%04x\n",aux->UnionCP.CONSTANT_InvokeDynamicInfo.bootstrap_method_attr_index);
				fprintf(fp, "InvokeDynamic - Name and Type Index: 			%04x\n",aux->UnionCP.CONSTANT_InvokeDynamicInfo.name_and_type_index);
				break;
			default:
			fprintf(fp,"\n");
				break;
		}
		printf("\n");
	}

	fprintf(fp, "\n\n************************ Interfaces ************************\n\n");
	contador = 0;
	for (u2 * auxInterfaces = classfile->interfaces; auxInterfaces < classfile->interfaces+classfile->interfaces_count; auxInterfaces++) {
		ponteiroprint = decodificaNIeNT(classfile->constant_pool,*auxInterfaces,1);
		fprintf(fp, "Interface: 				cp_info#%d <%s>\n",*auxInterfaces, ponteiroprint);
	}

	fprintf(fp, "\n\n************************ Fields ************************\n\n");
	contador = 0;
	for (auxField = classfile->fields; auxField < classfile->fields + classfile->fields_count; auxField++,contador++) {
		fprintf(fp, ">>> [%d] Field \n",contador);
		ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+auxField->name_index);
		fprintf(fp, "Name: 				cp_info#%d <%s>\n",auxField->name_index,ponteiroprint);
		ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+auxField->descriptor_index);
		fprintf(fp, "Descriptor: 		cp_info#%d <%s>\n",auxField->descriptor_index,ponteiroprint);
		ponteiroprint = decodificaAccessFlags(auxField->access_flags);
		fprintf(fp, "Access Flags: 		0x%04x [%s]\n",auxField->access_flags,ponteiroprint);
		fprintf(fp, "Attributes Count:		%d\n\n",auxField->attributes_count);
		if (auxField->attributes_count > 0) {
			fieldAttrAux = auxField->attributes;
			for (int posicaoFields = 0; posicaoFields < auxField->attributes_count; posicaoFields++) {
				ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+(*(fieldAttrAux+posicaoFields))->attribute_name_index);
				fprintf(fp, "Attribute Name Index: 	cp_info#%d <%s>\n",(*(fieldAttrAux+posicaoFields))->attribute_name_index,ponteiroprint);
				fprintf(fp, "Attribute Length: 		%d\n",(*(fieldAttrAux+posicaoFields))->attribute_length);
				if (strcmp(ponteiroprint, "ConstantValue") == 0) {
					constantValue_attribute * cvAux = (constantValue_attribute*)(*(fieldAttrAux+posicaoFields))->info;
					cp_info * cpInfoAux = classfile->constant_pool-1+cvAux->constantvalue_index;
					//FLOAT
					if (cpInfoAux->tag == 4) {
						float valorCV = decodificaFloatInfo(classfile->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: 		cp_info#%d <%f>\n",cvAux->constantvalue_index,valorCV);
					//Integer-Byte-Boolean-Short-Char
					} else if (cpInfoAux->tag == 3) {
						int valorRetorno = decodificaIntegerInfo (classfile->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: 		cp_info#%d <%d>\n",cvAux->constantvalue_index,valorRetorno);
					//STRING
					} else if (cpInfoAux->tag == 8) {
						char * stringEntrada = decodificaNIeNT(classfile->constant_pool,cvAux->constantvalue_index,1);
						fprintf(fp, "Constant Value Index: 		cp_info#%d <%s>\n",cvAux->constantvalue_index,stringEntrada);
					//DOUBLE
					} else if (cpInfoAux->tag == 6) {
						double valorDB = decodificaDoubleInfo(classfile->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: 		cp_info#%d <%lf>\n",cvAux->constantvalue_index,valorDB);
					} else if (cpInfoAux->tag == 5) {
						uint64_t valorL = decodificaLongInfo(classfile->constant_pool-1+cvAux->constantvalue_index);
						fprintf(fp, "Constant Value Index: 		cp_info#%d <%lu>\n",cvAux->constantvalue_index,valorL);
					}
				} else if (strcmp(ponteiroprint,"Signature") == 0) {
					signature_attribute * sig = (signature_attribute*)(*(fieldAttrAux+posicaoFields))->info;
					char * Signature_Index = decodificaStringUTF8(classfile->constant_pool-1+sig->signature_index);
					fprintf(fp, "Signature index: 		cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
				}
				printf("\n");
			}
		}
		}

	fprintf(fp, "\n\n************************ Methods ************************\n\n");

	contador = 0;

	for (auxMethod = classfile->methods; auxMethod < classfile->methods + classfile->methods_count; auxMethod++,contador++) {
		fprintf(fp, "[>>> [%d] Method\n\n",contador);
		ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+auxMethod->name_index);
		fprintf(fp, "Name: 			cp_info#%d <%s>\n",auxMethod->name_index,ponteiroprint);
		ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+auxMethod->descriptor_index);
		fprintf(fp, "Descriptor: 		cp_info#%d <%s>\n",auxMethod->descriptor_index,ponteiroprint);
		ponteiroprint = decodificaAccessFlags(auxMethod->access_flags);
		fprintf(fp, "Access Flags: 		0x%04x [%s]\n",auxMethod->access_flags,ponteiroprint);
		fprintf(fp, "\nAttributes Count: 	%d\n\n",auxMethod->attributes_count);

		//fprintf(fp, "Atributos:\n");
		attribute_info ** auxAttrCompleto = auxMethod->attributes;
		for (int posicao = 0; posicao < auxMethod->attributes_count; posicao++) {
			ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+(*(auxAttrCompleto+posicao))->attribute_name_index);
			fprintf(fp, "Attribute Name Index: 	cp_info#%d <%s>\n",(*(auxAttrCompleto+posicao))->attribute_name_index,ponteiroprint);
			fprintf(fp, "Attribute Length: 	%d\n",(*(auxAttrCompleto+posicao))->attribute_length);

			if (strcmp(ponteiroprint,"Code") == 0) {
				code_attribute * auxCodePontual = (code_attribute*)(*(auxAttrCompleto+posicao))->info;
				fprintf(fp, "Max Stack:		%d\n",auxCodePontual->max_stack);
				fprintf(fp, "Max Locals: 		%d\n",auxCodePontual->max_locals);
				fprintf(fp, "Code length: 		%d\n",auxCodePontual->code_length);
				printf("\n\n");
				if(auxCodePontual->code_length > 0) {
					ponteiroprint = decodificarCode(classfile->constant_pool,classfile->constant_pool_count,auxCodePontual->code,auxCodePontual->code_length,instrucoes);
					fprintf(fp, "%s\n",ponteiroprint);
				}
				if(auxCodePontual->exception_table_length > 0) {
					fprintf(fp, "Exception Table:\n");
					fprintf(fp, "Nr.\t\tStart PC\tEnd PC\tHandread PC\tCatch Type\n");
					int contadorExceptionTable = 0;
					for(exceptionTableAux = auxCodePontual->table; exceptionTableAux < auxCodePontual->table + auxCodePontual->exception_table_length; exceptionTableAux++){
						fprintf(fp, "%d\t\t%02x\t\t%02x\t\t%02x\t%02x\n",contadorExceptionTable,exceptionTableAux->start_pc,exceptionTableAux->end_pc,exceptionTableAux->handler_pc,exceptionTableAux->catch_type);
						contadorExceptionTable++;
					}
					fprintf(fp, "\n\n");
				}
				fprintf(fp, "Attributes Count: 		%d\n",auxCodePontual->attributes_count);
				if (auxCodePontual->attributes_count > 0) {
					int lntContador = 0;
					attribute_info ** auxAttributesFromCode = auxCodePontual->attributes;
					for (int posicaoDois = 0; posicaoDois < auxCodePontual->attributes_count; posicaoDois++) {
						ponteiroprint = decodificaStringUTF8(classfile->constant_pool-1+(*(auxAttributesFromCode+posicaoDois))->attribute_name_index);
						fprintf(fp, "Attribute Name Index: 		cp_info#%d <%s>\n",(*(auxAttributesFromCode+posicaoDois))->attribute_name_index,ponteiroprint);
						fprintf(fp, "Attribute Length:		%d\n",(*(auxAttributesFromCode+posicaoDois))->attribute_length);
						if (strcmp(ponteiroprint,"LineNumberTable") == 0) {
							line_number_table * lntAux = (line_number_table*)(*(auxAttributesFromCode+posicaoDois))->info;
							fprintf(fp, "Line Number Table Length: 	%d\n",(int)lntAux->line_number_table_length);
							fprintf(fp, "\nAttribute Info: \n");
							fprintf(fp, "Nr.\t|\tStartPC\t|\tLineNumber\n");
							for (line_number_tableInfo * linfo = lntAux->info; linfo < lntAux->info + lntAux->line_number_table_length; linfo++) {
								fprintf(fp, "%d\t|\t%d\t|\t%d\n",lntContador,linfo->start_pc,linfo->line_number);
								lntContador++;
							}
							fprintf(fp, "\n");
						} else if (strcmp(ponteiroprint,"StackMapTable") == 0) {
							int offsetImpressao = 0;
							stackMapTable_attribute * smt = (stackMapTable_attribute*)(*(auxAttributesFromCode+posicaoDois))->info;
							stack_map_frame ** smf = smt->entries;
							fprintf(fp, "Nr.\t\tStack Map Frame\n");
							for (int posicaoSMF = 0; posicaoSMF < smt->number_of_entries; posicaoSMF++) {
								if ((*(smf+posicaoSMF))->frame_type >= 0 && (*(smf+posicaoSMF))->frame_type <= 63) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->frame_type);
									fprintf(fp, "%d\t\tSAME(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,(*(smf+posicaoSMF))->frame_type);
								} else if ((*(smf+posicaoSMF))->frame_type >= 64 && (*(smf+posicaoSMF))->frame_type <= 127) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,((*(smf+posicaoSMF))->frame_type)-64);
									fprintf(fp, "%d\t\tSAME_LOCALS_1_STACK_ITEM(%d), Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,((*(smf+posicaoSMF))->frame_type-64));
									fprintf(fp, "\t\tStack verifications:\n");
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame.stack;
									switch ((*(VTIAux))->tag) {
										case 0:
											fprintf(fp, "\t\t\tTOP\n");
											break;
										case 1:
											fprintf(fp, "\t\t\tINTEGER\n");
											break;
										case 2:
											fprintf(fp, "\t\t\tFLOAT\n");
											break;
										case 3:
											fprintf(fp, "\t\t\tLONG\n");
											break;
										case 4:
											fprintf(fp, "\t\t\tDOUBLE\n");
											break;
										case 5:
											fprintf(fp, "\t\t\tNULL\n");
											break;
										case 6:
											fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
											break;
										case 7:
											ponteiroprint = decodificaNIeNT(classfile->constant_pool,(*(VTIAux))->type_info.object_variable_info.cpool_index,1);
											fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux))->type_info.object_variable_info.cpool_index, ponteiroprint);
											break;
										case 8:
											fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux))->type_info.uninitialized_variable_info.offset);
											break;
									}
								} else if ((*(smf+posicaoSMF))->frame_type == 247) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta);
									fprintf(fp, "%d\t\tSAME_LOCALS_1_STACK_ITEM_EXTENDED(%d), Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta);
									fprintf(fp, "\t\tStack verifications:\n");
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.stack;
									switch ((*(VTIAux))->tag) {
										case 0:
											fprintf(fp, "\t\t\tTOP\n");
											break;
										case 1:
											fprintf(fp, "\t\t\tINTEGER\n");
											break;
										case 2:
											fprintf(fp, "\t\t\tFLOAT\n");
											break;
										case 3:
											fprintf(fp, "\t\t\tLONG\n");
											break;
										case 4:
											fprintf(fp, "\t\t\tDOUBLE\n");
											break;
										case 5:
											fprintf(fp, "\t\t\tNULL\n");
											break;
										case 6:
											fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
											break;
										case 7:
											ponteiroprint = decodificaNIeNT(classfile->constant_pool,(*(VTIAux))->type_info.object_variable_info.cpool_index,1);
											fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux))->type_info.object_variable_info.cpool_index, ponteiroprint);
											break;
										case 8:
											fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux))->type_info.uninitialized_variable_info.offset);
											break;
									}
								} else if ((*(smf+posicaoSMF))->frame_type >= 248 && (*(smf+posicaoSMF))->frame_type <= 250) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.chop_frame.offset_delta);
									fprintf(fp, "%d\t\tCHOP(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.chop_frame.offset_delta);
								} else if ((*(smf+posicaoSMF))->frame_type == 251) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.same_frame_extended.offset_delta);
									fprintf(fp, "%d\t\tSAME_FRAME_EXTENDED(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.same_frame_extended.offset_delta);
								} else if ((*(smf+posicaoSMF))->frame_type >= 252 && (*(smf+posicaoSMF))->frame_type <= 254) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.append_frame.offset_delta);
									fprintf(fp, "%d\t\tAPPEND(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao, (*(smf+posicaoSMF))->map_frame_type.append_frame.offset_delta);
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.append_frame.locals;
									fprintf(fp, "\t\t  Local verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < ((*(smf+posicaoSMF))->frame_type-251); posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												fprintf(fp, "\t\t\tTOP\n");
												break;
											case 1:
												fprintf(fp, "\t\t\tINTEGER\n");
												break;
											case 2:
												fprintf(fp, "\t\t\tFLOAT\n");
												break;
											case 3:
												fprintf(fp, "\t\t\tLONG\n");
												break;
											case 4:
												fprintf(fp, "\t\t\tDOUBLE\n");
												break;
											case 5:
												fprintf(fp, "\t\t\tNULL\n");
												break;
											case 6:
												fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(classfile->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,1);
												fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
								} else if ((*(smf+posicaoSMF))->frame_type == 255) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.full_frame.offset_delta);
									fprintf(fp, "%d\t\tFULL_FRAME(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao, (*(smf+posicaoSMF))->map_frame_type.full_frame.offset_delta);
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.full_frame.locals;
									fprintf(fp, "\t\t  Local verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < (*(smf+posicaoSMF))->map_frame_type.full_frame.number_of_locals; posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												fprintf(fp, "\t\t\tTOP\n");
												break;
											case 1:
												fprintf(fp, "\t\t\tINTEGER\n");
												break;
											case 2:
												fprintf(fp, "\t\t\tFLOAT\n");
												break;
											case 3:
												fprintf(fp, "\t\t\tLONG\n");
												break;
											case 4:
												fprintf(fp, "\t\t\tDOUBLE\n");
												break;
											case 5:
												fprintf(fp, "\t\t\tNULL\n");
												break;
											case 6:
												fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(classfile->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,1);
												fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
									VTIAux = (*(smf+posicaoSMF))->map_frame_type.full_frame.stack;
									fprintf(fp, "\t\t  Stack verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < (*(smf+posicaoSMF))->map_frame_type.full_frame.number_of_stack_items; posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												fprintf(fp, "\t\t\tTOP\n");
												break;
											case 1:
												fprintf(fp, "\t\t\tINTEGER\n");
												break;
											case 2:
												fprintf(fp, "\t\t\tFLOAT\n");
												break;
											case 3:
												fprintf(fp, "\t\t\tLONG\n");
												break;
											case 4:
												fprintf(fp, "\t\t\tDOUBLE\n");
												break;
											case 5:
												fprintf(fp, "\t\t\tNULL\n");
												break;
											case 6:
												fprintf(fp, "\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(classfile->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,1);
												fprintf(fp, "\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												fprintf(fp, "\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
								}
							}
						}
					}
				}
			} else if (strcmp(ponteiroprint,"Exceptions") == 0) {
				exceptions_attribute * excpAux = (exceptions_attribute*)(*(auxAttrCompleto+posicao))->info;
				int contadorExcp = 0;
				char * exceptionIndexString;
				fprintf(fp, "Nr.\t\tException\t\t\tVerbose\n");
				for (u2 * indexExcp = excpAux->exception_index_table; indexExcp < excpAux->exception_index_table + excpAux->number_of_exceptions; indexExcp++) {
					exceptionIndexString = decodificaNIeNT(classfile->constant_pool,*indexExcp,1);
					fprintf(fp, "%d\t\tcp_info#%d\t\t\t%s\n",contadorExcp,*indexExcp,exceptionIndexString);
					contadorExcp++;
				}
			} else if (strcmp(ponteiroprint,"Signature") == 0) {
				signature_attribute * sig = (signature_attribute*)(*(auxAttrCompleto+posicao))->info;
				char * Signature_Index = decodificaStringUTF8(classfile->constant_pool-1+sig->signature_index);
				fprintf(fp, "Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
			}
		}
	}

	fprintf(fp, "\n\n************************ Attributes ************************\n\n");
	auxAttributeClasse = classfile->attributes;
	for (int posicao = 0; posicao < classfile->attributes_count; posicao++) {
		ponteiroprint = decodificaStringUTF8(classfile->constant_pool+(*(auxAttributeClasse+posicao))->attribute_name_index-1);
		fprintf(fp, "Attribute Name Index:		cp_info#%d <%s>\n",(*(auxAttributeClasse+posicao))->attribute_name_index,ponteiroprint);
		fprintf(fp, "Attribute Length:		%d\n",(int) (*(auxAttributeClasse+posicao))->attribute_length);
		if (strcmp(ponteiroprint,"SourceFile") == 0) {
		 	source_file_attribute * SourceFile = ((source_file_attribute*)((*(auxAttributeClasse+posicao))->info));
			fprintf(fp, "Source File Name Index:		cp_info#%d <%s>\n",SourceFile->source_file_index,decodificaStringUTF8(classfile->constant_pool+SourceFile->source_file_index-1));
		} else if (strcmp(ponteiroprint, "InnerClasses") == 0) {
			innerClasses_attribute * innerC = ((innerClasses_attribute*)((*(auxAttributeClasse+posicao))->info));
			fprintf(fp, "Nr.\t\tInner Class\t\t\tOuter Class\t\tInner Name\t\tAccess Flags\n");
			char * innerClassString, * outerClassString, * innerNameIndex, * accessFlagsInner;
			classes ** vetorClasses = innerC->classes_vector;
			for (int posicaoInncerC = 0; posicaoInncerC < innerC->number_of_classes; posicaoInncerC++) {
				innerClassString = decodificaNIeNT(classfile->constant_pool,(*(vetorClasses+posicaoInncerC))->inner_class_info_index,1);
				outerClassString = decodificaNIeNT(classfile->constant_pool,(*(vetorClasses+posicaoInncerC))->outer_class_info_index,1);
				innerNameIndex = decodificaStringUTF8(classfile->constant_pool-1+(*(vetorClasses+posicaoInncerC))->inner_name_index);
				accessFlagsInner = decodificaAccessFlags((*(vetorClasses+posicaoInncerC))->inner_class_access_flags);
				fprintf(fp, "%d\t\tcp_info#%d\t\t\tcp_info#%d\t\tcp_info#%d\t\t0x%04x\n",posicaoInncerC,(*(vetorClasses+posicaoInncerC))->inner_class_info_index,(*(vetorClasses+posicaoInncerC))->outer_class_info_index,(*(vetorClasses+posicaoInncerC))->inner_name_index,(*(vetorClasses+posicaoInncerC))->inner_class_access_flags);
				fprintf(fp, "  \t\t%s\t\t%s\t\t%s\t\t\t%s\n",innerClassString,outerClassString,innerNameIndex,accessFlagsInner);
			}
		} else if (strcmp(ponteiroprint,"Signature") == 0) {
			signature_attribute * sig = (signature_attribute*)((*(auxAttributeClasse+posicao))->info);
			char * Signature_Index = decodificaStringUTF8(classfile->constant_pool-1+sig->signature_index);
			fprintf(fp, "Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
		}
	}
}

int setaOffsetImpressao (int posicao, u1 offset) {

	if (posicao == 0) {
		return offset;
	} else {
		return (offset+1);
	}
}
