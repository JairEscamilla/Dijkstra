/*
 * @author:  Allan Jair Escamilla Hernández, Raul Gonzalez Portillo y Cesar Mauricio Arellano Velasquez
 * @date:    17/septiembre/2019
 * @file:    grafos.c
 * @brief:  Implementacion de un generador de grafos y el algoritmo de Dijkstra.
 * -> Con una estructura de datos en C generar un tipo de datos "nodo" que contenga n conexiones definidas por el usuario.
 * -> Implementar las funciones de : anadir_nodo, anadir_vertice, desplegar_grafo, buscar_dato.
 * -> Cada nodo será etiquetado por su orden de insercion.
 */
#include"dijkstra.h" // Incluimos el archivo que contiene las estructuras y la implementacion del algoritmo de Dijkstra



// Prototipos de las funciones
void insertarNodo(Nodo** Raiz, char etiqueta, int conexiones, int i);
void buscarNodo(Nodo *Raiz, char buscado, Nodo **aux);
void agregarArista(Nodo *Raiz, int total);
void buscarDato(Nodo *Raiz, int total);
void liberar_memoria(Nodo** Raiz, int cant, int total, Nodo* anterior);
void deplegarTabla(Tabla tabla, int cuentaNodos);
void desplegarGrafo(int cuentaNodos);
void menu();

// Funcion principal
int main(){
    // DEFINIENDO VARIABLES
    Nodo* Raiz = NULL, *Inicial;
    int opcion = 2, conexiones, i = 0, cuentaNodos = 0;
    char etiqueta = 'a';
    unsigned int visitados = 0;
    int cuentaPasos = 0;
    int bandera = 0;
    Tabla t1;
    // CONTINUACION DE LA EJECUCION DEL PROGRAMA
    do{
        printf("Ingresar la cantidad de conexiones por nodo-> ");
        scanf("%d", &conexiones);
        if (conexiones < 1)
            printf("Los nodos deben tener al menos una conexion\n");
    } while (conexiones < 1);
    do{
        menu(); // DESPLEGANDO MENU
        scanf("%d", &opcion);
        switch (opcion){
        case 1:
            insertarNodo(&Raiz, etiqueta, conexiones, i);
            etiqueta++;
            i++;
            cuentaNodos++;
            printf("Se ha insertado correctamente el nodo al grafo!\n\n");
            break;
        case 2:
            if(Raiz != NULL)
                agregarArista(Raiz, conexiones);
            else
                printf("El grafo esta vacio\n");
            break;
        case 3:
            if(Raiz != NULL)
                buscarDato(Raiz, conexiones);
            else 
                printf("El grafo esta vacio\n");
            break;
        case 4:
            
            if(Raiz != NULL)
                desplegarGrafo(cuentaNodos);
            else 
                printf("El grafo esta vacio\n");
            break;
        case 5:
            if(Raiz != NULL){
                inicializarTabla(&t1, cuentaNodos);
                Dijkstra(Raiz, &t1, cuentaPasos, Raiz->etiqueta, cuentaNodos, conexiones, visitados);
                printf("Tabla calculada\n");
            }else 
                printf("El grafo esta vacio\n");
            bandera = 1;
            break;
        case 6:
            if(bandera == 0)
                printf("La tabla aun no ha sido calculada\n");
            else 
                deplegarTabla(t1, cuentaNodos);
            break;
        case 7:
            if(bandera == 1){
                calcular_ruta(t1, cuentaNodos);
            }else 
                printf("Aun no se ha calculado la tabla\n");
            break;
        case 8:
            if(Raiz != NULL){
                liberar_memoria(&Raiz, 0, conexiones, NULL);
            }
            printf("Saliendo del programa... \n");
            exit(0);
            break;
        default:
            printf("Ingresa una opcion valida!\n");
            break;
        }
        printf("\nPresione enter para continuar... ");
        __fpurge(stdin);
        getchar();
    } while (opcion != 8);
    
    return 0;
}

// DESARROLLANDO LAS FUNCIONES

