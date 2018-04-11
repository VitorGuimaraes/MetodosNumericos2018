#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int calcula_k(int n, double *termos){
    int expoente = n;
	//Percorre o vetor de coeficientes
	for(int i = 0; i < n+1; i++){
		//Se o coeficiente for negativo k recebe o expoente do coeficiente
		if(termos[i] < 0){
			printf("Expoente = %d\n", expoente);
			return expoente;
		}
		expoente--;
	}
}

double calcula_b(int n, double *termos){
	int menor = 0;
	//Percorre o vetor de coeficientes
	for(int i = 0; i < n+1; i++){
		//Se o coeficiente atual for o mais negativo ele é armazenado
		if(termos[i] < menor)
			menor = termos[i];
	}
	printf("Menor = %.2f\n", fabs(menor));
	return fabs(menor);
}

double calcula_l(int n, double k, double b, double an){
	printf("L = %.4f\n", 1.0 + pow(b/an, 1.0/(n-k)));
	return 1.0 + pow(b/an, 1.0/(n-k));
}

void inverteSinal(int n, double *coeficientes){
	for(int i = 0; i < n+1; i++){
		if(i%2 != 0)
			coeficientes[i] *= -1;
		printf("termo[%d] = %lf\n", i, coeficientes[i]);
	}
}

int main(){
	double *coeficientes = 0;
	int grau = 4;

	coeficientes = (double*)malloc(sizeof(double)*grau+1);
	double vetor[5] = {1, 2, -3, -8, -4};

	for(int i = 0; i < grau+1; i++){
		// coeficientes[i] = vetor[i];
		// printf("Coeficiente[%d] = %lf\n", i, coeficientes[i]);

		printf("Informe o coeficiente a%d:\n", grau-i);
		scanf("%lf", &coeficientes[i]);
	}
	printf("\n");

	for(int i = 0; i < grau+1; i++)
		printf("Coeficiente[%d] = %lf\n", i, coeficientes[i]);

    // calcula_k(grau, coeficientes);
	// calcula_b(grau, coeficientes);
	// calcula_l(grau, 3, 16, 20);
	inverteSinal(grau, coeficientes);
	
}

// printf("%.2f, %.2f, %.2f, %.2f, %.2f", coeficienteInverso[0], coeficienteInverso[1], coeficienteInverso[2], coeficienteInverso[3], coeficienteInverso[4]);