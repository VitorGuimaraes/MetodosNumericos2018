/*     1º Exercício Programa da disciplina de Métodos Numéricos */ 
/*												                */ 
/* Implementação de Conversão entre bases numéricas, Método     */
/* de Jordan, Teorema de Lagrange, Teorema de Bolzano e Método  */
/* da Bisseção                                                  */ 
/* Implementado pelos alunos:			                 		*/
/* ***, ***, Leonardo Holanda, Thalia Freitas e Vitor Guimarães */
/* Data: 20/04/2018									     		*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#define EPSILON 0.00000001 

//Variáveis Globais
double **A; //armazena a matriz	
double *p;  //armazena a matriz de ponteiros
int ordem;  //armazena a ordem da matriz 

void converte(double num, int base){
	int quociente = (int)num;   //variavel usada nas iteracoes da parte inteira
	int i = 0;                  //variavel auxiliar 
	int aux = 0;                //varivel auxiliar
	int precisao = 20;
	double parteInteira = 0;    
	double parteFracionaria = num-(double)quociente;

	if((base == 2) || (base == 8)){
		while(quociente >= base){      //Cálculo da parte inteira da conversão
			parteInteira += quociente%base*pow(10, i);
			quociente = quociente/base;
			i++;
		}
		parteInteira += quociente%base*pow(10, i); 
		printf("%d.", (int)parteInteira);       
	}	

	if(base == 16){
		parteFracionaria = num-(int)num;
		printf("%X.", (int)num);
	}

	for(i = 1; i <= precisao; i++){    //Imprime a parte fracionária da conversão
		parteFracionaria *= base;            
		aux = (int)parteFracionaria;   //Obtenção da parte inteira
		
		if(parteFracionaria > 0){
			if((base == 2) || (base == 8))
				printf("%d", (int)parteFracionaria);
			else if(base == 16)
				printf("%X", (int)parteFracionaria);

			parteFracionaria -= aux;
		}
		else
			printf("0");

		if(parteFracionaria == 0)
			break;
	}
	printf("\n");
}//converte

double **alocaMatriz(int l, int c){
	//Se houver memoria disponível, aloca uma matriz com l linhas e c colunas e 
	//devolve o endereço base da matriz, caso contrario devolve um ponterio nulo
	
	double **m;
	m = malloc(sizeof(double *) * l);
	if(m == NULL){
		//Memória insuficiente
		return NULL;
	}
	for(int i = 0; i < l; i++){
		m[i] = malloc(sizeof(double) * c);
		if(m[i] == NULL){
			for(int j = 0; j < i; j++){
				free(m[j]);
			}
			free(m);
			return NULL;	
		}
	}
	return m;
}//alocaMatriz

void imprimeMatriz(double **m, int l, int c){
	/* Imprime valores de uma matriz de double com l linhas e c colunas */
	for(int i = 0; i < l; i++){
		for(int j = 0; j < c; j++){
			printf("%10.3lf ", m[i][j]);
		}
		printf("\n\n");
	}
}//imprimeMatriz

void lerMatriz(){

	FILE *arquivo = NULL;
	char caminhoDoArquivo[50];

	while(arquivo == NULL){
		printf("Insira o caminho do arquivo e pressione Enter: \n");
		fgets(caminhoDoArquivo, sizeof(caminhoDoArquivo), stdin);
		//Remove o último caractere do caminho, pois o fgets armazena a quebra de linha '\n'
		char *p_chr = strchr(caminhoDoArquivo, '\n'); 
		if(p_chr != NULL) 
			*p_chr = '\0';
		arquivo = fopen(caminhoDoArquivo, "r");
	}

	fscanf(arquivo, "%d", &ordem); //Lê a primeira linha do arquivo

	A = alocaMatriz(ordem, ordem + 1);
	p = malloc(sizeof(double)*ordem); //Matriz de ponteiros
	if(A == NULL || p == NULL)
		printf("Memória insuficiente!");		
	
	for(int i = 0; i < ordem; i++){
		for(int j = 0; j < ordem + 1; j++){
			fscanf(arquivo, "%lf", &A[i][j]);				
		}
	}
	fclose(arquivo);
}//lerMatriz

void Jordan(){
	
}

//################################ Lagrange #################################
int calcula_k(int n, double *coeficientes){
	int expoente = n;
	//Percorre o vetor de coeficientes
	for(int i = 0; i < n+1; i++){
		//Se o coeficiente for negativo, k recebe o expoente do coeficiente
		if(coeficientes[i] < 0)
			return expoente;
		expoente--;
	}
}

double calcula_b(int n, double *coeficientes){
	int menor = 0;
	//Percorre o vetor de coeficientes
	for(int i = 0; i < n+1; i++){
		//Se o coeficiente atual for o mais negativo ele é armazenado
		if(coeficientes[i] < menor)
			menor = coeficientes[i];
	}
	return fabs(menor);
}