/* * Funcion que imprime el menu
*/
void menu(){
    system("clear");
    printf("1.- Insertar nodo.\n");
    printf("2.- Agregar arista.\n");
    printf("3.- Buscar dato.\n");
    printf("4.- Desplegar grafo.\n");
    printf("5.- Calcular tabla\n");
    printf("6.- Desplegar tabla.\n");
    printf("7.- Calcular ruta más corta.\n");
    printf("8.- Salir.\n");
    printf("\n\nSeleccione una opcion-> ");
}

/* * Funcion que inserta un nodo al grafo.
   * @param Nodo** Raiz recibe la direccion de memoria del nodo raiz del grafo.
   * @param char etiqueta recibe un valor que contendrá la etiqueta del nodo a insertar.
   * @param int conexiones recibe la cantidad de conexiones por nodo.
*/
void insertarNodo(Nodo **Raiz, char etiqueta, int conexiones, int i){
    char NodoConnect;
    Nodo* aux = NULL,  *temp2;
    Nodo* temp = (Nodo*)malloc(sizeof(Nodo));
    temp->etiqueta = etiqueta;
    temp->cantidadConexiones = 0;
    temp->coincidencias = 0;
    temp->conexiones = (Nodo**)malloc(sizeof(Nodo*) * conexiones);
    temp->costo = (int*)malloc(sizeof(int)*conexiones);
    temp->next = NULL;
    for(int i = 0; i < conexiones; i++)
        temp->conexiones[i] = NULL; 
    if(*Raiz == NULL)
        *Raiz = temp;
    else{
        do{
            printf("Ingresar el nodo al que va conectado: ");
            scanf(" %c", &NodoConnect);
            buscarNodo(*Raiz, NodoConnect, &aux);
            if (aux == NULL)
                printf("El nodo ingresado no existe en el grafo, asegurese de ingresar uno que realmente exista!\n\n");
        } while (aux == NULL);
        if(aux->cantidadConexiones < conexiones){
            printf("Ingresar el costo: ");
            scanf("%d", &(aux->costo[aux->cantidadConexiones]));
            temp->costo[temp->cantidadConexiones] = aux->costo[aux->cantidadConexiones];
            aux->conexiones[aux->cantidadConexiones] = temp;
            temp->conexiones[temp->cantidadConexiones] = aux;
            (aux->cantidadConexiones)++;
            (temp->cantidadConexiones)++;
            if(i != 0){
                temp2 = *Raiz;
                while(temp2->next != NULL)
                    temp2 = temp2->next;
                temp2->next = temp;
            }
        }else{
            printf("Ya no hay espacio para mas conexiones\n");
        }
    }
}

/* * Funcion que busca un nodo dado por el usario
   * @param Nodo* Raiz recibe la direccion de memoria del nodo raiz del grafo.
   * @param int cant recibe una variable que contara las veces que se acceda a cada conexion de un nodo.
   * @param int total recibe el total de conexiones por nodo.
   * @param char buscado recibe la etiqueta del nodo a buscar.
   * @param Nodo** aux recibe la direccion de memoria de un apuntador que almacenará el nodo buscado.
*/
void buscarNodo(Nodo* Raiz, char buscado, Nodo** aux){
    Nodo* temp = Raiz;
    while (temp != NULL){
        if(temp != NULL && temp->etiqueta == buscado)
            *aux = temp;
        temp = temp->next;
    }
}

/* * Funcion que busca un dato en el grafo.
   * @param Nodo* Raiz recibe la direccion de memoria del nodo raiz del grafo.
   * @param int total recibe el total de conexiones por nodo.
*/
void buscarDato(Nodo* Raiz, int total){
    char etiqueta;
    Nodo* aux = NULL;
    printf("Ingresar nodo a buscar-> ");
    __fpurge(stdin);
    scanf("%c", &etiqueta);
    buscarNodo(Raiz, etiqueta, &aux);
    if(aux == NULL){
        printf("No se ha encontrado el dato\n");
        return;
    }
    printf("El dato %c ha sido encontrado en el grafo\nSus conexiones son: \n", aux->etiqueta);
    for(int i = 0; i < total; i++)
        if(aux->conexiones[i] != NULL)
            printf("\tNodo: %c, Costo: %d\n", aux->conexiones[i]->etiqueta, aux->costo[i]);
        else  
            printf("\tNodo: NULL, Costo: NULL\n");
}

