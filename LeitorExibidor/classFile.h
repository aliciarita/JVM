/**
 * @file estruturasclassfile.h
 * Estrutura interna de arquivo .class 
 * @date 2022-03-10
*/
#ifndef	ESTRUTURACLASSFILE_H
#define ESTRUTURACLASSFILE_H

#include <stdio.h>
#include <stdint.h>
#include "estruturas.h"
	/**
	 * Estrutura ClassFile
	 */
typedef struct{
	u4 				magic;
	u2 				minor_version;
	u2 				major_version;
	u2 				constant_pool_count;
	cp_info 		*constant_pool;  /* [constant_pool_count-1] */
	u2 				access_flags;
	u2 				this_class;
	u2 				super_class;
	u2 				interfaces_count;
	u2 				*interfaces;	/* [interfaces_count] */
	u2 				fields_count;
	field_info 		*fields;		/* [fileds_count] */
	u2 				methods_count;
	method_info 	*methods;		/* [methods_count] */
	u2 				attributes_count;
	attribute_info ** attributes;   /* [attributes_count] */
}ClassFile;
#endif
