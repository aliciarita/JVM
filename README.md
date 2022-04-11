# JVM - Java Virtual Machine
## Projeto lógico da JVM - Software Básico UnB

Para compilar basta rodar

> make

ou

> gcc -std=c99 -Wall classFile.h instrucoes.h leitor.h interpretador.h estruturas.h listaOperandos.h listaFrames.h listaClasses.h pilhaOperandos.h pilhaFrames.h jvm.h  instrucoes.c leitor.c listaOperandos.c listaFrames.c listaClasses.c pilhaFrames.c pilhaOperandos.c interpretador.c jvm.c main.c -lm

Será gerado um arquivo executável, que receber um arquivo ".class", para o Windows:

> ./a.exe nome_do_arquivo.class

Para o Linux:

> ./a.out nome_do_arquivo.class

