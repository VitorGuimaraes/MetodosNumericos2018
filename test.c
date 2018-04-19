#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Variáveis Globais
double **A; //armazena a matriz	
double *p;  //armazena a matriz de ponteiros
int ordem;  //armazena a ordem da matriz 

double **alocaMatriz(int l, int c){
	//Se houver memória disponível aloca uma matriz com l linhas e c colunas e 
	//devolve o endereço base da matriz, caso contrário devolve um ponterio nulo
	
	double **m;
	m = malloc(sizeof(double *) * l);
	if(m == NULL){ //Memória insuficiente
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

void imprimeMatriz(double **m, int n_linha, int n_coluna){
	// Imprime os valores da matriz com l linhas e c colunas 
	for(int i = 0; i < n_linha; i++){
		for(int j = 0; j < n_coluna; j++){
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

void pivota(double **A, int pivot_row, int pivot_col){
	float temp;
	int i, large_pivot_row = pivot_row;
	
	//Verifica qual o maior número na COLUNA e atribui a large_pivot_row
	for(i = pivot_row; i < ordem; i++){
		//to find greatest among the pivot column 
		if(A[i][pivot_col] > A[large_pivot_row][pivot_col]){
			large_pivot_row = i;
		}
	}
	
	//Verifica se o pivo passado por parâmetro é o mesmo da verificação anterior
	if(pivot_row != large_pivot_row){
		//to interchange the rows
		for(i = 0; i < ordem+1; i++){
			temp = A[large_pivot_row][i];
			A[large_pivot_row][i] = A[pivot_row][i];
			A[pivot_row][i] = temp;
		}
	}
	printf("PIVOTEI KRL\n");
	imprimeMatriz(A, ordem, ordem+1);
}

int main(){
    double diagonal_element;
    double flag;

    int i, j, k, l, passo = 0, sub = 0, mul = 0, div = 0;

    lerMatriz();
    imprimeMatriz(A, ordem, ordem+1);

    for(i = 0; i < ordem; i++){
		for(j = 0; j < ordem+1; j++){
            if(i == j){
                pivota(A, i, j);

                diagonal_element = A[i][j];
				k = i;
				l = j;
				
				printf("elemento diagonal = %.2lf\n\n", diagonal_element);

                for(l = 0; l < ordem+1; l++){
					//for making the diagonal element 1
					A[k][l] /= diagonal_element;
					div++;
				}
				printf("jurubebaabaporojucaiba\n");
				imprimeMatriz(A, ordem, ordem+1);

                for(k = 0; k < ordem; k++){
					//setting flag = the element on respective row which is exactly below the concerned diagonal element
					flag = A[k][j];
										
					for(l = 0; l < ordem+1; l++)
						if(k != i){
							//performing row operation to male all the elements = 0, except diagonal element 
							A[k][l]= (A[k][l]) - flag * (A[i][l]);
							mul++;
							sub++;
						}
				}	
            }
        }
        printf("Passo %d\n", passo);
        imprimeMatriz(A, ordem, ordem+1);
        passo++;
    }
    //printing result
	for(i = 0; i < ordem; i++)
        printf("%.2lf\n", A[i][ordem]);
    
    return 0;
}