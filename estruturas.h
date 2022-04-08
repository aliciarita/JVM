/**
 * @file estruturas.h
 * @date 2022-03-30
 */
#ifndef	ESTRUTURAS_h
#define ESTRUTURAS_h

/*  bibliotecas */
#include <stdio.h>
#include <stdint.h>
#include "pilhaOperandos.h"

/* Rótulos para uint8_t = u1, uint16_t = u2, uint32_t = u4*/
typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;


/** Definição de valores máximos para u1, u2, u4*/
#define MAXU1 255
#define MAXU2 65535
#define MAXU4 2147483647

/* Definição para float*/
#define expoente(x) ((x << 1) >> 24)
#define mantissa(x) ((x << 9) >> 9)
#define sinal(x) (x >> 31)

/* Definição para double */
#define expoente_d(x) ((x << 1) >> 53)
#define mantissa_d(x) ((x << 12) >> 12)
#define sinal_d(x) (x >> 63)


#define NAME_INDEX 1
#define NAME_AND_TYPE 2
#define STRING_INDEX 3
#define CLASS_INDEX 4
#define NAME_AND_TYPE_INFO_NAME_INDEX 5
#define NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX 6
#define FIELD_INDEX 7


/**
 * Estrutura de Constant Pool cp_info
 * O byte de tag define o tipo da informação em cp_info
*/
typedef struct cp_info{
	u1 tag; /* byte tag que define o tipo de entrada*/
	/* Pool de constantes: as informações estão armazenadas na tabela constant_pool */

	/* Union para conter os tipos de entrada na Constant Pool.
	Cada struct assume com seus respectivos atributos de informação*/
	union{ /* Tipos válidos de tags */

		/* CONSTANT */
		struct{
			u2 name_index; /*nome da classe ou interface */
		} Class;
		
		struct{
			u2 class_index; /* nome da classe ou interface que tem a declaração do field*/
			u2 name_and_type_index; /* o nome e o tipo do field*/
		} Fieldref;

		struct{
			u2 class_index; /* declaração desse método*/
			u2 name_and_type_index; /*o nome e o tipo do método*/
		} Methodref;

		struct{
			u2 class_index; /* declaração desse método*/
			u2 name_and_type_index; /*o nome e o tipo do método*/
		} InterfaceMethodref;

		struct{
			u2 string_index; /*	a sequência de caracteres com a qual o objeto String será iniciado*/
		} String;

		struct{
			u4 bytes;	/*int em big-endian*/
		} Integer;

		struct{
			u4 bytes; /*float em big-endian */
		} Float;

		struct{
			u4 high_bytes;/* Parte alta do valor, 4 bytes*/
			u4 low_bytes; /* Parte baixa do valor, 4 bytes*/
		} Long;
		
		struct{
			u4 high_bytes;/* Parte alta do valor, 4 bytes*/
			u4 low_bytes; /* Parte baixa do valor, 4 bytes*/
		} Double;

		struct{
			u2 name_index; /* nome do field ou método */
			u2 descriptor_index; /* descritor do field ou método*/
		} NameAndType;

		struct{
			u2 length; /*número de bytes no array*/
			u1 *bytes; /*bytes da string*/
		} UTF8;
		
		struct{
			u1 reference_kind;/*caracteriza o comportamento do bytecode*/
			u2 reference_index; /*indica a estrutura de Method,Field ou InterfaceMethod*/
		} MethodHandle;
		
		struct{
			u2 descriptor_index; /* descritor do método*/
		} MethodType;

		struct{
			u2 bootstrap_method_attr_index;/* indice válido em Bootstrap Methods*/
    		u2 name_and_type_index; /* nome e o tipo do método*/
		} InvokeDynamicInfo;
	}UnionCP;
}cp_info;

/**
 * Controle de acesso
 * u2 access_flags
 * máscaras em decimal especificando permissões de acesso e própriedade de métodos.
 */
