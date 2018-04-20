/*     1º Exercício Programa da disciplina de Cálculo Numérico     */ 
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
double **A;        //armazena a matriz	
double *xResults;  //armazena os resultados do SL
int ordem;  	   //armazena a ordem da matriz 
double *coeficientes;         //coeficientes da equação algébrica
double *coeficienteInverso;   //coeficientes invertidos da equação algébrica

//###################### Conversão entre Bases ######################
void converte(double num, int base) {
	//Recebe um número decimal e a base numérica para a qual deve ser convertido
	//e printa o número convertido com precisão de até 20 casas decimais
	int quociente = (int)num;   //variável usada nas iterações da parte inteira
	int i = 0;                  //variável auxiliar 
	int aux = 0;                //variável auxiliar
	int precisao = 20;
	long double parteInteira = 0;    
	double parteFracionaria = num-(double)quociente;

	if(base == 2 || base == 8) {
		while(quociente >= base) {      //Cálculo da parte inteira da conversão
			parteInteira += quociente % base * pow(10, i);
			quociente = quociente / base;
			i++;
		}
		parteInteira += quociente % base * pow(10, i); 
		printf("%lld.", (long long int)parteInteira);       
	}	

	else if(base == 16) {
		parteFracionaria = num - (int)num;
		printf("%X.", (int)num);
	}

	for(i = 1; i <= precisao; i++) {    //Imprime a parte fracionária da conversão
		parteFracionaria *= base;            
		aux = (int)parteFracionaria;   //Obtenção da parte inteira
		
		if(parteFracionaria > 0) {
			if(base == 2 || base == 8)
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

//###################### Método de Jordan ######################
double **alocaMatriz(int nLinhas, int nColunas) {
	//Se houver memória disponível, aloca uma matriz com nLinhas  e nColunas e 
	//devolve o endereço base da matriz, caso contrário devolve um ponterio nulo
	double **m;
	int i, j;
	m = malloc(sizeof(double *)*nLinhas);
	if(m == NULL)  //Memória insuficiente
		return NULL;
	
	for(i = 0; i < nLinhas; i++) {
		m[i] = malloc(sizeof(double)*nColunas);
		if(m[i] == NULL) {
			for(j = 0; j < i; j++) {
				free(m[j]);
			}
			free(m);
			return NULL;	
		}
	}
	return m;
}//alocaMatriz

double *alocaX(int ordem) {
	//Se houver memória disponível, aloca espaço na memória para armazenar os 
	//resultados do SL e retorna o endereço base da memória alocada, caso 
	//contrário devolve um ponterio nulo
	double *x;
	x = malloc(sizeof(double)*ordem); //Aloca os resultados do SL
	
	if(x == NULL) {  //Memória insuficiente
		free(x);
		return NULL;
	}
	return x;
}

void imprimeMatriz(double **m, int nLinhas, int nColunas) {
	// Imprime valores de uma matriz com nLinhas e nColunas 
	int i, j;
	printf("\n\t\t|----- Matriz -----|\n\n");
	for(i = 0; i < nLinhas; i++) {
		for(j = 0; j < nColunas; j++) {
			printf("%10.3lf ", m[i][j]);
		}
		printf("\n\n");
	}
}//imprimeMatriz

int lerMatriz() {
	//Lê o arquivo de uma matriz a partir do caminho indicado pelo usuário
	//e aloca a matriz e as variáveis do resultado do SL 
	FILE *arquivo = NULL;
	char caminhoDoArquivo[50];
	int i, j;
	
	//Esvazia o buffer do stdin
	while((getchar()) != '\n' && getchar() != EOF){}

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

	A = alocaMatriz(ordem, ordem+1);//Aloca a matriz lida do arquivo
	xResults = alocaX(ordem);	    //Aloca memória para os resultados do SL
	if(A == NULL || xResults == NULL) {
		printf("Memória insuficiente!");		
		return 1;
	}

	for(i = 0; i < ordem; i++) {	//Armazena a matriz
		for(j = 0; j < ordem + 1; j++) 
			fscanf(arquivo, "%lf", &A[i][j]);				
	}
	fclose(arquivo);
}//lerMatriz

int subRetroativa(double **m, int n, double x[]){   
	//Recebe uma matriz, a quantidade de variáveis e um vetor para
	//armazenar os resultados, e então resolve o SL aplicando o método de 
	//substituição retroativa
	int tipo = 0;
	int i, j;
	double soma;
	
	for(i = n-1; i >= 0; i--){
		soma = 0;
		for(j = i+1; j < n; j++){
			soma += m[i][j] * x[j];
		}
		if(fabs(m[i][i]) < EPSILON){
			if(fabs(m[i][n] - soma) < EPSILON){ // x[i] é variável livre 
				x[i] = 0;
				tipo = 1;				
			}
			else {
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
	//Recebe m, a matriz aumentada de um SL com n variáveis e 
	//transforma m na matriz aumentada de um SL ti equivalente
	int i, j, k;
	double mult; 
	double *aux;
	
	for(i = 0; i <= n-1; i++) {
		if(m[i][i] == 0) {
			j = i + 1;
			while(j < n && m[i][j] == 0) 
				j++;
			if(j < n) {
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

//###################### Teorema de Lagrange ######################
double *alocaCoeficientes(int grau) {
	//Se houver memória disponível, aloca espaço na memória para armazenar os 
	//coeficientes da equação algébrica e retorna o endereço base da memória alocada, 
	//caso contrário devolve um ponterio nulo
	double *coef;
	coef = malloc(sizeof(double)*grau+1);

	if(coef == NULL) { //Memória insuficiente
		free(coef);
		return NULL;
	}
	return coef;
}

double *alocaCoeficienteInverso(int grau) {
	//Se houver memória disponível, aloca espaço na memória para armazenar os 
	//coeficientes invertidos da equação algébrica e retorna o endereço base da 
	//memória alocada, caso contrário devolve um ponterio nulo
	double *coefInverso;
	coefInverso = malloc(sizeof(double *)*grau+1);

	if(coefInverso == NULL) { //Memória insuficiente
		free(coefInverso);
		return NULL;
	}
	return coefInverso;
}

int calcula_k(int n, double *coeficientes) {
	int expoente = n;
	int i;
	//Percorre o vetor de coeficientes
	for(i = 0; i < n+1; i++) {
		//Se o coeficiente for negativo, k recebe o expoente do coeficiente
		if(coeficientes[i] < 0)
			return expoente;
		expoente--;
	}
}//calcula_k

double calcula_b(int n, double *coeficientes) {
	int menor = 0;
	int i;
	//Percorre o vetor de coeficientes
	for(i = 0; i < n+1; i++) {
		//Se o coeficiente atual for o mais negativo ele é armazenado
		if(coeficientes[i] < menor)
			menor = coeficientes[i];
	}
	return fabs(menor);
}

double calcula_l(int n, double k, double b, double an) {
	printf("L = %.6f\n", 1.0 + pow(b/an, 1.0/(n-k)));
	return 1.0 + pow(b/an, 1.0/(n-k));
}

void inverteSinal(int n, double *coeficientes) {
	//Inverte o sinal dos coeficientes de expoente ímpar
	int i;
	for(i = 0; i < n+1; i++) {	
		coeficientes[i] *= pow(-1, i);
	}
}

int Lagrange(int n, double *coeficientes, double *coeficienteInverso) {
	//Recebe o grau do polinômio e seus coeficientes e calcula um intervalo
	//para as raízes negativas e positivas
	double k[4];
	double b[4];
	double an[2];
	double L[4];
	int i, l;
	
	//Obtém o polinômio invertido para calcular L1 e L3
	int iterator = 0; //variável auxiliar
	for(i = n; i > -1; i--) {
		coeficienteInverso[iterator] = coeficientes[i];
		iterator++;
	}

	//Se o an do polinômio invertido for negativo multiplica o polinômio por -1
	if(coeficienteInverso[0] < 0) {
		for(i = 0; i < n+1; i++) {
			coeficienteInverso[i] *= -1;
		}
	}

	an[0] = coeficientes[0]; 
	an[1] = coeficienteInverso[0]; 
	//Calcula L0, L1, L2 e L3
	for(l = 0; l < 4; l++) {
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
	printf("\nIntervalo das raízes negativas: %.4lf <= x- <= %.6lf\n", -L[2], -1/L[3]);
	printf("Intervalo das raízes positivas: %.4lf <= x+ <= %.6lf\n\n", 1/L[1], L[0]);

	//O sinal de alguns coeficientes estavam invertidos após calcular Lagrange, portanto
	//é necessário desinvertê-los para obter o polômio original de volta
	inverteSinal(n, coeficientes);

}//Teorema de Lagrange

double resolvePolinomio(int grau, double *coeficientes, double fx) {
	double resultado = 0;
	int i;
	for(i = 0; i < grau; i++)
		resultado += coeficientes[i] * pow(fx, grau-i);
	return resultado + coeficientes[grau];
}//resolvePolinomio

//###################### Método da Bisseção ######################
double bissecao(double a, double b, int grau, double *coeficientes, double TOL, int n_iteracoes) {
	//Recebe um intervalo a~b, o grau do polinômio, os coeficientes, a tolerância máxima do erro
	//e o número máximo de iterações, então calcula uma aproximação para uma raiz contida no
	//intervalo
	
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
	double numero              = 0;   //número a ser convertido
	int tipo			       = 0;	  //tipo do sistema linear
	int grau	               = 0;   //grau da equação algébrica
	double raiz			       = 0;   //raiz aproximada do Método da Bisseção
	double intervalo[2];        //intervalo para o Teorema de Bolzano 
	double resultInterv[2];     //Resultado do cálculo do polinômio no intervalo	
	char opcao;			        //opção selecionada
	int i;

	while(opcao != 'F') {
		printf("\t\t|-----------------------|\n");
		printf("\t\t|          Menu         |\n");
		printf("\t\t|-----------------------|\n\n");
		printf("\t\tC - Conversão\n");
		printf("\t\tS - Sistema Linear\n");
		printf("\t\tE - Equação Algébrica\n");
		printf("\t\tF - Finalizar\n");

		opcao = getchar();
		system("cls");    

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

				imprimeMatriz(A, ordem, ordem+1);
				Jordan(A, ordem);
				tipo = subRetroativa(A, ordem, xResults);

				imprimeMatriz(A, ordem, ordem+1);
				for(i = 0; i < ordem; i++)
					printf("x[%d] = %9.3lf\n", i+1, xResults[i]);
				printf("Tipo = %d\n", tipo);

				if(tipo == 2)
					printf("Sistema linear incompativel.\n");	
				else
					printf("Sistema linear %sdeterminado.\n", tipo ? "in" : "");
				
				printf("Pressione Enter para continuar... \n\n");
				break;

			case 'E':
				printf("Informe o grau da equação algébrica: \n");
				scanf("%d", &grau);

				if(coeficientes == NULL) { 
					coeficientes = alocaCoeficientes(grau);
					coeficienteInverso = alocaCoeficienteInverso(grau);
				}

				for(i = 0; i < grau+1; i++) {
					printf("Informe o coeficiente a%d:\n", grau-i);
					scanf("%lf", &coeficientes[i]);

					//Verifica se an é maior que zero
					while(coeficientes[0] < 1) {
						printf("a%d deve ser maior que 0!\n", grau);
						printf("Informe o coeficiente a%d:\n", grau);
						scanf("%lf", &coeficientes[0]);
					}

					//Verifica se a0 é diferente de zero
					while(coeficientes[grau] == 0) {
						printf("a0 deve ser maior que 0!\n");
						printf("Informe o coeficiente a0:\n");
						scanf("%lf", &coeficientes[grau]);
					}
				}				
				Lagrange(grau, coeficientes, coeficienteInverso);

				printf("Informe o primeiro valor do intervalo (ex: 0): ");
				scanf("%lf", &intervalo[0]);

				printf("Informe o segundo valor do intervalo (ex: 3): ");
				scanf("%lf", &intervalo[1]);

				resultInterv[0] = resolvePolinomio(grau, coeficientes, intervalo[0]);
				resultInterv[1] = resolvePolinomio(grau, coeficientes, intervalo[1]);

				printf("Resultado[%.2lf] = %.2lf\n", intervalo[0], resultInterv[0]);
				printf("Resultado[%.2lf] = %.2lf\n", intervalo[1], resultInterv[1]);

				//Teorema de Bolzano
				if(resultInterv[0] * resultInterv[1] < 0) { 
					printf("O intervalo contém um número ímpar de raízes!\n");
					raiz = bissecao(intervalo[0], intervalo[1], grau, coeficientes, 0.00000001, 1000);
					printf("A raiz aproximada contida no intervalo [%.2lf, %.2lf] é: %.10lf\n\n", intervalo[0], intervalo[1], raiz);
				}
				else
					printf("O intervalo contém um número par de raízes!\n\n");
				break;

			case 'F':
				return 0;

			default:
				printf("Opção inválida, tente novamente!\n");
		}
		//Esvazia o buffer do stdin
		while((getchar()) != '\n' && getchar() != EOF){}
	}
	return 0;
}