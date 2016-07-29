#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  int tamano;
}BloqueInfo;

int opcion=0;
int tamano=0;
char comando[256];
char analizo[2][256];
int contador=0;
char nombrearchivo[20];

int main()
{
    printf("Bienvenido a FILE SYSTEM EXT2/EXT3\n");
    printf("Ingrese un comando :\n");
    fgets(comando, 256, stdin);
    tamano = strlen(comando);

    char * pch;
    pch = strtok (comando," ");
    while (pch != NULL)
    {
    strcpy(analizo[contador], pch);
    printf ("esta:%s\n",pch);
    pch = strtok (NULL," ");
    contador++;
    }

    if( strcmp(analizo[0],"exec") == 0 ) //aqui comparo si ambos nombres son iguales
        printf("son iguales");
    else
        printf("Comando no reconocido");


    return 0;
}

