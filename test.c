#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// function to convert decimal to binary
void decToBinary(int n)
{
    // array to store binary number
    int binaryNum[1000];
 
    // counter for binary array
    int i = 0;
    while (n > 0) {
 
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
 
    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
        printf("%d", binaryNum[j]);
}

void decimalParaBinario(double num, int precisao){
    int quociente = (int)num, i = 0, aux = 0;
    long double parteInteira = 0;
    double parteFracionaria = num - (double)quociente;

    /*Laco que calcula a parte inteira da conversao, dividindo a parte inteira da função*/
    while(quociente >= 2){
        parteInteira += quociente % 2 * pow(10, i);
        // printf("ParteInteira = %.4Lf\n", parteInteira);
        quociente = quociente / 2;
        // printf("quociente = %d\n", quociente);
        i++;
    }

    parteInteira += quociente % 2 * pow(10, i);
    // printf("ParteInteira = %.4Lf\n", parteInteira);
    printf("%lld.", (long long int)parteInteira);

    /* Laço que imprime a parte fracionária da conversão,
    multiplicando a parte fracionária pela base */
    for(i=1 ; i <= precisao ; i++){
        parteFracionaria*=2;
        aux=(int)parteFracionaria;

        /* Se a partefracionária for maior que zero
         ela imprime a parte inteira  e subtrai da parte fracionária
        se não, imprime zero e começa novamente.*/
        if(parteFracionaria>0) {
            printf("%d",(int)parteFracionaria);
            parteFracionaria-=aux;
        }

        else
          printf("0");

        /*A menos que a parte fracionaria seja 0, ai a coversão tera acabado.*/
        if(parteFracionaria==0)
            break;
    }
    printf("\n");
 }


// Driver program to test above function
int main()
{
    decimalParaBinario(40, 20);
    return 0;
}