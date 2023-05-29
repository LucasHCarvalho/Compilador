//Desenvolvido por Lucas Henrique de Carvalho Pinto

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

uint8_t entrada[255];
uint8_t a, pos = 0;
int p, n = 0, z = 0;

void imprime(){
	printf("Acumulador: %u\nPC: %d\nFlag N: %d\nFlag Z: %d\n",a,p,n,z);
	for(int i = 0; i < 256; i++){
		printf("%u ", entrada[i]);
	}
}

int interpreta(){
	bool ativo = true;
	
	if(entrada[0] == 42){
		a = entrada[1];
		p = entrada[2];
		
		imprime();
		printf("\n\n");
	}
	else{
		printf("Header inválido!");
		return 1;
	}

	while(ativo){
		pos = 0;
		if(a == 0){
			z = 1;
			n = 0;
		}
		else if(a > 127){
			z = 0;
			n = 1;
		}
		else{
			z = 0;
			n = 0;
		}
		
		//Fim
		if(entrada[p] == 240 || p >= 256){
			ativo = false;
		}
		//STA
		else if(entrada[p] == 16){
			p += 1;
			pos = entrada[p];
			
			entrada[pos] = a;
		}
		//LDA
		else if(entrada[p] == 32){
			p += 1;
			pos = entrada[p];
			
			a = entrada[pos];
		}
		//ADD
		else if(entrada[p] == 48){
			p += 1;
			pos = entrada[p];
			
			a += entrada[pos];
		}
		//OR
		else if(entrada[p] == 64){
			p += 1;
			pos = entrada[p];
			
			a = a | pos;
		}
		//AND
		else if(entrada[p] == 80){
			p += 1;
			pos = entrada[p];
			
			a = a & pos;
		}
		//NOT
		else if(entrada[p] == 96){
			a = (a * -1) - 1;
		}
		//JMP
		else if(entrada[p] == 128){
			p += 1;
			p = entrada[p] - 1;
		}
		//JN
		else if(entrada[p] == 144){
			if(n == 1){
				p += 1;
				p = entrada[p] -1;
			}
		}
		//JZ
		else if(entrada[p] == 160){
			if(z == 1){
				p += 1;
				p = entrada[p] -1;
			}
		}

		p++;
	}

	imprime();
	return 0;
}

int main(int argc, char **argv) {
  //valida Argumentos passados, pedindo necessáriamente 2
  if(argc != 2){
		printf("Favor inserir 2 argumentos:\nEx.: main entrada.nar\n");
		return 1;
	}    
  
	FILE* input_file = fopen(argv[1], "rb");
	//Tendo os arquivos verifica se pode abrir o arquivo passado
	if(!input_file){
		printf("Arquivo inválido\n");
		return 1;
	} 
	//posiciona no começo do arquivo
	fseek(input_file, 0, SEEK_SET);
	//insere na variavel entrada
	fread(&entrada, 255, 1, input_file);

	interpreta();
	
	FILE *output_file = fopen("saida.nar", "wb");
	fwrite(entrada, 1, 255, output_file);

  exit(EXIT_SUCCESS);
}