enum access_flags{
	PUBLIC = 1,				/*Público: pode ser acessado de fora do pacote.*/
	PRIVATE = 2,			/*Privado: acesso restrito à definição da classe.*/
	PROTECTED = 4,			/*Protegido: pode ser chamado na classe e subclasses.*/
	STATIC = 8,				/*Estático: método de classe (chamado sem referir objeto)*/
	FINAL = 16,				/*Final: não pode ser sobre-escrito em subclasses*/
	SUPER = 32,		/*Sincronizado: requer um monitor antes de ser executado (Thread)*/
	VOLATILE = 64,			
	TRANSIENT = 128,
	NATIVE = 256,			/*Nativo: implementado em linguagem não Java (C, C++, Assembly)*/
	INTERFACE_FLAG = 512,
	ABSTRACT = 1024,		/*Abstrato: sem definição, deve ser sobreescrito em uma subclasse*/
	STRICT = 2048,			/*Strictfp: utiliza modo de ponto flutuante FP-strict (não normalizado)*/
	SYNTHETIC = 4096,		/*Sintético: não presente no código fonte*/
	ENUM = 16384,
};

/**
 * pool de constantes
 * tipos válidos de tags
 */
enum ValoresTag {

	CONSTANT_Class = 7,
	CONSTANT_Fieldref = 9,
	CONSTANT_Methodref = 10,
	CONSTANT_InterfaceMethodref = 11,
	CONSTANT_String = 8,
	CONSTANT_Integer = 3,
	CONSTANT_Float = 4,
	CONSTANT_Long = 5,
	CONSTANT_Double = 6,
	CONSTANT_NameAndType = 12,
	CONSTANT_Utf8 = 1,
	CONSTANT_MethodHandle = 15,
	CONSTANT_MethodType = 16,
	CONSTANT_InvokeDynamic = 18
};

enum tipos_array{
	T_BOOLEAN = 4,
	T_CHAR,
	T_FLOAT,
	T_DOUBLE,
	T_BYTE,
	T_SHORT,
	T_INT,
	T_LONG
};

/**Definição da estrutura de Attributes*/
typedef struct attribute_info {
	u2 attribute_name_index;/*o nome do atributo*/
	u4 attribute_length;/*tamanho restante do atributo*/
	void *info;/* array contendo a informação no formato String*/
}attribute_info;

/* Definição do static data */
typedef struct staticData {
	u4 * low;
	u4 * high;
	u1 * string;
}staticData;

/**Definição de estrutura de Field*/
typedef struct field_info{
	u2 access_flags; /*acesso ao field e suas propriedades*/
	u2 name_index;/*o nome do field*/
	u2 descriptor_index; /*descritor do field*/
	u2 attributes_count; /* contador de atributos*/
	attribute_info ** attributes;/* array de estruturas Attribute Info*/
	staticData * dadosStatics;
}field_info;

typedef struct exception_table{
	u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
}exception_table;

typedef struct line_number_tableInfo {
	u2 start_pc;
	u2 line_number;
}line_number_tableInfo;

typedef struct line_number_table {
	u2 line_number_table_length;
	line_number_tableInfo * info;
}line_number_table;

/**Definição de estrutura Code attribute */
typedef struct code_attribute {
  	u2 max_stack;/*máximo do operando na pilha deste método */
  	u2 max_locals;/*máximo variáveis locais alocado na chamada deste método*/
  	u4 code_length;	/** número de bytes no array do código para este método*/
  	u1 * code;	/*array para o código que implementa o método*/
  	u2 exception_table_length; /* Número de entrada na tabela de exceções*/
  	exception_table * table; /* Alocar com exception_table_length*/
  	u2 attributes_count;/*Número de atributos do Code Attribute*/
	attribute_info ** attributes;/* array de atributos para este Code*/
}code_attribute;

/**Definição de estrutura Method*/
typedef struct method_info{
	u2 access_flags;/*acesso ao método e suas propriedades*/
	u2 name_index; /* nome do método*/
	u2 descriptor_index;/* descritor do método*/
	u2 attributes_count; /* número de atributos adicionais deste método*/
	attribute_info ** attributes;/* array de estruturas Attrite Info*/
}method_info;

