/**
 * @file estruturas.h
 * @date 2022-03-11
 */
#ifndef	ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdint.h>

/**
 * Rótulos para uint8_t, uint16_t e uint32_t.
*/

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

/**
 * Define u1, u2, u4
 */
#define MAXIMO_U1 255
#define MAXIMO_U2 65535
#define MAXIMO_U4 2147483647

/**
 * Estrutura de Constant Pool cp_info
 * O byte de tag define o tipo da informação em cp_info
*/

typedef struct cp_info{
	u1 tag;
	 /* Pool de constantes: as informações estão armazenadas na tabela constant_pool */
	union{ /* Tipos válidos de tags */

		struct CONSTANT_Class{
			u2 name_index;
		}CONSTANT_Class;

		struct CONSTANT_Fieldref{
			u2 class_index;
			u2 name_and_type_index;
		}CONSTANT_Fieldref;

		struct CONSTANT_Methodref{
			u2 class_index;
			u2 name_and_type_index;
		}CONSTANT_Methodref;
		
		struct CONSTANT_InterfaceMethodref{
			u2 class_index;
			u2 name_and_type_index;
		}CONSTANT_InterfaceMethodref;
		
		struct CONSTANT_Integer{
			u4 bytes;
		}CONSTANT_Integer;

		struct CONSTANT_Long{
			u4 high_bytes;
			u4 low_bytes;
		}CONSTANT_Long;

		struct CONSTANT_Double{
			u4 high_bytes;
			u4 low_bytes;
		}CONSTANT_Double;
		
		struct CONSTANT_Float{
			u4 bytes;
		}CONSTANT_Float;

		struct CONSTANT_String{
			u2 string_index;
		}CONSTANT_String;
		
		struct CONSTANT_NameAndType{
			u2 name_index;
			u2 descriptor_index;
		}CONSTANT_NameAndType;

		struct CONSTANT_MethodHandle{
			u1 reference_kind;
			u2 reference_index;
		}CONSTANT_MethodHandle;

		struct CONSTANT_MethodType{
			u2 descriptor_index;
		}CONSTANT_MethodType;
		
		struct CONSTANT_InvokeDynamicInfo{
			u2 bootstrap_method_attr_index;
    		u2 name_and_type_index;
		}CONSTANT_InvokeDynamicInfo;

		struct CONSTANT_UTF8{
			u2 length;
			u1 *bytes;
		}CONSTANT_UTF8;

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
	SYNCHRONIZED = 32,		/*Sincronizado: requer um monitor antes de ser executado (Thread)*/
	BRIDGE = 64,			/*Método Bridge, gerado por um compilador*/
	VARARGS = 128,			/*Declarado com um número variável de argumentos*/
	NATIVE = 256,			/*Nativo: implementado em linguagem não Java (C, C++, Assembly)*/
	ABSTRACT = 1024,		/*Abstrato: sem definição, deve ser sobreescrito em uma subclasse*/
	STRICT = 2048,			/*Strictfp: utiliza modo de ponto flutuante FP-strict (não normalizado)*/
	SYNTHETIC = 4096,		/*Sintético: não presente no código fonte*/
	ENUM = 16384,
	VOLATILE = 64,			
	TRANSIENT = 128
};

/**
 * pool de constantes
 * tipos válidos de tags
 */
enum ValoresTag {

	VALOR_CONSTANT_Class = 7,
	VALOR_CONSTANT_Fieldref = 9,
	VALOR_CONSTANT_Methodref = 10,
	VALOR_CONSTANT_InterfaceMethodref = 11,
	VALOR_CONSTANT_String = 8,
	VALOR_CONSTANT_Integer = 3,
	VALOR_CONSTANT_Float = 4,
	VALOR_CONSTANT_Long = 5,
	VALOR_CONSTANT_Double = 6,
	VALOR_CONSTANT_NameAndType = 12,
	VALOR_CONSTANT_Utf8 = 1,
	VALOR_CONSTANT_MethodHandle = 15,
	VALOR_CONSTANT_MethodType = 16,
	VALOR_CONSTANT_InvokeDynamic = 18
};


typedef struct attribute_info {
	u2 attribute_name_index;
	u4 attribute_length;
	void *info;
}attribute_info;

typedef struct field_info{
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info ** attributes;
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

typedef struct code_attribute {
  u2 max_stack;
  u2 max_locals;
  u4 code_length;
  u1 * code;
  u2 exception_table_length;
  exception_table * table; 
  u2 attributes_count;
	attribute_info ** attributes;
}code_attribute;

typedef struct method_info{
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info ** attributes;
}method_info;

typedef struct source_file_attribute {
	u2 attribute_name_index;
	u4 attribute_length;
	u2 source_file_index;
}source_file_attribute;

typedef struct constantValue_attribute {
    u2 constantvalue_index;
}constantValue_attribute;

typedef struct exceptions_attribute {
	u2 attribute_name_index;
	u4 attribute_length;
	u2 number_of_exceptions;
	u2 *exception_index_table; 	// [number_of_exceptions]
}exceptions_attribute;

typedef struct classes {
	u2 inner_class_info_index;
	u2 outer_class_info_index;	
	u2 inner_name_index;		
	u2 inner_class_access_flags; 
}classes;

typedef struct innerClasses_attribute{
	u2 attribute_name_index;
	u4 attribute_length;
	u2 number_of_classes;
	classes ** classes_vector; 	
}innerClasses_attribute;

typedef struct enclosingMethod_attribute {
	u2 class_index;			
	u2 method_index;	
}enclosingMethod_attribute;

typedef struct signature_attribute {
	u2 signature_index;
}signature_attribute;

typedef struct sourceDebugExtension_attribute {
	u1 * debug_extension; 	
}sourceDebugExtension_attribute;

typedef struct local_variable_table {
	u2 start_pc;
	u2 length;
	u2 name_index;
	u2 descriptor_index;
	u2 index;
}local_variable_table;

typedef struct localVariableTable_attributes {
	u2 attribute_name_index;
	u4 attribute_length;
	u2 local_variable_table_length;
	local_variable_table *local_variables; 
}localVariableTable_attributes;

typedef struct localVariableTypeTable {
	u2 local_variable_type_table_length;
	local_variable_table *local_variables; 
}localVariableTypeTable;

struct annotation;
struct element_value;

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


struct element_value_pairs {

	u2 element_name_index;
	element_value value;

};
typedef struct element_value_pairs element_value_pairs;


struct annotation {

	u2 type_index;
	u2 num_element_value_pairs;
	element_value_pairs *elementes_pairs;

};
typedef struct annotation annotation;


struct runTimeVisibleAnnotations_attribute {

	u2 num_annotations;
	annotation *annotations;


};
typedef struct runTimeVisibleAnnotations_attribute runTimeVisibleAnnotations_attribute;


struct runTimeInvisibleAnnotations_attribute {

	u2 num_annotations;
	annotation *annotations;

};
typedef struct runTimeInvisibleAnnotations_attribute runTimeInvisibleAnnotations_attribute;


struct parameter_annotations {	

	u2 num_annotations;
	annotation *annotations;	


};
typedef struct parameter_annotations parameter_annotations;


struct runtimeVisibleParameterAnnotations_attribute {

    u1 num_parameters;
    parameter_annotations *parameters_annotations;	

};
typedef struct runtimeVisibleParameterAnnotations_attribute runtimeVisibleParameterAnnotations_attribute;



struct runtimeInvisibleParameterAnnotations_attribute {

    u1 num_parameters;
	parameter_annotations *parameters_annotations;

};
typedef struct runtimeInvisibleParameterAnnotations_attribute runtimeInvisibleParameterAnnotations_attribute;


struct annotationDefault_attribute {

    element_value default_value;

};
typedef struct annotationDefault_attribute annotationDefault_attribute;

struct bootstrap_methods {

	u2 bootstrap_method_ref;
	u2 num_bootstrap_arguments;
    u2 *bootstrap_arguments;		


};
typedef struct bootstrap_methods bootstrap_methods;


struct bootstrapMethods_attribute {

    u2 num_bootstrap_methods;

    bootstrap_methods *bt_methods; 

};
typedef struct bootstrapMethods_attribute bootstrapMethods_attribute;



struct verification_type_info{

	u1 tag;

	union{

		struct {

		} top_variable_info;

		struct {

		} integer_variable_info;

		struct {

		} float_variable_info;

		struct {

		} long_variable_info;

		struct{

		} double_variable_info;

		struct {
		} null_variable_info;

		struct {

		} uninitializedThis_variable_info;
		struct {
			u2 cpool_index;

		} object_variable_info;

		struct {
			u2 offset; 

		} uninitialized_variable_info;


	} type_info;

};
typedef struct verification_type_info verification_type_info;


struct stack_map_frame {
	u1 frame_type;
	union{
		struct{

		

		} same_frame;

		struct{
			
			verification_type_info ** stack; 

		} same_locals_1_stack_item_frame;

		struct{

			u2 offset_delta;
    	verification_type_info ** stack; 

		} same_locals_1_stack_item_frame_extended;

		struct{
			u2 offset_delta;

		} chop_frame;

		struct{
			u2 offset_delta;

		} same_frame_extended;

		struct{

			u2 offset_delta;
			verification_type_info ** locals;

		} append_frame;

		struct{
    	u2 offset_delta;
    	u2 number_of_locals;
    	verification_type_info ** locals; 
    	u2 number_of_stack_items;
    	verification_type_info ** stack; 
		} full_frame;
	} map_frame_type;
};
typedef struct stack_map_frame stack_map_frame;


struct stackMapTable_attribute {
    u2 number_of_entries;
    stack_map_frame ** entries;
};
typedef struct stackMapTable_attribute stackMapTable_attribute;

#endif
