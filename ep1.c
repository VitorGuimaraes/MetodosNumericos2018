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

//Variáveis Globais
double **A; //armazena a matriz	
double *p;  //armazena a matriz de ponteiros
int ordem;  //armazena a ordem da matriz 

void converte(double num, int base){
	int quociente = (int)num;   //variável usada nas iterações da parte inteira
	int i = 0;                  //variável auxiliar 
	int aux = 0;                //variável auxiliar
	int precisao = 20;
	long double parteInteira = 0;    
	double parteFracionaria = num-(double)quociente;

	if((base == 2) || (base == 8)){
		while(quociente >= base){      //Cálculo da parte inteira da conversão
			parteInteira += quociente % base * pow(10, i);
			quociente = quociente/base;
			i++;
		}
		parteInteira += quociente%base*pow(10, i); 
		printf("%lld.", (long long int)parteInteira);       
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
	//Se houver memória disponível, aloca uma matriz com l linhas e c colunas e 
	//devolve o endereço base da matriz, caso contrário devolve um ponterio nulo
	
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
	// Imprime valores de uma matriz de double com l linhas e c colunas 
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

// void Jordan(){
	
// }

//################################ Teorema de Lagrange #################################
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
	printf("L = %.4f\n", 1.0 + pow(b/an, 1.0/(n-k)));
	return 1.0 + pow(b/an, 1.0/(n-k));
}

//Inverte o sinal dos coeficientes de coeficiente ímpar, ...
void inverteSinal(int n, double *coeficientes){
	int i = 0;
	if(n%2 == 0)	//Se o grau do polinômio for par
		i = 1;		//Começa a inverter a partir do segundo coeficiente
	else			//Senão começa a inverter a partir do primeiro coeficiente
		i = 0;
	for(i; i < n+1; i+=2){	
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
	printf("Intervalo das raízes positivas: %.4lf <= x+ <= %.4lf\n\n", 1/L[1], L[0]);

	//O sinal de an-1, an-3... estavam invertidos após calcular Lagrange, portanto
	//é necessário desinvertê-los para obter o polômio original de volta
	inverteSinal(n, coeficientes);
}//Teorema de Lagrange

double resolvePolinomio(int grau, double *coeficientes, double fx){
	double resultado = 0;
	
	for(int i = 0; i < grau; i++){
		resultado += coeficientes[i] * pow(fx, grau-i);
	} 
	return resultado + coeficientes[grau];
}

double bissecao(double a, double b, int grau, double *coeficientes, double TOL, int n_iteracoes){

	double f_a = 0;
	double f_b = 0;
	double f_m = 0;
	double m   = 0;
	int    i   = 1;

	while(i <= n_iteracoes){ 
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

int main(){

	setlocale(LC_ALL, "Portuguese");
	double numero        = 0;   //número a ser convertido
	int grau	         = 0;   //grau da equação algébrica
	double *coeficientes = 0;   //coeficienes da equação algébrica
	double raiz			 = 0;   //raiz aproximada do Método da Bisseção
	double intervalo[2];        //intervalo para o Teorema de Bolzano 
	double resultInterv[2];     //Resultado do cálculo do polômio no intervalo	
	char opcao;			        //opção selecionada

	while(opcao != 'F'){
		printf("\t\t|-----------------------|\n");
		printf("\t\t|          Menu         |\n");
		printf("\t\t|-----------------------|\n\n");
		printf("\t\tC - Conversão\n");
		printf("\t\tS - Sistema Linear\n");
		printf("\t\tE - Equação Algébrica\n");
		printf("\t\tF - Finalizar\n");

		opcao = getchar();
		
		system("clear");
		switch(opcao){
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
				lerMatriz();
				printf("\n\t\t|----- Matriz -----|\n\n");
				imprimeMatriz(A, ordem, ordem+1);
				// Jordan(A, ordem);	

				break;

			case 'E':
				printf("Informe o grau da equação algébrica: \n");
				scanf("%d", &grau);

				coeficientes = (double*)malloc(sizeof(double)*grau+1);

				for(int i = 0; i < grau+1; i++){
					printf("Informe o coeficiente a%d:\n", grau-i);
					scanf("%lf", &coeficientes[i]);

					//Verifica se an é maior que zero
					while(coeficientes[0] < 1){
						printf("a%d deve ser maior que 0!\n", grau);
						printf("Informe o coeficiente a%d:\n", grau);
						scanf("%lf", &coeficientes[i]);
					}

					//Verifica se a0 é diferente de zero
					while(coeficientes[grau] == 0){
						printf("a0 deve ser maior que 0!\n");
						printf("Informe o coeficiente a0:\n");
						scanf("%lf", &coeficientes[grau]);
					}
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

				if(resultInterv[0] * resultInterv[1] < 0){
					printf("O intervalo contém um número ímpar de raízes!\n");
					// raiz = bissecao(intervalo[0], intervalo[1], grau, coeficientes, 0.00000001, 1000);
					printf("A raiz aproximada contida no intervalo [%.2lf, %.2lf] é: %.8lf\n", intervalo[0], intervalo[1], raiz);
				}
				else
					printf("O intervalo contém um número par de raízes!\n\n");

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
//NÃO ESQUECER DE DESALOCAR OS MALLOC