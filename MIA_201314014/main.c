#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>


//STRUCTS QUE USA EL DISCO
typedef struct {
  char dato[64];
}BloqueInfo;

typedef struct DiscoDuro
{
 int size;
 char path[50];
 char name[50];
 char unit[50];
}DiscoDuro;


typedef struct
{
    char estado[1];
    char tipo[1];
    char ajuste[2];
    int byteIni;
    int tamano;
    char nombre[16];
}Particion;


typedef struct MBR
{
    int tamano;
    char fecha_creacion[128];
    int disk_signature;
    Particion partition_1;
    Particion partition_2;
    Particion partition_3;
    Particion partition_4;
}MBR;


//FUNCIONES
char** cad_split(char* a_str, const char a_delim);
void CrearDisco(char direccion[256]);
void agregarmbr(MBR* datosMbr);
DiscoDuro* parserMkdisk(char datos[150]);
void rmdisk(char archivo[150]);

//VARIABLES LOCALES
int opcion=0;
int tamdisco=0;
char comando[256];
char analizo[2][256];
int contador=0;
int KB = 1024;
int estadom=0;
FILE * midisco;
char nombredisco[256]="/home/milton/Disco3.dsk";

int main()
{
    tamdisco = 60* KB;
    CrearDisco(nombredisco);
    DiscoDuro *nuevoDisco =parserMkdisk("mkdisk -size::35 -unit::mi -path::hola ");
    printf("unit:%s\n",nuevoDisco->unit);
    printf("path:%s\n",nuevoDisco->path);
    printf("size:%d\n",nuevoDisco->size);
    rmdisk("/home/milton/Disco2.dsk");
    printf("Bienvenido a FILE SYSTEM EXT2/EXT3\n");
    while(estadom!=1){
    printf("Ingrese un comando :\n");
    scanf("%[^\n]", comando);


    char** tokens;
    tokens =cad_split(comando, ' ');

    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            strcpy(analizo[i], *(tokens + i));
        }
        free(tokens);
    }
    if( strcmp(analizo[0],"exec") == 0 ) {//aqui comparo si ambos comandos son iguales
        printf("son iguales:%s\n",analizo[1]);
        FILE* script;
        char linea[80];
        int endoffile=0;
        script=fopen(analizo[1],"r");
        if(script==NULL){
        printf("Ruta erronea, verifique ruta\n");
        }else{
            endoffile=fscanf(script," %[^\n]",&linea);

             while(endoffile!=EOF){
              //Si endofile es EOF entonces estamos en el final del archivo
            printf("%s\n",linea);
            endoffile=fscanf(script," %[^\n]",&linea);
            }
            fclose(script);
          }
     }else{
        printf("Comando no reconocido\n");
     }

    pausar();
    }
    return 0;
}



void CrearDisco(char direccion[256]) {


    midisco = fopen(direccion,"wb");
    fseek(midisco,0,SEEK_SET);
    for(int i = 0; i<tamdisco; i++){
        fwrite("/0",1,1,midisco);
    }
    fclose(midisco);


    MBR* datosMbr = (MBR *) malloc(sizeof(MBR));
    Particion particionTemp;
    strcmp(particionTemp.ajuste,"wf");
    particionTemp.byteIni = 0;
    strcmp(particionTemp.estado,"i");
    strcpy(particionTemp.nombre, "-");
    particionTemp.tamano = 0;
    strcmp(particionTemp.tipo, "p");
    time_t tiempo = time(0);
    struct tm *elTiempo = localtime(&tiempo);
    strftime(datosMbr->fecha_creacion, 128, "%d/%m/%y %H:%M:%S", elTiempo);
    datosMbr->disk_signature = rand() % 9999;
    datosMbr->partition_1=particionTemp;
    datosMbr->partition_2=particionTemp;
    datosMbr->partition_3=particionTemp;
    datosMbr->partition_4=particionTemp;
    datosMbr->tamano = tamdisco;

    printf("fecha: %s\n",datosMbr->fecha_creacion);
    printf("diskSignatura: %d\n",datosMbr->disk_signature);
    printf("tamano(Mbr): %d\n",sizeof(MBR));
    printf("tamano(Particion): %d\n",sizeof(Particion));
    agregarmbr(datosMbr);
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

void pausar(){
printf("PRESIONE ENTER PARA CONTINUAR");

while(getchar()!='\n');
getchar();

}


void agregarmbr(MBR* datosMbr){

    FILE *archivo;
    archivo = fopen("/home/milton/Disco2.dsk", "r+b");
    fseek(archivo, 0, SEEK_SET);
    fwrite(datosMbr, sizeof(MBR), 1, archivo);
    fclose(archivo);

}


DiscoDuro* parserMkdisk(char datos[150]){
    DiscoDuro *temp = (DiscoDuro*) malloc(sizeof(DiscoDuro));
    strcpy(temp->unit ,"m");
    char*linea = datos;
    while(*linea != '\n' && *linea != '\0' && linea != NULL)
    {
        while(*linea != '-'){
            linea++;
        }
        char pedazo[50];
        //limpio pedazo a analizar
        for(int i = 0; i<=50; i++){
        pedazo[i] = '\0';
        }
        int cont = 0;
        while(*linea != ' ' && linea != NULL && *linea != '\n' && *linea != '\0')
        {
        pedazo[cont] = *linea;
        linea++;
        cont++;
        }

        char* analizar =pedazo;
        analizar = strtok(analizar, "::");
        if(strcmp(analizar,"-size")==0)
        {
            analizar = strtok(NULL, "::");
            temp->size= atoi(analizar);
        }else if(strcmp(analizar,"-path")==0)
            {
                analizar = strtok(NULL, "::");
                strcpy(temp->path ,analizar);
            }else if(strcmp(analizar,"-unit")==0)
                {
                    analizar = strtok(NULL,"::");
                    strcpy(temp->unit ,analizar);
                }
    }
return temp;
}

void rmdisk(char archivo[150])
{
    int er = 0;
    FILE *eliminar = fopen(archivo, "r+b");
    if(eliminar == NULL)
    {
        er = 1;
        printf("El disco que se desea ELIMINAR NO EXISITE\n");
    }else
        fclose(eliminar);
    if(er==0)
    {
        remove(archivo);
        printf("El disco se ELIMINO CON EXITO\n");
    }
    er = 0;
}





