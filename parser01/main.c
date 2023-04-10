#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH 512

typedef enum Token { Inteiro, Decimal, Mais, Nulo, Indeterminado } Token;

typedef struct Simbolo {
  Token token;
  char texto[LENGTH];
} Simbolo;

struct Simbolo *vetor;

Simbolo limpaTexto(Simbolo token) {
  for (int i = 0; i < LENGTH; i++)
    token.texto[i] = NULL;

  return token;
}

void imprimeToken(Simbolo token) {
  for (int i = 0; i < LENGTH; i++)
    printf("%c", token.texto[i]);
}

int coletaNumeros(char *texto, int inicio, int tamanho) {
  int final = inicio - 1;

  for (int i = inicio; i < tamanho; i++) {
    if (isdigit(texto[i]))
      final = i;
    else
      i = tamanho;
  }

  return final;
}

int tokenizar(char *texto, int tamanho) {
  int pos = 0;
  int l = 0;
  Simbolo novo;

  for (int i = 0; i < tamanho; i++) {
    novo = limpaTexto(novo);
    l = 0;

    if (texto[i] == ' ' || texto[i] == '\n' || texto[i] == '\0' ||
        texto[i] == '\t') {

    } else {
      if (texto[i] == '+') {
        novo.token = Mais;
        novo.texto[0] = texto[i];
      } else if (isdigit(texto[i])) {
        novo.token = Inteiro;

        int final = coletaNumeros(texto, i, tamanho);
        int anterior = final, pontos = 0;

        for (int j = i; j <= final; j++) {
          novo.texto[l] = texto[j];
          l++;
        }

        i = final;

        do {
          anterior = final;
          if (texto[i + 1] == '.') {
            pontos++;
            i++;
            novo.token = Decimal;
            novo.texto[l] = texto[i];

            final = coletaNumeros(texto, i + 1, tamanho);
            for (int r = i; r <= final; r++) {
              novo.texto[l] = texto[r];
              l++;
            }

            i = final;
          } else if (isdigit(texto[i + 1]))
            novo.token = Indeterminado;
        } while (anterior < final);

        if (pontos > 1)
          novo.token = Indeterminado;
      } else {
        novo.token = Indeterminado;
        novo.texto[0] = texto[i];
      }
      vetor[pos] = novo;
      pos++;
    }
  }

  return pos;
}

int transformaInteiro(int pos) {
  int num = 0;

  for (int i = 0; i < LENGTH; i++)
    if (vetor[pos].texto[i] != NULL) {
      num *= 10;
      num += vetor[pos].texto[i] - '0';
    }

  return num;
}

void calculaInteiro() {
  int num1 = transformaInteiro(0), num2 = transformaInteiro(2);

  if (vetor[1].token == 2)
    printf("%d", num1 + num2);
}

void calculaFloat() {
  float num1 = atof(vetor[0].texto), num2 = atof(vetor[2].texto);

  if (vetor[1].token == 2)
    printf("%f", num1 + num2);
}

int validaFormato(char *texto, int nCaracteres) {
  int tamanho = tokenizar(texto, nCaracteres);
  char tipoNum = 'i';
  for (int i = 0; i < tamanho; i++) {
    if (vetor[0].token > 1 || tamanho == 2 || tamanho > 3 ||
        vetor[i].token > 3) {
      printf("Erro");
      return 1;
    } else if (tamanho == 1) {
      imprimeToken(vetor[i]);
      return 0;
    }

    if (vetor[i].token == Decimal)
      tipoNum = 'd';
  }

  if (tipoNum == 'i')
    calculaInteiro();
  else
    calculaFloat();

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Favor inserir 2 argumentos:\nEx.:./main texto.txt\n");
    return 1;
  }

  FILE *output_file = fopen(argv[1], "r");

  if (!output_file) {
    printf("Arquivo inválido\n");
    return 1;
  }

  fseek(output_file, 0L, SEEK_END);
  int tamanho = ftell(output_file);

  char arquivo[tamanho];

  vetor = (Simbolo *)malloc(tamanho * sizeof(Simbolo));

  fseek(output_file, 0, SEEK_SET);
  fread(&arquivo, tamanho, 1, output_file);

  printf("Entrada: ");
  if (tamanho == 0)
    printf("ε");
  for (int i = 0; i < tamanho; i++)
    printf("%c", arquivo[i]);

  printf("\nResultado: ");
  if (tamanho == 0)
    printf("0");
  else
    validaFormato(arquivo, tamanho);

  free(vetor);
  return 0;
}
