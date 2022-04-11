// main.c
// Arquivo com o código 'main' para execução do programa.

/* Inclusão de bibliotecas para entrada e saída de dados, tratamento de strings e estruturas e mecanismos de leitura do arquivo .class*/

#include "leitor.h"
#include "jvm.h"

/*
  
  Função main do programa
  
  argc Número de argumentos do programa
  argv Argumentos da linha de comando
 
 */
int main(int argc, char const *argv[]){

	/*Alocação de string para conter o nome do arquivo .class a ser lido*/
	char *nomearquivo = malloc(100*sizeof(char));
	char *outputName = malloc(100*sizeof(char));  
	int outset = 0, inputset = 0;  

	/*Alocação da estrutura para o arquivo .class*/
	jvm = CriarJVM();
	jvm = InicializarJVM();
	// jvm->classes = InserirFim(jvm->classes);
	//classes->arquivoClass = (ClassFile*)malloc(sizeof(ClassFile));

	/*Estrutura condicional que analisa se foi passado o nome do arquivo como parâmetro*/
	if(argc > 1){
		if(strlen(argv[1]) > 4 && !strcmp(argv[1] + strlen(argv[1]) - 4, ".txt")){
			strcpy(outputName, argv[1]);
			outset = 1;
		} else if(strlen(argv[1]) > 6 && !strcmp(argv[1] + strlen(argv[1]) - 6, ".class")){
			strcpy(nomearquivo, argv[1]);
			inputset = 1;
		}
		if(argc > 2){
			if(strlen(argv[2]) > 4 && !strcmp(argv[2] + strlen(argv[2]) - 4, ".txt")){
				strcpy(outputName, argv[2]);
				outset = 1;
			} else if(strlen(argv[2]) > 6 && !strcmp(argv[2] + strlen(argv[2]) - 6, ".class")){
				strcpy(nomearquivo, argv[2]);
				inputset = 1;
			}
		}
	}
	if(!inputset){
		printf("Digite o nome do arquivo .class\n");
		scanf("%s",nomearquivo);
	}
	if(!outset){
		printf("Digite o nome do arquivo de saida\n");
		scanf("%s",outputName);
	}

	/*Chamada da função que realiza a leitura do arquivo .class*/
	jvm->classes = InserirFim_classes(jvm->classes,lerArquivo(nomearquivo));

	int opcao = menu();
	if (opcao == 1) {
		FILE *outputFile = fopen(outputName, "w");
		if (jvm->classes->arquivoClass != NULL && outputFile != NULL) {
			imprimirClassFile(jvm->classes->arquivoClass, stdout);
			imprimirClassFile(jvm->classes->arquivoClass, outputFile);
			fclose(outputFile);
		} else {
			printf("Erro!");
			exit(1);
		}
	}else if (opcao == 2) {
		executarJVM();
		exit(1);
	}else if(opcao == 0){
		exit(1);
	}else{
		printf("Erro!");
	}


	free(jvm->classes->arquivoClass);
	free(jvm->classes);
	free(jvm);
	free(nomearquivo);
	free(outputName);


	/*Retorno da função main*/
	return (0);
}
