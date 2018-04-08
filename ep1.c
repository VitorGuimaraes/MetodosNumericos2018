/*     1º Exercício Programa da disciplina de Métodos Numéricos     */ 
/*												                    */ 
/* Implementação de Conversão entre bases numéricas, Método         */
/* de Jordan, Teorema de Lagrange, Teorema de Bolzano e Método      */
/* da Bisseção                                                      */ 
/* Implementado pelos alunos:			                 		    */
/* ***, ***, Leonardo Holanda, Thalia Freitas e Vitor Guimarães     */
/* Data: 20/04/2018									     			*/

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
	double parteFracionaria = num-(double) quociente;

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

int criterioLinhasColunas(double **m, int n){

	double *linha      = 0;
	double *coluna     = 0;
	int criterioLinha  = 1; 
	int criterioColuna = 1;
	
	linha = (double*)malloc(sizeof(double)*n);
	coluna = (double*)malloc(sizeof(double)*n);

	//Calcula o somatório Aij (i!=j) das linhas e colunas
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i != j){
				linha[i]  += m[i][j];
				coluna[i] += m[j][i];
			}
		}
	}

	//Verifica o criterio de linhas e colunas
	for(int i = 0; i < n; i++){
		if((m[i][i] >= linha[i]) && (criterioLinha == 1))
			criterioLinha = 1;
		else
			criterioLinha = 0;

		if((m[i][i] >= coluna[i]) && (criterioColuna == 1))
			criterioColuna = 1;
		else
			criterioColuna = 0;
	}

	if((criterioLinha == 0) && (criterioColuna == 0))
		printf("O sistema linear não satisfaz o critério das linhas nem o critério das colunas\n");
	else
		return 1;
}

void gaussSeidel(double **m, int n){

	if(criterioLinhasColunas(m, n) == 1){
		double *variavel  = NULL; //x, y, z...
		double *antVal    = NULL; //Valor anterior de x, y, z...
		double *sumTermos = 0;    //soma os termos entre o primeiro e ultimo elemento de cada linha
		int cont 		  = 0;	  //contador das iterações do método

		sumTermos = (double*)malloc(sizeof(double)*n);
		variavel  = (double*)malloc(sizeof(double)*n);
		antVal    = (double*)malloc(sizeof(double)*n);

		//As variaveis da equação são iniciadas em 0
		for(int i = 0; i < n; i++){
			variavel[i] = 0;
			antVal[i]   = 1;
		}

		int variacao(double *variavel, double *antVal){
			double variacao[n];
			int variacaoMinima = 0;

			for(int i = 0; i < n; i++){
				variacao[i] = fabs(variavel[i] - antVal[i]);
				
				if(variacao[i] < EPSILON)
					variacaoMinima = 1;
				else
					variacaoMinima = 0;
			}
			return variacaoMinima;
		}

		//Faz as soma dos coeficientes para y=0 e z=0
		//Posteriormente as variáveis são isoladas e x, y, z... sao calculados
		while((variacao(variavel, antVal) == 0) || cont < 1000){
			for(int i = 0; i < n; i++){
				for(int j = 0; j < n; j++){
					if(i != j)
						sumTermos[i] += m[i][j] * -1 * variavel[j];
				}
				
				antVal[i]   = variavel[i];
				variavel[i] = (m[i][n] + sumTermos[i]) / m[i][i];

				//Zera o somador de coeficientes das equações
				for(int i = 0; i < n; i++){
					sumTermos[i] = 0;
				}
			}
			cont++;
		}

		for(int i = 0; i < n; i++){
			printf("Solução aproximada da variável[%d] = %.2lf\n", i, variavel[i]);
		}
		printf("%d iterações\n", cont);
	}
}//gaussSeidel

