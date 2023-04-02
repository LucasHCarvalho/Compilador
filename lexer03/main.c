#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH 512

typedef enum Token {
  Mais,
  Menos,
  Multiplicacao,
  Potencia,
  Divisao,
  Indeterminado,
  Inteiro,
  Decimal,
  Nulo
} Token;

typedef struct Simbolo {
  Token token;
  char texto[LENGTH];
} Simbolo;

typedef Simbolo *Indicador;

void limpaTexto(Indicador token) {
  for (int i = 0; i < LENGTH; i++)
    token->texto[i] = NULL;
}

void imprimeToken(Indicador token) {
  if (token->token != Nulo) {
    printf("\t%s -> ", (token->token == Mais)            ? "Mais"
                       : (token->token == Menos)         ? "Menos"
                       : (token->token == Multiplicacao) ? "Multiplicacao"
                       : (token->token == Potencia)      ? "Potencia"
                       : (token->token == Divisao)       ? "Divisao"
                       : (token->token == Indeterminado) ? "Indeterminado"
                       : (token->token == Inteiro)       ? "Inteiro"
                       : (token->token == Decimal)       ? "Decimal"
                                                         : "");
    for (int i = 0; i < LENGTH; i++)
      printf("%c", token->texto[i]);
    printf("\n");
  }
}

int ultimoNumero(char *texto, int inicio, int tamanho) {
  int final = inicio;

  for (int i = inicio; i < tamanho; i++) {
    if (isdigit(texto[i]))
      final = i;
    else
      i = tamanho;
  }

  return final;
}

char tokenizar(char *texto, int tamanho) {
  Indicador novo;
  novo = malloc(sizeof(Simbolo));

  for (int i = 0; i < tamanho; i++) {
    limpaTexto(novo);

    if (texto[i] == ' ' || texto[i] == '\n' || texto[i] == '\0')
      novo->token = Nulo;
    else if (texto[i] == '+') {
      novo->token = Mais;
      novo->texto[0] = texto[i];
    } else if (texto[i] == '-') {
      novo->token = Menos;
      novo->texto[0] = texto[i];
    } else if (texto[i] == '*') {
      novo->texto[0] = texto[i];
      if (texto[i + 1] == '*') {
        novo->token = Potencia;
        novo->texto[1] = texto[i];
        i++;
      } else
        novo->token = Multiplicacao;
    } else if (texto[i] == '/') {
      novo->token = Divisao;
      novo->texto[0] = texto[i];
    } else if (isdigit(texto[i])) {
      novo->token = Inteiro;

      int final = verificaNumero(texto, i, tamanho);

      for (int j = i; j <= final; j++) {
        novo->texto[j] = texto[j];
      }

      i = final;

      if (texto[i + 1] == '.') {
        i++;
        novo->token = Decimal;
        novo->texto[i] = texto[i];
				
        final = ultimoNumero(texto, i+1, tamanho);
        for (int r = i; r <= final; r++) {
          novo->texto[r] = texto[r];
        }

        i = final;
      }
    } else {
      novo->token = Indeterminado;
      novo->texto[0] = texto[i];
    }

    imprimeToken(novo);
  }

  free(novo);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Favor inserir 2 argumentos:\nEx.: texto.txt\n");
    return 1;
  }

  FILE *output_file = fopen(argv[1], "r");

  if (!output_file) {
    printf("Arquivo inválido\n");
    return 1;
  }

  int tamanho;
  fseek(output_file, 0L, SEEK_END);
  tamanho = ftell(output_file);

  char arquivo[tamanho];

  fseek(output_file, 0, SEEK_SET);
  fread(&arquivo, tamanho, 1, output_file);

  printf("Token: [\n");
  tokenizar(arquivo, tamanho);
  printf("]");

  return 0;
}
