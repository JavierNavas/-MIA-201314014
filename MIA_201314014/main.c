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


typedef struct administrador
{
    int size;
    char unit[50];
    char path[50];
    char type[50];
    char ajuste[50];
    char delet[50];
    char nombre[50];
    int agregar;
}administrador;

typedef struct EBR
{
    char estado[1];
    char ajuste[2];
    char nombre[16];
    int siguiente;
    int byteIni;
    int size;
}EBR;

//FUNCIONES
char** cad_split(char* a_str, const char a_delim);
void CrearDisco(char direccion[256]);
void agregarmbr(MBR* datosMbr,char*ruta);
DiscoDuro* parserMkdisk(char datos[150]);
void rmdisk(char archivo[150]);
administrador* parserfdisk(char datos[100]);
int fdisk(char datos[100]);
int existeExtendida(char *ruta);
int noParticiones(char *ruta);
MBR* obtenerMBR(char *ruta);
int encuentroParticion(MBR *mbr, int inicio);

EBR* getEbr(char *ruta);

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
    /*fdisk("fdisk -delete::fast -name::Particion1 -path::/home/Disco1.dsk");
    tamdisco = 60* KB;
    CrearDisco(nombredisco);
    DiscoDuro *nuevoDisco =parserMkdisk("mkdisk -size::35 -unit::mi -path::hola ");
    printf("unit:%s\n",nuevoDisco->unit);
    printf("path:%s\n",nuevoDisco->path);
    printf("size:%d\n",nuevoDisco->size);
    rmdisk("/home/milton/Disco2.dsk");*/
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
        FILE* script;
        char linea[180];
        char lineatras[180];
        char p[180];
        char trozoan[10];
        int endoffile=0;
        script=fopen(analizo[1],"r");
        if(script==NULL){
        printf("Ruta erronea, verifique ruta\n");
        }else{
            printf("El archivo si existe:%s\n",analizo[1]);
            endoffile=fscanf(script," %[^\n]",&linea);
            strcpy(p,linea);
            int c=1;
            int siglinea=0;
            while(endoffile!=EOF){
            char *li = linea;
            if(*li == '#'){
               printf("Linea No.%d es un Comentario:%s\n",c,linea);
            }else{
              if(siglinea==0){
              for(int i = 0; linea[i]; i++)
               linea[i] = tolower(linea[i]);
               char *trozo = NULL;
               trozo = strtok(linea," ");
               strcpy(trozoan,trozo);
               if(strcmp(trozo,"mkdisk") == 0 ){
                  if ( p[ strlen( p ) - 1 ] == '\\' ){
                       siglinea=1;
                       p[strlen(p)-1]=' ';
                       strcpy(lineatras,p);
                  }else{
                  printf("Linea No.%d es el comando mkdisk:%s\n",c,p);
                  }
               }
              }else{
                printf("entre aqui hahah\n");
                if(strcmp(trozoan,"mkdisk") == 0 ){
                  strcat(lineatras,linea);
                  printf("Linea No.%d es el comando mkdisk:%s\n",c,lineatras);
                }
                siglinea=0;
              }
            }
            c++;
            endoffile=fscanf(script," %[^\n]",&linea);
            strcpy(p,linea);
            }
            fclose(script);
          }
     }else if(strcmp(analizo[0],"salir") == 0 ){
         exit(1);
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
    agregarmbr(datosMbr,"hola");
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


void agregarmbr(MBR* datosMbr,char*ruta){

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


administrador* parserfdisk(char datos[100])
{
    administrador *admi = (administrador *) malloc(sizeof(administrador));
    admi->size = 0;
    strcpy(admi->type,"p");
    strcpy(admi->unit,"k");
    strcpy(admi->delet,"");
    strcpy(admi->ajuste,"wf");
    admi->agregar = 0;
    char *linea = datos;
    while(*linea != '\n' && *linea!= '\0' && linea != NULL)
    {
        while(*linea != '-')
            linea++;
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
        char* cadena =pedazo;
        cadena = strtok(cadena, "::");
        if(strcmp(cadena, "-size") == 0)
        {
            cadena = strtok(NULL, "=");
            admi->size = atoi(cadena);
        }
        else if(strcmp(cadena, "-path") == 0)
            {
                cadena = strtok(NULL, "::");;
                strcpy(admi->path,cadena);
            }
        else if(strcmp(cadena, "-type") == 0)
            {
                cadena = strtok(NULL, "::");
                strcpy(admi->type,cadena);
            }
        else if(strcmp(cadena, "-fit") == 0)
            {
                cadena = strtok(NULL, "::");
                strcpy(admi->ajuste,cadena);
            }
        else if(strcmp(cadena, "-delete") == 0)
            {
                cadena = strtok(NULL, "::");
                strcpy(admi->delet,cadena);
            }
        else if(strcmp(cadena, "-name") == 0)
            {
                cadena = strtok(NULL, "::");
                strcpy(admi->nombre,cadena);
            }
        else if(strcmp(cadena, "-add") == 0)
            {
                cadena = strtok(NULL, "::");;
                admi->agregar = atoi(cadena);
            }
        else if(strcmp(cadena, "-unit") == 0)
            {
                cadena = strtok(NULL, "::");;
                strcpy(admi->unit,cadena);
            }
    }
    return admi;
}

int fdisk(char datos[100])
{
    Particion nuevo;
    administrador*formatear= parserfdisk(datos);
    printf("unit:%s\n",formatear->unit);
    printf("path:%s\n",formatear->path);
    printf("delete:%s\n",formatear->delet);
    int estado = 0;
    FILE *archivo = fopen(formatear->path, "rb");
    if(archivo == NULL)
    {
        printf("El disco que desea modificar no existe");
        return 0;
    }
    fclose(archivo);
    if(strcmp(formatear->unit, "m") == 0)
    {
        formatear->size = formatear->size * 1024 * 1024;
        formatear->agregar = formatear->agregar * 1024 * 1024;
    }
    else if(strcmp(formatear->unit, "k") == 0)
    {
        formatear->size = formatear->size * 1024;
        formatear->agregar = formatear->agregar * 1024;
    }
    else if(strcmp(formatear->unit, "b") == 0)
    {
        formatear->size = formatear->size;
        formatear->agregar = formatear->agregar;
    }
    else
    {
        printf("AVISO:Comando unit no reconocido\n");
        return 0;
    }
    if(existeExtendida(formatear->path)==1 && strcmp(formatear->type, "e") == 0)
    {
        printf("AVISO:Existe una particion extendida en el disco\n");
        return 0;
    }
    if(noParticiones(formatear->path) >= 4)
    {
        printf("AVISO:ya existen 4 Particiones no se puede agregar mas\n");
        return 0;
    }
    MBR *mbr = obtenerMBR(formatear->path);
    if(strcmp(formatear->delet, "") != 0)
    {
        if(strcmp(formatear->delet, "fast") == 0)
        {
            if(strcmp(mbr->partition_1.nombre,formatear->nombre) == 0)
                strcmp(mbr->partition_1.estado,"i");
            else if(strcmp(mbr->partition_2.nombre,formatear->nombre) == 0)
                strcmp(mbr->partition_2.estado,"i");
            else if(strcmp(mbr->partition_3.nombre,formatear->nombre) == 0)
                strcmp(mbr->partition_3.estado,"i");
            else if(strcmp(mbr->partition_4.nombre,formatear->nombre) == 0)
                strcmp(mbr->partition_4.estado,"i");
            else
            printf("La particion \"%s\" no se existe en eldisco\n", formatear->nombre);
            agregarmbr(formatear->path, mbr);
        }
        else if(strcmp(formatear->delet, "full") == 0)
            {
                printf("AVISO:FULL\n");
            }
        else
            {
                printf("AVISO:+delete no es correcto\n");
            }
    }
    else if(formatear->agregar != 0)
        {
            if(formatear->agregar < 0)
            {
                printf("se eliminara espacio\n");
            }
            else
                printf("se agregara espacio\n");
        }
    else
        {
            int espacio= 0;
            strcmp(nuevo.tipo, formatear->type);
            strcmp(nuevo.ajuste, formatear->ajuste);
            strcmp(nuevo.estado, "a");
            strcpy(nuevo.nombre, formatear->nombre);
            nuevo.tamano = formatear->size;
            if(strcmp(formatear->type,"p") == 0 || strcmp(formatear->type, "e") == 0)
            {
                if(strcmp(mbr->partition_1.estado,"i") == 0)
                {
                    nuevo.byteIni = sizeof(MBR);
                    switch (encuentroParticion(mbr, 1))//primera particion ocupada
                    {
                    case 2:
                        espacio = mbr->partition_2.byteIni - sizeof(MBR);
                        if(espacio >= nuevo.tamano)//verifico si el tamano alcanza
                        {
                            mbr->partition_1 = nuevo;
                            estado = 1;
                        }
                        break;
                    case 3:
                        espacio= mbr->partition_3.byteIni - sizeof(MBR);
                        if(espacio >= nuevo.tamano)//verifico si el tamano alcanza
                        {
                            mbr->partition_1 = nuevo;
                            estado = 1;
                        }
                        break;
                    case 4:
                        espacio = mbr->partition_4.byteIni - sizeof(MBR);
                        if(espacio >= nuevo.tamano)//verifico si el tamano alcanza
                        {
                            mbr->partition_1 = nuevo;
                            estado = 1;
                        }
                        break;
                    default:
                        espacio = mbr->tamano- sizeof(MBR);
                        if(espacio >= nuevo.tamano)
                        {
                            mbr->partition_1 = nuevo;
                            estado = 1;
                        }
                        break;
                    }

                }
                if(strcmp(mbr->partition_2.estado, "i") == 0 && estado==0)
                {
                    if(strcmp(mbr->partition_2.estado, "a") == 0)
                        nuevo.byteIni = mbr->partition_1.byteIni + mbr->partition_1.tamano;
                    else
                        nuevo.byteIni = sizeof(MBR);
                    switch (encuentroParticion(mbr, 2))//busco la primera particion ocupada
                    {
                    case 3:
                        if(strcmp(mbr->partition_1.estado, "a") == 0)
                            espacio = mbr->partition_3.byteIni- mbr->partition_1.byteIni - mbr->partition_1.tamano;
                        else
                            espacio = mbr->partition_3.byteIni - sizeof(MBR);
                        if(espacio >= nuevo.tamano)
                        {
                            mbr->partition_2 = nuevo;
                            estado = 1;
                        }
                        break;
                    case 4:
                        if(strcmp(mbr->partition_1.estado, "a") == 0)
                            espacio = mbr->partition_4.byteIni-mbr->partition_1.byteIni - mbr->partition_1.tamano;
                        else
                            espacio = mbr->partition_4.byteIni - sizeof(MBR);
                        if(espacio >= nuevo.tamano)
                        {
                            mbr->partition_2 = nuevo;
                            estado= 1;
                        }
                        break;
                    default:
                        if(strcmp(mbr->partition_1.estado, "a") == 0)
                            espacio = mbr->tamano - mbr->partition_1.byteIni - mbr->partition_1.tamano;
                        else
                            espacio = mbr->tamano - sizeof(MBR);
                        if(espacio>= nuevo.tamano)
                        {
                            mbr->partition_2 = nuevo;
                            estado= 1;
                        }
                        break;
                    }
                }
                if(strcmp(mbr->partition_3.estado, "i") == 0 && estado==0)//nueva particion inactiva
                {
                    if(strcmp(mbr->partition_2.estado, "a") == 0)
                        nuevo.byteIni = mbr->partition_2.byteIni+ mbr->partition_2.tamano;
                    else if(strcmp(mbr->partition_1.estado, "a") == 0)
                            nuevo.byteIni = mbr->partition_1.byteIni + mbr->partition_1.tamano;
                    else
                            nuevo.byteIni = sizeof(MBR);
                    switch (encuentroParticion(mbr, 3))
                    {
                    case 4:
                        if(strcmp(mbr->partition_2.estado, "a") == 0)
                            espacio= mbr->partition_4.byteIni - mbr->partition_2.byteIni - mbr->partition_2.tamano;
                        else if(strcmp(mbr->partition_1.estado, "a") == 0)
                            espacio = mbr->partition_4.byteIni - mbr->partition_1.byteIni - mbr->partition_1.tamano;
                        else
                            espacio = mbr->partition_4.byteIni - sizeof(MBR);
                        if(espacio >= nuevo.tamano)//verifico si el tamano alcanza
                        {
                            mbr->partition_3 = nuevo;
                            estado= 1;
                        }
                        break;
                    default:
                        if(strcmp(mbr->partition_2.estado, "a") == 0)
                            espacio= mbr->tamano - mbr->partition_2.byteIni - mbr->partition_2.tamano;
                        else if(strcmp(mbr->partition_1.estado, "a") == 0)
                            espacio= mbr->tamano - mbr->partition_1.byteIni - mbr->partition_1.tamano;
                        else
                            espacio = mbr->tamano - sizeof(MBR);
                        if(espacio>= nuevo.tamano)
                        {
                            mbr->partition_3 = nuevo;
                            estado= 1;
                        }
                        break;
                    }
                }
                if(strcmp(mbr->partition_4.estado, "i") == 0 && estado==0)
                {
                    if(strcmp(mbr->partition_3.estado, "a") == 0)
                        nuevo.byteIni = mbr->partition_3.byteIni + mbr->partition_3.tamano;
                    else if(strcmp(mbr->partition_2.estado, "a") == 0)
                        nuevo.byteIni = mbr->partition_2.byteIni + mbr->partition_2.tamano;
                    else if(strcmp(mbr->partition_1.estado, "a") == 0)
                        nuevo.byteIni = mbr->partition_1.byteIni + mbr->partition_1.tamano;
                    else
                        nuevo.byteIni = sizeof(MBR);
                    if(strcmp(mbr->partition_3.estado, "a") == 0)
                        espacio = mbr->tamano - mbr->partition_3.byteIni - mbr->partition_3.tamano;
                    else if(strcmp(mbr->partition_2.estado, "a") == 0)
                        espacio = mbr->tamano - mbr->partition_2.byteIni - mbr->partition_2.tamano;
                    else if(strcmp(mbr->partition_1.estado, "a") == 0)
                        espacio = mbr->tamano - mbr->partition_1.byteIni - mbr->partition_1.tamano;
                    else
                        espacio = mbr->tamano - sizeof(MBR);
                    if(espacio >= nuevo.tamano)
                    {
                        mbr->partition_4 = nuevo;
                        estado= 1;
                    }
                }
                if(estado==1)//se se logro crear una particion entonces
                {
                    agregarmbr(formatear->path, mbr);//actualizo el mbr del disco
                    if(strcmp(nuevo.tipo, "e") == 0)//si la particion es extendida se crea un ebr
                    {
                        printf("entre a particion extendida\n");
                        EBR *ebr = (EBR *) malloc(sizeof(EBR));
                        strcmp(ebr->ajuste, "wf");
                        ebr->byteIni = nuevo.byteIni;
                        strcmp(ebr->estado, "i");
                        strcpy(ebr->nombre, "-");
                        ebr->siguiente = -1;
                        ebr->size = 0;
                        setEbr(formatear->path, ebr);//rfinal y debo mostrar datos de ebr
                    }
                    printf("disco formateado: .%s.\n", formatear->path);
                    printf("datos de mbr\n");
                    printf("AVISO:La particion \"%s\" se CREO con exito\n", nuevo.nombre);
                }
                else
                    printf("La capacidad del disco se ha sobrepasado\n");
            }
            else
                if(strcmp(formatear->type, "l") == 0)
                {
                    if(existeExtendida(formatear->path)==1)
                    {
                        printf("particion logica\n");
                        EBR *actual = getEbr(formatear->path);
                    }
                    else
                    {
                        printf("AVISO:No existe una particion extendida dentro de este disco\n");
                    }
                }
                else
                    printf("AVISO:El tipo de particion  no es correcto\n");
        }
    printf("size: %d\n", formatear->size);
    printf("path: %s\n", formatear->path);
    printf("unit: %s\n", formatear->unit);
    printf("borrar: %s\n", formatear->delet);
    printf("tipo: %s\n", formatear->type);
    printf("nombre: %s\n", formatear->nombre);
    printf("ajuste: %s\n", formatear->ajuste);
    printf("agregar: %d\n", formatear->agregar);



 return 0;
}

//verifico si hay particion extendida
int existeExtendida(char *ruta)
{
    MBR *mbr = obtenerMBR(ruta);
    if(strcmp(mbr->partition_1.estado, "a") == 0 && strcmp(mbr->partition_1.tipo, "e") == 0)
        return 1;
    if(strcmp(mbr->partition_2.estado, "a") == 0 && strcmp(mbr->partition_2.tipo, "e") == 0)
        return 1;
    if(strcmp(mbr->partition_3.estado, "a") == 0 && strcmp(mbr->partition_3.tipo, "e") == 0)
        return 1;
    if(strcmp(mbr->partition_4.estado, "a") == 0 && strcmp(mbr->partition_4.tipo, "e") == 0)
        return 1;
    return 0;
}

//miro el numero de particiones que tiene el disco de la ruta de entrada
int noParticiones(char *ruta)
{
    int cantidad = 0;
    MBR *mbr = obtenerMBR(ruta);
    if(strcmp(mbr->partition_1.estado, "a") == 0)
        cantidad++;
    if(strcmp(mbr->partition_2.estado, "a") == 0)
        cantidad++;
    if(strcmp(mbr->partition_3.estado, "a") == 0)
        cantidad++;
    if(strcmp(mbr->partition_4.estado, "a") == 0)
        cantidad++;
    return cantidad;
}

//obtengo el mbr del disco dependiendo de la ruta
MBR* obtenerMBR(char *ruta)
{
    FILE *archivo = fopen(ruta, "r+b");
    MBR *mbr = (MBR *) malloc(sizeof(MBR));
    fseek(archivo, 0, SEEK_SET);
    fread(mbr, sizeof(MBR), 1, archivo);
    fclose(archivo);
    return mbr;
}

int encuentroParticion(MBR *mbr, int inicio)
{
    switch(inicio)
    {
    case 1:
        if(strcmp(mbr->partition_2.estado,"a") == 0)
            return 2;
        else if(strcmp(mbr->partition_3.estado,"a") == 0)
            return 3;
        else if(strcmp(mbr->partition_4.estado,"a") == 0)
            return 4;
        break;
    case 2:
        if(strcmp(mbr->partition_3.estado,"a") == 0)
            return 3;
        else if(strcmp(mbr->partition_4.estado,"a") == 0)
                return 4;
        break;
    case 3:
        if(strcmp(mbr->partition_4.estado,"a") == 0)
            return 4;
        break;
    }
    return -1;
}
void setEbr(char *ruta, EBR *ebr)
{
    FILE *archivo;
    archivo = fopen(ruta, "r+b");
    fseek(archivo, ebr->byteIni, SEEK_SET);
    fwrite(ebr, sizeof(EBR), 1, archivo);
    fclose(archivo);
}

EBR* getEbr(char *ruta)
{
    MBR *mbr = obtenerMBR(ruta);
    FILE *archivo = fopen(ruta, "r+b");
    EBR *ebr = (EBR *) malloc(sizeof(EBR));
    //se busca la particion extendida dentro del disco
    if(strcmp(mbr->partition_1.estado, "a") == 0 && strcmp(mbr->partition_1.tipo, "e") == 0)
        fseek(archivo, mbr->partition_1.byteIni, SEEK_SET);
    if(strcmp(mbr->partition_2.estado, "a") == 0 && strcmp(mbr->partition_2.tipo, "e") == 0)
        fseek(archivo, mbr->partition_2.byteIni, SEEK_SET);
    if(strcmp(mbr->partition_3.estado, "a") == 0 && strcmp(mbr->partition_3.tipo, "e") == 0)
        fseek(archivo, mbr->partition_3.byteIni, SEEK_SET);
    if(strcmp(mbr->partition_4.estado, "a") == 0 && strcmp(mbr->partition_4.tipo, "e") == 0)
        fseek(archivo, mbr->partition_4.byteIni, SEEK_SET);
    fread(ebr, sizeof(EBR), 1, archivo);
    fclose(archivo);
    return ebr;
}



