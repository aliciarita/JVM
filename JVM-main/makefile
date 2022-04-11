# Compiler GCC
GCC=gcc
C99=-std=c99

# Flags GCC
FLAGS=-Wall
LM=-lm

# Analisadores
ANALISADORES=-g3 -fsanitize=address -fno-omit-frame-pointer --analyze -Xanalyzer -analyzer-output=text

# Remove
RM=rm

all:
	$(GCC) $(C99) $(FLAGS) classFile.h instrucoes.h leitor.h interpretador.h estruturas.h listaOperandos.h listaFrames.h listaClasses.h pilhaOperandos.h pilhaFrames.h jvm.h  instrucoes.c leitor.c listaOperandos.c listaFrames.c listaClasses.c pilhaFrames.c pilhaOperandos.c interpretador.c jvm.c main.c $(LM)

debug:
	$(GCC) -g $(C99) $(FLAGS) $(ANALISADORES) classFile.h instrucoes.h leitor.h interpretador.h estruturas.h listaOperandos.h listaFrames.h listaClasses.h pilhaOperandos.h pilhaFrames.h jvm.h  instrucoes.c leitor.c listaOperandos.c listaFrames.c listaClasses.c pilhaFrames.c pilhaOperandos.c interpretador.c jvm.c main.c $(LM)

clean:
	$(RM) a.out
	$(RM) *.gch