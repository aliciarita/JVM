all:
	gcc -std=c99 -Wall estruturas.h  classFile.h instrucoes.h leitor.h instrucoes.c leitor.c main.c -lm

debug:
	gcc -g  -std=c99 -Wall -g3 -fsanitize=address -fno-omit-frame-pointer --analyze -Xanalyzer -analyzer-output=text estruturas.h  classFile.h instrucoes.h leitor.h instrucoes.c leitor.c main.c -lm

clean:
	rm a.out
	rm *.gch
