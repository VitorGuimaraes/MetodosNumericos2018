/*     1º Exercício Programa da disciplina de Métodos Numéricos    */ 
/*												                   */ 
/* Implementação de Conversão entre bases numéricas, Método        */
/* de Jordan, Teorema de Lagrange, Teorema de Bolzano e Método     */
/* da Bisseção                                                     */ 
/* Implementado pelos alunos:			                 		   */
/* Jorge Girão, Leonardo Holanda, Thalia Freitas e Vitor Guimarães */
/* Data: 20/04/2018									     		   */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>

#define EPSILON 0.000001

//Variáveis Globais
double **A; //armazena a matriz	
double *x;  //armazena os resultados do SL
int ordem;  //armazena a ordem da matriz 

//################################ Conversão entre Bases #################################
void converte(double num, int base) {
	int quociente = (int)num;   //variável usada nas iterações da parte inteira
	int i = 0;                  //variável auxiliar 
	int aux = 0;                //variável auxiliar
	int precisao = 20;
	long double parteInteira = 0;    
	double parteFracionaria = num-(double)quociente;

	if((base == 2) || (base == 8)) {
		while(quociente >= base) {      //Cálculo da parte inteira da conversão
			parteInteira += quociente % base * pow(10, i);
			quociente = quociente/base;
			i++;
		}
		parteInteira += quociente%base*pow(10, i); 
		printf("%lld.", (long long int)parteInteira);       
	}	

	if(base == 16) {
		parteFracionaria = num-(int)num;
		printf("%X.", (int)num);
	}

	for(i = 1; i <= precisao; i++) {    //Imprime a parte fracionária da conversão
		parteFracionaria *= base;            
		aux = (int)parteFracionaria;   //Obtenção da parte inteira
		
		if(parteFracionaria > 0) {
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

double **alocaMatriz(int row, int col) {
	//Se houver memória disponível, aloca uma matriz com l linhas e c colunas e 
	//devolve o endereço base da matriz, caso contrário devolve um ponterio nulo
	
	double **m;
	m = malloc(sizeof(double *) * row);
	if(m == NULL)  //Memória insuficiente
		return NULL;
	
	for(int i = 0; i < row; i++) {
		m[i] = malloc(sizeof(double) * col);
		if(m[i] == NULL) {
			for(int j = 0; j < i; j++) {
				free(m[j]);
			}
			free(m);
			return NULL;	
		}
	}
	return m;
}//alocaMatriz

//################################ Método de Jordan #################################
void imprimeMatriz(double **m, int l, int c) {
	// Imprime valores de uma matriz de double com l linhas e c colunas 
	printf("\n\t\t|----- Matriz -----|\n\n");
	for(int i = 0; i < l; i++) {
		for(int j = 0; j < c; j++) {
			printf("%10.3lf ", m[i][j]);
		}
		printf("\n\n");
	}
}//imprimeMatriz

int lerMatriz() {
	FILE *arquivo = NULL;
	char caminhoDoArquivo[50];

	while(arquivo == NULL) {
		printf("Insira o caminho do arquivo e pressione Enter: \n");
		fgets(caminhoDoArquivo, sizeof(caminhoDoArquivo), stdin);
		//Remove o último caractere do caminho, pois o fgets armazena a quebra de linha '\n'
		char *p_chr = strchr(caminhoDoArquivo, '\n'); 
		if(p_chr != NULL) 
			*p_chr = '\0';
		arquivo = fopen(caminhoDoArquivo, "r");
	}

	fscanf(arquivo, "%d", &ordem); //Lê a primeira linha do arquivo

	A = alocaMatriz(ordem, ordem+1);
	x = malloc(sizeof(double)*ordem); //Armazena os resultados do SL
	if(A == NULL || x == NULL){
		printf("Memória insuficiente!");		
		return 1;
	}

	for(int i = 0; i < ordem; i++) {
		for(int j = 0; j < ordem + 1; j++) 
			fscanf(arquivo, "%lf", &A[i][j]);				
	}
	fclose(arquivo);
}//lerMatriz

int subRetroativa(double **m, int n, double x[]){   
	int tipo = 0;
	int i, j;
	double soma;
	
	for(i = n-1; i >= 0; i--){
		soma = 0;
		for(j = i + 1; j < n; j++){
			soma += m[i][j] * x[j];
		}
		if(fabs(m[i][i]) < EPSILON){
			if(fabs(m[i][n] - soma) < EPSILON){ // x[i] é uma variável livre 
				x[i] = 0;
				tipo = 1;				
			}
			else{
				return 2; //Sistema Linear Incompativel
			}			
		}
		else{
			x[i] = (m[i][n] - soma) / m[i][i];
		}
	}
	return tipo;
}

void Jordan(double **m, int n) {
	//Recebe m , a matriz aumenta de um SL com n variáveis e 
	//transforma m na matriz aumentada de um SL ti equivalente
	int i, j, k;
	double mult; 
	double *aux;
	
	for(i = 0; i <= n-1; i++) {
		if(m[i][i] == 0) {
			j = i + 1;
			while(j < n && m[i][j] == 0) {
				j++;
			}
			if(j < n){
				aux = m[i];
				m[i] = m[j];
				m[j] = aux;
			}
		}
		if(m[i][i] != 0) {
			for(j = i+1; j < n; j++) {
				mult = -m[j][i] / m[i][i];
				m[j][i] = 0;

				for(k = i+1; k <= n; k++)
					m[j][k] = m[j][k] + m[i][k] * mult;
			}
			if(i != 0) {
				for(j = 0; j < i; j++) {
					mult = -m[j][i] / m[i][i];

					m[j][i] = 0;
					for(k = i+1; k <=n; k++)
						m[j][k] = m[j][k] + m[i][k] * mult;
				}
			}
		}
	}
}//Jordan

//################################ Teorema de Lagrange #################################
int calcula_k(int n, double *coeficientes) {
	int expoente = n;
	//Percorre o vetor de coeficientes
	for(int i = 0; i < n+1; i++) {
		//Se o coeficiente for negativo, k recebe o expoente do coeficiente
		if(coeficientes[i] < 0)
			return expoente;
		expoente--;
	}
}

double calcula_b(int n, double *coeficientes) {
	int menor = 0;
	//Percorre o vetor de coeficientes
	for(int i = 0; i < n+1; i++) {
		//Se o coeficiente atual for o mais negativo ele é armazenado
		if(coeficientes[i] < menor)
			menor = coeficientes[i];
	}
	return fabs(menor);
}

double calcula_l(int n, double k, double b, double an) {
	printf("L = %.4f\n", 1.0 + pow(b/an, 1.0/(n-k)));
	return 1.0 + pow(b/an, 1.0/(n-k));
}

//Inverte o sinal dos coeficientes de expoente ímpar
void inverteSinal(int n, double *coeficientes) {
	for(int i = 0; i < n+1; i++) {	
		coeficientes[i] *= pow(-1, i);
	}
}

int Lagrange(int n, double *coeficientes) {
	double k[4];
	double b[4];
	double an[2];
	double L[4];
	double *coeficienteInverso = 0;
	
	coeficienteInverso = (double*)malloc(sizeof(double)*n+1);
	
	if(coeficienteInverso != NULL){
		//Obtém o polinômio invertido para calcular L1 e L3
		int iterator = 0; //variável auxiliar
		for(int i = n; i > -1; i--) {
			coeficienteInverso[iterator] = coeficientes[i];
			iterator++;
		}

		//Se o an do polinômio invertido for negativo, multiplica o polinômio por -1
		if(coeficienteInverso[0] < 0) {
			for(int i = 0; i < n+1; i++) {
				coeficienteInverso[i] *= -1;
			}
		}

		an[0] = coeficientes[0]; 
		an[1] = coeficienteInverso[0]; 
		//Calcula L0, L1, L2 e L3
		for(int l = 0; l < 4; l++) {
			if(l == 2) //Quando calcular L2 inverte os sinais da equação
				inverteSinal(n, coeficientes);
			if(l == 3) //Quando calcular L3 inverte os sinais da equação
				inverteSinal(n, coeficienteInverso);

			if(l%2 == 0) { //Calcula k, b e L para L0 e L2
				k[l] = calcula_k(n, coeficientes);
				b[l] = calcula_b(n, coeficientes);
				L[l] = calcula_l(n, k[l], b[l], an[0]);
			}
			else { 		 //Calcula k, b e L para L1 e L3
				k[l] = calcula_k(n, coeficienteInverso);
				b[l] = calcula_b(n, coeficienteInverso);
				L[l] = calcula_l(n, k[l], b[l], an[1]);
			}
		}
		free(coeficienteInverso);
		printf("\nIntervalo das raízes negativas: %.4lf <= x- <= %.6lf\n", -L[2], -1/L[3]);
		printf("Intervalo das raízes positivas: %.4lf <= x+ <= %.6lf\n\n", 1/L[1], L[0]);

		//O sinal de alguns coeficientes estavam invertidos após calcular Lagrange, portanto
		//é necessário desinvertê-los para obter o polômio original de volta
		inverteSinal(n, coeficientes);
	}
	else if(coeficienteInverso == NULL){
		printf("Faltou memória (função Lagrange)\n");
		return 1;
	}
}//Teorema de Lagrange

double resolvePolinomio(int grau, double *coeficientes, double fx) {
	double resultado = 0;
	
	for(int i = 0; i < grau; i++)
		resultado += coeficientes[i] * pow(fx, grau-i);
	return resultado + coeficientes[grau];
}

//################################ Método da Bisseção #################################
double bissecao(double a, double b, int grau, double *coeficientes, double TOL, int n_iteracoes) {
	double f_a = 0;
	double f_b = 0;
	double f_m = 0;
	double m   = 0;
	int    i   = 1;

	while(i <= n_iteracoes) { 
		m   = (a+b)/2;
		f_a = resolvePolinomio(grau, coeficientes, a);
		f_b = resolvePolinomio(grau, coeficientes, b);
		f_m = resolvePolinomio(grau, coeficientes, m);

		if(f_a * f_m > 0)
			a = m;
		else
			b = m;
		i++;
		//Condição de parada
		if(((b-a)/2 < TOL) || (i == n_iteracoes))
			return m;
	}
}//Bisseção

int main() {
	setlocale(LC_ALL, "Portuguese");
	double numero        = 0;   //número a ser convertido
	int tipo			 = 0;	//tipo do sistema linear
	int grau	         = 0;   //grau da equação algébrica
	double *coeficientes = 0;   //coeficienes da equação algébrica
	double raiz			 = 0;   //raiz aproximada do Método da Bisseção
	double intervalo[2];        //intervalo para o Teorema de Bolzano 
	double resultInterv[2];     //Resultado do cálculo do polômio no intervalo	
	char opcao;			        //opção selecionada

	while(opcao != 'F') {
		printf("\t\t|-----------------------|\n");
		printf("\t\t|          Menu         |\n");
		printf("\t\t|-----------------------|\n\n");
		printf("\t\tC - Conversão\n");
		printf("\t\tS - Sistema Linear\n");
		printf("\t\tE - Equação Algébrica\n");
		printf("\t\tF - Finalizar\n");

		opcao = getchar();
		system("clear");
		switch(opcao) {
			case 'C':	
				printf("Insira o número a ser convertido: ");
				scanf("%lf", &numero);	
				printf("\nBinário:\n");
				converte(numero, 2);
				
				printf("\nOctal:\n");
				converte(numero, 8);

				printf("\nHexadecimal:\n");
				converte(numero, 16);
				break;

			case 'S':
				lerMatriz(A, ordem, ordem+1);
				if(A != NULL && x != NULL) {
					imprimeMatriz(A, ordem, ordem+1);
					Jordan(A, ordem);
					tipo = subRetroativa(A, ordem, x);

					imprimeMatriz(A, ordem, ordem+1);
					for(int i = 0; i < ordem; i++)
						printf("x[%d] = %9.3lf\n", (i+1), x[i]);

					printf("Tipo = %d\n", tipo);

					if(tipo == 2)
						printf("Sistema linear incompativel.\n");	
					else
						printf("Sistema linear %sdeterminado.\n", tipo ? "in" : "");
				} 
				else {
					printf("Faltou memória... Ahhh! \n");
					return 1;
				}
				free(x);
				printf("Pressione Enter para continuar... \n\n");
				break;

			case 'E':
				printf("Informe o grau da equação algébrica: \n");
				scanf("%d", &grau);

				coeficientes = (double*)malloc(sizeof(double)*grau+1);
				if(coeficientes != NULL) {
					for(int i = 0; i < grau+1; i++) {
						printf("Informe o coeficiente a%d:\n", grau-i);
						scanf("%lf", &coeficientes[i]);

						//Verifica se an é maior que zero
						while(coeficientes[0] < 1) {
							printf("a%d deve ser maior que 0!\n", grau);
							printf("Informe o coeficiente a%d:\n", grau);
							scanf("%lf", &coeficientes[i]);
						}

						//Verifica se a0 é diferente de zero
						while(coeficientes[grau] == 0) {
							printf("a0 deve ser maior que 0!\n");
							printf("Informe o coeficiente a0:\n");
							scanf("%lf", &coeficientes[grau]);
						}
					}				
				}
				else {
					printf("Faltou memória... Ahhh! \n");
					return 1;
				}
				Lagrange(grau, coeficientes);

				printf("Informe o primeiro valor do intervalo (ex: 0): ");
				scanf("%lf", &intervalo[0]);

				printf("Informe o segundo valor do intervalo (ex: 3): ");
				scanf("%lf", &intervalo[1]);

				resultInterv[0] = resolvePolinomio(grau, coeficientes, intervalo[0]);
				resultInterv[1] = resolvePolinomio(grau, coeficientes, intervalo[1]);

				printf("Resultado[%.2lf] = %.2lf\n", intervalo[0], resultInterv[0]);
				printf("Resultado[%.2lf] = %.2lf\n", intervalo[1], resultInterv[1]);

				if(resultInterv[0] * resultInterv[1] < 0) {
					printf("O intervalo contém um número ímpar de raízes!\n");
					raiz = bissecao(intervalo[0], intervalo[1], grau, coeficientes, 0.00000001, 1000);
					printf("A raiz aproximada contida no intervalo [%.2lf, %.2lf] é: %.8lf\n", intervalo[0], intervalo[1], raiz);
				}
				else
					printf("O intervalo contém um número par de raízes!\n\n");
				free(coeficientes);
				break;

			case 'F':
				return 0;

			default:
				printf("Opção inválida, tente novamente!\n");
		}
		opcao = getchar();
	}
	return 0;
}