#include <stdio.h> 

int main(void){
  int   ch;
  char  buf[BUFSIZ];

  while(1){
    puts("Flushing input");
    while((ch = getchar()) != '\n' && ch != EOF);
    printf ("\nBuffer depois de limpar: %s\n", buf);
    printf("Enter some text: ");
    
    if(fgets(buf, sizeof(buf), stdin)){
        printf ("You entered: %s", buf);
    }
  }
  
  return 0;
}