typedef struct source_file_attribute {
	u2 source_file_index;
}source_file_attribute;

typedef struct constantValue_attribute {
    u2 constantvalue_index;
}constantValue_attribute;

typedef struct exceptions_attribute {

	u2 number_of_exceptions;
	u2 *exception_index_table; 	// Alocar com number_of_exceptions
							   	// Each value in the exception_index
								// table array must be a valid
								// index into the constant_pool table.

}exceptions_attribute;

typedef struct classes {

	u2 inner_class_info_index;	// Todos essas variaveis devem apontar para constant_pool;
	u2 outer_class_info_index;	// If C is not a member of a class or an interface
								// (that is, if C is a top-level class or interface
								// or a local class or an anonymous class), the value
								// of the outer_class_info_index item must be zero.
	u2 inner_name_index;		// If C is anonymous, the value of the
								// inner_name_index item must be zero.

	u2 inner_class_access_flags; // It is used by a compiler to recover the original
								 // information when source code is not available.

}classes;

typedef struct innerClasses_attribute{

	u2 number_of_classes;
	/* o numero da classe pode ser
		ACC_PUBLIC			1
		ACC_PRIVATE			2
		ACC_PROTECTED		4
		ACC_STATIC			8
		ACC_FINAL			16
		ACC_INTERFACE		512
		ACC_ABSTRACT		1024
		ACC_SYNTHETIC		4096
		ACC_ANNOTATION		8192
		ACC_ENUM			16384
	*/	
	classes ** classes_vector; 	/* Alocar com number_of_classes*/
}innerClasses_attribute;


typedef struct enclosingMethod_attribute {
	u2 class_index;			
	u2 method_index;		
}enclosingMethod_attribute;

/*typedef struct synthetic_attribute {
	u2 attribute_name_index;
	u4 attribute_length;
}synthetic_attribute;*/

typedef struct signature_attribute {
	u2 signature_index;
}signature_attribute;

typedef struct sourceDebugExtension_attribute {
	u1 * debug_extension; /* Alocar com o tamanho de attribute_length*/
}sourceDebugExtension_attribute;

typedef struct local_variable_table {
	u2 start_pc;
	u2 length;
	u2 name_index;
	u2 descriptor_index;
	u2 index;
}local_variable_table;

typedef struct localVariableTable_attributes {
	u2 local_variable_table_length;
	local_variable_table *local_variables; /* Alocar com o tamanho de local_variable_table_length*/
}localVariableTable_attributes;

typedef struct localVariableTypeTable {
	u2 local_variable_type_table_length;
	local_variable_table *local_variables; /* Alocar com o tamanho de local_variable_table_length*/
}localVariableTypeTable;

/*typedef struct deprecated_attribute{
	u2 attribute_name_index;
	u4 attribute_length;
}deprecated_attribute; */

struct annotation;

typedef struct element_value{
	u1 tag;
	union{
		u2 const_value_index;
		u2 outer_class_info_index;
		
		struct annotation *annotation_value;
		
		struct {
			u2 type_name_index;
			u2 const_name_index;
		} enum_const_index;
		
		struct {
			u2 num_values;
			struct element_value *element_values;
		} array_value;

	} value;
}element_value;

typedef struct element_value_pairs {
	u2 element_name_index;
	element_value value;
}element_value_pairs;

typedef struct annotation {
	u2 type_index;
	u2 num_element_value_pairs;
	element_value_pairs *elementes_pairs;
}annotation;

typedef struct runTimeVisibleAnnotations_attribute {
	u2 num_annotations;
	annotation *annotations;
}runTimeVisibleAnnotations_attribute;

typedef struct runTimeInvisibleAnnotations_attribute {
	u2 num_annotations;
	annotation *annotations;/* Alocar com o tamanho de num_annotations*/
}runTimeInvisibleAnnotations_attribute;