/* * Funcion que agrega una arista al grafo.
   * @param Nodo* Raiz recibe la direccion de memoria del nodo raiz del grafo.
   * @param int total recibe el total de conexiones por nodo.
*/
void agregarArista(Nodo* Raiz, int total){
    char etiqueta1, etiqueta2;
    Nodo* agregarArista = NULL, *recibirConexion = NULL;
    printf("Ingresar el nodo al que desea agregar la arista-> ");
    __fpurge(stdin);
    scanf("%c", &etiqueta1);
    buscarNodo(Raiz, etiqueta1, &agregarArista);  
    printf("Ingresar el nodo al que se va a conectar el nodo anterior-> ");
    __fpurge(stdin);
    scanf("%c", &etiqueta2);
    buscarNodo(Raiz, etiqueta2, &recibirConexion);  
    if(agregarArista == NULL || recibirConexion == NULL){
        printf("Alguno de los nodos ingresados no existe\n");
        return;
    }
    if(agregarArista->cantidadConexiones >= total || recibirConexion->cantidadConexiones >= total){
        printf("Alguno de los nodos ya no tiene espacio para mas conexiones\n");
        return;
    }
    printf("Ingresar costo del salto: ");
    scanf("%d", &(agregarArista->costo[agregarArista->cantidadConexiones]));
    recibirConexion->costo[recibirConexion->cantidadConexiones] = agregarArista->costo[agregarArista->cantidadConexiones];
    agregarArista->conexiones[agregarArista->cantidadConexiones] = recibirConexion;
    recibirConexion->conexiones[recibirConexion->cantidadConexiones] = agregarArista;
    agregarArista->cantidadConexiones = agregarArista->cantidadConexiones + 1;
    recibirConexion->cantidadConexiones = recibirConexion->cantidadConexiones + 1;
    printf("Se ha agregado la arista con exito\n");
}


/* * Funcion que libera la memoria del grafo.
   * @param Nodo** Raiz recibe la direccion de memoria del nodo raiz del grafo.
   * @param int cant recibe una variable que contara las veces que se acceda a cada conexion de un nodo.
   * @param int total recibe el total de conexiones por nodo.
*/
void liberar_memoria(Nodo** Raiz, int cant, int total, Nodo* anterior){
    Nodo* aux = *Raiz;
    while (*Raiz != NULL){
        aux = *Raiz;
        *Raiz = (*Raiz)->next;
        free(aux);
    }
}


/* * Funcion que despliega la tabla de calculada por el algoritmo de Dijkstra.
   * @param Tabla tabla recibe la tabla donde se calcularon los caminos mas cortos.
   * @param int cuentaNodos recibe la cantidad de nodos en el grafo.
*/
void deplegarTabla(Tabla tabla, int cuentaNodos){
    printf("VERTEX: ");
    for(int i = 0; i < cuentaNodos; i++)
        printf("%c, ", tabla.vertex[i]);
    
    printf("\nSDF: ");
    for (int i = 0; i < cuentaNodos; i++)
        printf("%d, ", tabla.sdf[i]);
    
    printf("\nPREV VERTEX: ");
    for (int i = 0; i < cuentaNodos; i++)
        printf("%c, ", tabla.prevVertex[i]);
}

/* * Funcion que despliega las etiquetas de los nodos pertenecientes al grafo.
   * @param int cuentaNodos recibe la cantidad de nodos en el grafo.
*/
void desplegarGrafo(int cuentaNodos){
    char lbl = 'a';
    for(int i = 0; i < cuentaNodos; i++){
        printf("%c, ", lbl);
        lbl++;
    }
}