#include <stdio.h>

typedef enum Token {
  Mais,
	Menos,
	Multiplicacao,
	Divisao,
	Indeterminado,
	Nulo
} Token;

typedef struct Digito {
  Token token;
  char caracter;
}Digito;

typedef Digito * Texto;

char tokenizar(char *texto, int tamanho){
	Texto novo;
  novo = malloc (sizeof(Digito));
  
	int i = 0;
	for (i = 0; i < tamanho; i++) {
    if(texto[i] == ' ' || texto[i] == '\n' || texto[i] == '\0')
			novo->token = Nulo;
		else if(texto[i] == '+')
			novo->token = Mais;
		else if(texto[i] == '-')
			novo->token = Menos;
		else if(texto[i] == '*')
			novo->token = Multiplicacao;
		else if(texto[i] == '/')
			novo->token = Divisao;
		else
			novo->token = Indeterminado;
		
		novo->caracter = texto[i];

		if(novo->token != Nulo)
			printf("\t%s -> %c,\n", 
				(novo->token == Mais) ? "Mais" :
	      (novo->token == Menos) ? "Menos" :
				(novo->token == Multiplicacao) ? "Multiplicacao" :
	      (novo->token == Divisao) ? "Divisao" :
       	(novo->token == Indeterminado) ? "Indeterminado" : "", novo->caracter);
  }
	
	free(novo);
}

int main(int argc, char **argv) {
	if(argc != 2){
		printf("Favor inserir 2 argumentos:\nEx.: main mbr.bin\n");
		return 1;
	}    
  
  FILE* output_file = fopen(argv[1], "r");

	if(!output_file){
    printf("Arquivo inválido\n");
		return 1;
	}

	int tamanho;
  fseek(output_file, 0L, SEEK_END);
  tamanho = ftell(output_file);

  int i = 0, numero;
  char arquivo[tamanho];

  fseek(output_file, 0, SEEK_SET);
  fread(&arquivo, tamanho, 1, output_file);
	
  printf("Token: [\n");
		tokenizar(arquivo, tamanho);
	printf("]");
	
  return 0;
}