typedef struct parameter_annotations {
	u2 num_annotations;
	annotation *annotations;/* Alocar com o tamanho de num_annotations*/
}parameter_annotations;

typedef struct runtimeVisibleParameterAnnotations_attribute {
    u1 num_parameters;
    parameter_annotations *parameters_annotations;	/* Alocar com o tamanho de num_parameters*/
}runtimeVisibleParameterAnnotations_attribute;

typedef struct runtimeInvisibleParameterAnnotations_attribute {
    u1 num_parameters;
	parameter_annotations *parameters_annotations;	/*Alocar com o tamanho de num_parameters*/
}runtimeInvisibleParameterAnnotations_attribute;

typedef struct annotationDefault_attribute {
    element_value default_value;
}annotationDefault_attribute;

typedef struct bootstrap_methods {
	u2 bootstrap_method_ref;
	u2 num_bootstrap_arguments;
    u2 *bootstrap_arguments;/* Alocar com o tamanho num_bootstrap_arguments*/
}bootstrap_methods;

typedef struct bootstrapMethods_attribute {
    u2 num_bootstrap_methods;
    bootstrap_methods *bt_methods; /*Alocar com o tamanho de num_bootstrap_methods*/
}bootstrapMethods_attribute;

typedef struct verification_type_info{
	u1 tag;
	union{
		struct {
			//u1 tag; // aqui tem valor 0;
		} top_variable_info;

		struct {
			//u1 tag; // aqui tem valor 1;
		} integer_variable_info;

		struct {
			//u1 tag; // aqui tem valor 2;
		} float_variable_info;

		struct {
			//u1 tag; // aqui tem valor 4;
		} long_variable_info;

		struct{
			//u1 tag; // aqui tem valor 3;
		} double_variable_info;

		struct {
			//u1 tag; // aqui tem valor 5;
		} null_variable_info;

		struct {
			//u1 tag; // aqui tem valor 6;
		} uninitializedThis_variable_info;

		struct {
			//u1 tag; // aqui tem valor 7;
			u2 cpool_index; /*index da classe em constant_pool*/
		} object_variable_info;

		struct {
			//u1 tag; // aqui tem valor 8
			u2 offset; 
		} uninitialized_variable_info;

	} type_info;
}verification_type_info;


typedef struct stack_map_frame {
	u1 frame_type;
	union{
		struct{

		} same_frame;

		struct{
			//u1 frame_type; // == 64 a 127
			verification_type_info ** stack; 
		} same_locals_1_stack_item_frame;

		struct{
			//u1 frame_type; // == 247
			u2 offset_delta;
    	verification_type_info ** stack;
		} same_locals_1_stack_item_frame_extended;

		struct{
			//u1 frame_type; //= 248 a 250
			u2 offset_delta;
		} chop_frame;

		struct{
			//u1 frame_type; // == 251
			u2 offset_delta;
		} same_frame_extended;

		struct{
			//u1 frame_type;// == 252 a 254
			u2 offset_delta;
			verification_type_info ** locals;
		} append_frame;

		struct{
			//frame_type == 255
    	u2 offset_delta;
    	u2 number_of_locals;
    	verification_type_info ** locals; /*Vetor alocar com number_of_locals*/
    	u2 number_of_stack_items;
    	verification_type_info ** stack; /* Vetor alocar com number_of_stack_items*/
		} full_frame;

	} map_frame_type;
}stack_map_frame;

typedef struct stackMapTable_attribute {
    u2 number_of_entries;
    stack_map_frame ** entries;
}stackMapTable_attribute;

typedef struct vetor_locais{
	u4 *variavel;
	u1 tipo_variavel;
}vetor_locais;

typedef struct frame{
	u4 end_retorno; // Confirmar se é realmente endereço de retorno
	pilha_operandos *p;
	vetor_locais *v;
	u2 vetor_length;
	cp_info *cp;
	char *classeCorrente;
}frame;

#endif