double calcula_l(int n, double k, double b, double an){
	// printf("L = %.4f\n", 1.0 + pow(b/an, 1.0/(n-k)));
	return 1.0 + pow(b/an, 1.0/(n-k));
}

//Inverte o sinal dos coeficientes a2, a4, ...
void inverteSinal(int n, double *coeficientes){
	for(int i = 0; i < n+1; i++){
		if(i%2 != 0)
			coeficientes[i] *= -1;
	}
}

void Lagrange(int n, double *coeficientes){

	double k[4];
	double b[4];
	double an[2];
	double L[4];
	double *coeficienteInverso = 0;
	
	coeficienteInverso = (double*)malloc(sizeof(double)*n+1);

	//Obtém o polinômio invertido para calcular L1 e L3
	int iterator = 0; //variável auxiliar
	for(int i = n; i > -1; i--){
		coeficienteInverso[iterator] = coeficientes[i];
		iterator++;
	}

	//Se o an do polinômio invertido for negativo, multiplica o polinômio por -1
	if(coeficienteInverso[0] < 0){
		for(int i = 0; i < n+1; i++){
			coeficienteInverso[i] *= -1;
		}
	}

	an[0] = coeficientes[0]; 
	an[1] = coeficienteInverso[0]; 

	//Calcula L0, L1, L2 e L3
	for(int l = 0; l < 4; l++){
		if(l == 2) //Quando calcular L2 inverte os sinais da equação
			inverteSinal(n, coeficientes);
		if(l == 3) //Quando calcular L3 inverte os sinais da equação
			inverteSinal(n, coeficienteInverso);

		if(l%2 == 0){ //Calcula k, b e L para L0 e L2
			k[l] = calcula_k(n, coeficientes);
			b[l] = calcula_b(n, coeficientes);
			L[l] = calcula_l(n, k[l], b[l], an[0]);
		}
		else{ 		 //Calcula k, b e L para L1 e L3
			k[l] = calcula_k(n, coeficienteInverso);
			b[l] = calcula_b(n, coeficienteInverso);
			L[l] = calcula_l(n, k[l], b[l], an[1]);
		}
	}
	printf("\nIntervalo das raízes negativas: %.4lf <= x- <= %.4lf\n", -L[2], -1/L[3]);
	printf("Intervalo das raízes positivas: %.4lf <= x+ <= %.4lf\n", 1/L[1], L[0]);
}//Lagrange

int main(){

	setlocale(LC_ALL, "Portuguese");
	double numero        = 0; //número a ser convertido
	int grau	         = 0; //grau da equação algébrica
	double *coeficientes = 0; //coeficienes da equação algébrica
	int verifica		 = 1; //variável auxiliar
	int i 			     = 0; //variável auxiliar
	char opcao;			      //opção selecionada
	
	while(opcao != 'F'){
		system("cls");
		printf("\t\t|-----------------------|\n");
		printf("\t\t|          Menu         |\n");
		printf("\t\t|-----------------------|\n\n");
		printf("\t\tC - Conversão\n");
		printf("\t\tS - Sistema Linear\n");
		printf("\t\tE - Equação Algébrica\n");
		printf("\t\tF - Finalizar\n");

		opcao = getchar();
		switch(opcao){
			case 'C':
				printf("Digite o número a ser convertido: \n");
				scanf("%lf", &numero);	
				printf("\nBinário:\n");
				converte(numero, 2);
				
				printf("\nOctal:\n");
				converte(numero, 8);

				printf("\nHexadecimal:\n");
				converte(numero, 16);
				break;

			case 'S':
				lerMatriz();
				printf("\n\t\t|----- Matriz -----|\n\n");
				imprimeMatriz(A, ordem, ordem+1);
				// gaussSeidel(A, ordem);	
				break;
				
			case 'E':
				printf("Informe o grau da equação algébrica: \n");
				scanf("%d", &grau);

				coeficientes = (double*)malloc(sizeof(double)*grau+1);

				for(i = 0; i < grau+1; i++){
					printf("Informe o coeficiente a%d:\n", grau-i);
					scanf("%lf", &coeficientes[i]);

					//Verifica se an é menor que zero
					if(coeficientes[0] < 0){
						printf("a%d deve ser maior que 0!\n", grau-i);
						verifica = 0;
					}
				}

				while((verifica == 1) && (i < grau+1)){
					printf("Informe o coeficiente a%d:\n", grau-i);
					scanf("%lf", &coeficientes[i]); 
					
					if((coeficientes[i] < 0) && (i < grau) || (coeficientes[grau] == 0)){
						printf("a%d deve ser maior que 0 e a0 deve ser diferente de 0!\n", grau-i);
						verifica = 0;
					}
					i++;
				}

				if(verifica == 1)
					Lagrange(grau, coeficientes);
				break;

			case 'F':
				return 0;
			default:
				printf("Opção inválida, tente novamente!");
		}
		opcao = getchar();
	}
	return 0;
}