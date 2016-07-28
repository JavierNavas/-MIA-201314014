#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int opcion=0;
int tamano=0;
char comando[256];
int main()
{
    printf("Bienvenido a FILE SYSTEM EXT2/EXT3\n");
    printf("Ingrese un comando:\n");
    gets(comando);
    tamano = strlen(comando);
     switch(opcion)
     {
     case 1:
      printf("---------------\n\n");
      break;
     case 2:
      printf("---------------\n\n");
      break;
     default:
       printf("comando erroneo\n");
       printf("---------------\n\n");
     }
    return 0;
}
