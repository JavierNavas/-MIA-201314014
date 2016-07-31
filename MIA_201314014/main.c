#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char** cad_split(char* a_str, const char a_delim);
void CrearDisco(char direccion[256], int tamano);

typedef struct {
  char dato[64];
}BloqueInfo;

int opcion=0;
int tamano=0;
char comando[256];
char analizo[2][256];
int contador=0;
int KB = 1024;

int main()
{
    printf("Bienvenido a FILE SYSTEM EXT2/EXT3\n");
    printf("Ingrese un comando :\n");
    scanf("%s", comando);
    tamano = strlen(comando);


    char** tokens;
    tokens =cad_split(comando, ',');

    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            strcpy(analizo[i], *(tokens + i));
        }
        free(tokens);
    }

    //CrearDisco(comando,5);
    if( strcmp(analizo[0],"exec") == 0 ) //aqui comparo si ambos nombres son iguales
        printf("son iguales");
    else
        printf("Comando no reconocido");


    return 0;
}



void CrearDisco(char direccion[256], int tamano) {
    int particion = tamano * KB*KB;

    printf("******************************************************************\n");
    printf("******************************************************************\n");
    char nombre[256];
    strcpy(nombre, direccion);
    printf("esta es la direccion:%s",nombre);
    FILE *archivo;
        strcat(nombre,".bin");
        printf("cuidado:%s",nombre);
        archivo = fopen(nombre, "ab");
        BloqueInfo bloque;
        int faltante = particion/sizeof(BloqueInfo); /// lo divido para obtener el numero de bloques que caben en 5 megas
        int i;
        strcpy(bloque.dato,"\0");
        for (i = 1; i <= faltante; i++) {
            fwrite(&bloque, sizeof (BloqueInfo), 1, archivo);
            fflush(archivo);
        }
        fclose(archivo);

}



char** cad_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