void newtown(double x0, int grau, double *coeficientes){

	double derivada[grau-1]; //armazena os coeficientes de p'(x)
	double xi    = x0;       //armazena o valor atual de xi
	double xiAnt = xi+1;     //armazena o valor anterior de xi
	int cont     = 0; 		 //conta a quantidade de iterações

	//Faz a derivada p'(x)
	for(int i = 0; i < grau; i++){
		derivada[i] = (grau-i) * coeficientes[i];
	}

	double calculaPx(double xi){
		double  px = 0; //armazena o resultado de p(x)
		
		px = pow(xi, grau); 
		for(int i = 1; i < grau-1; i++)
			px += coeficientes[i] * pow(xi, grau-i); 
		px += coeficientes[grau-1] * xi + 
		coeficientes[grau];
		return px;
	}

	double calculaP1x(double xi){
		double p1x = 0; //armazena o resultado de p'(x)

		p1x = derivada[0] * pow(x0, grau-1);
		for(int i = 1; i < grau-2; i++)
			p1x += derivada[i] * pow(xi, grau-i-1);
		p1x += derivada[grau-2] * xi + derivada[grau-1];
	}

	while((xi - xiAnt > EPSILON) || (cont < 1000)){
		xiAnt = xi;
		xi = xiAnt - calculaPx(xi)/calculaP1x(xi);
		cont++;
	}
	printf("\nMétodo de Newton com x0 = %.8lf\n", x0);
	printf("%d iterações\n", cont);
	printf("Raiz aproximada = %.8lf\n", xi);
}//newtown

void lagrange(int grau, double *coeficientes){

	double k[4];
	double n = grau;
	double b[4];
	double an[2];
	double L[4];
	int cont = 0; //variável auxiliar
	double *equacaoInversa = 0;

	equacaoInversa = (double*)malloc(sizeof(double)*grau+1);

	//Inverte a equação
	for(int i = grau; i > -1; i--){
		equacaoInversa[cont] = coeficientes[i];
		cont++;
	}
	an[0] = coeficientes[0]; 
	an[1] = equacaoInversa[0]; 

	void calcula_k(double *termos, int index){
		int i = 0;
		while((k[index] != grau-i+1) && (i < grau+1)){
			if(termos[i] < 0)
				k[index] = grau-i;
			i++;
		}
	}

	void calcula_b(double *termos, int index){
		for(int i = 0; i < grau+1; i++){
			if(termos[i] < b[index])
				b[index] = (termos[i]);			
		}
		b[index] = fabs(b[index]);
	}

	void calcula_l(int index, int index_an){
		if(n-k[index] == 1)
			L[index] = 1.0 + b[index]/an[index_an];
		else
			L[index] = 1.0 + pow(b[index]/an[index_an], 1.0/(n-k[index]));
	}

	void inverteSinal(double *termos){
		for(int i = 0; i < grau+1; i++){
			if(i%2 != 0)
				termos[i] *= -1;
		}
	}

	//Calcula L0, L1, L2 e L3
	for(int i = 0; i < grau; i++){
		if(i == 2) //Quando calcular L2 inverte os sinais da equação
			inverteSinal(coeficientes);
		if(i == 3) //Quando calcular L3 inverte os sinais da equação
			inverteSinal(equacaoInversa);

		if(i%2 == 0)
			calcula_k(coeficientes, i);
		else
			calcula_k(equacaoInversa, i);
		
		calcula_b(coeficientes, i);

		if(i%2 == 0)
			calcula_l(i, 0);
		else
			calcula_l(i, 1);
	}

	printf("\nIntervalo das raízes negativas: %.4lf <= x- <= %.4lf\n", -L[2], -1/L[3]);
	printf("Intervalo das raízes positivas: %.4lf <= x+ <= %.4lf\n", 1/L[1], L[0]);

	//Desinverte o sinal dos coeficientes a2, a4... 
	inverteSinal(coeficientes);
	newtown(L[0], grau, coeficientes);
}//lagrange

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
				gaussSeidel(A, ordem);	
				break;
				
			case 'E':
				printf("Informe o grau da equação algébrica: \n");
				scanf("%d", &grau);

				coeficientes = (double*)malloc(sizeof(double)*grau+1);

				while((verifica == 1) && (i < grau+1)){
					printf("Informe o coeficiente a%d:\n", grau-i);
					scanf("%lf", &coeficientes[i]); 
					
					// if((coeficientes[i] < 0) && (i < grau) || (coeficientes[grau] == 0)){
					// 	printf("a%d deve ser maior que 0 e a0 deve ser diferente de 0!\n", grau-i);
					// 	verifica = 0;
					// }
					i++;
				}

				if(verifica == 1)
					lagrange(grau, coeficientes);
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