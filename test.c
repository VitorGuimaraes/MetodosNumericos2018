#include <stdio.h>
#include <stdlib.h>

void PressioneEnterContinuar(){ 
	printf("\n\nPressione ENTER para continuar... ");
	while(1){ 
		int c = getchar();
		if(c != '\n' && c != EOF) 
			break; 
	} 
}

void clrKyb(void) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {};
}

int main(){

    char continuar = 's'; 

    while(continuar == 's'){
        char choice;

        // system("clear");
        printf("A - comer\n");
        printf("B - beber\n");
        printf("C - dormir\n");
        printf("D - andar\n");
        printf("F - FINALIZAR\n");
        
        scanf("%c", &choice);

        printf("*Choice = %c\n", choice);
        switch(choice){
            case 'A':
                printf("Comendo...\n");
                break;
            
            case 'B':
                printf("Bebendo...\n");
                break;
            
            case 'C':
                printf("Dormindo...\n");
                break;

            case 'D':
                printf("Andando...\n");
                break;

            case 'F':
                return 0;

            default:
                printf("Opção inválida!\n");
        printf("Deseja continuar? s-Sim n-Não\n");
        scanf("%c", &continuar);
        }
    }
    return(0);
}