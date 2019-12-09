#include <stdio.h>
#include <stdlib.h>
#include "caminoAtodosPrueba.h"
#include <time.h>

//Tipo de dato nodo para ordenar los puntos 
typedef struct nodo nodo;
struct nodo{
	int conexiones; //conexiones encontrada (cantidad de pixeles que coinciden)
	int punto;  //cantidad de veces que se encuentra la conexiones
	nodo * sig;
	nodo * ant;
};

//Tipo de dato nodo auxiliar, para manejar la lista de puntos para el ordenamiento
typedef struct nodoAux nodoAux;
struct nodoAux{
	nodo * primero;
	nodo * ultimo;
 	int largo;
};

int infinito=0;  //Se asignara un valor a infinito sumando todas las distancias
//Esto asegura el funcionamiento de dijkstra solo para el caso en que no se repitan los puntos
//#############################################################################################################################################
//Funcion que lee el archivo de texto que contiene la informacion del garfo que se trabajara
//Entrada: el nombre del archivo (string), la cantidad de vertices del grafo (puntero a int), y un auxiliar
//de corte, para el caso en que no se logre leer correctamente el archivo y terminar la ejecucion del programa (como puntero a int)
//Salida: una matriz de enteros (la matriz de adyacencia asociada)
int ** leerArchivo(char*nombreArchivo, int * cantidad, int * corte){
	FILE * archivo=fopen(nombreArchivo, "r");
	int ** matriz;
	int puntoNuevo;
	int punto;
	int distancia;
	char nada;
	char caracter;
	int i;
	int j;
	int k;
	int k2;

	if(archivo == NULL){
		printf("Hubo un problema con la lectura del archivo. \n");
		printf("Fin del programa. \n");
		*corte=1;
	}else{
		fscanf(archivo,"%d", cantidad);
		matriz=(int **)malloc(sizeof(int *)*(*cantidad));
		for(i=0;i<(*cantidad);i++){
			matriz[i]=(int *)malloc(sizeof(int)*(*cantidad));
			for(j=0;j<(*cantidad);j++){
				matriz[i][j]=0;	
			}
		}
		i=-1;
		while((caracter = fgetc(archivo)) != EOF){
			if(caracter=='\n' || caracter =='\r'){
				if(caracter=='\r'){
					caracter=fgetc(archivo);
				}
				i++;
			}
			fscanf(archivo,"%d%c%d",&punto,&nada,&distancia);
			punto--;
			matriz[i][punto]=distancia;
			infinito+=distancia;
	    }
	}
	fclose(archivo);
	return matriz;
}

//Funcion que inserta un nodo al principio de la lista
//Entradas: la conexiones que se quiere ingresar y el puntero al nodo auxiliar con el que se esta manejando la lista
//Salida: no tiene
void insertarDelante(int conexiones, int p, nodoAux * aux){
	nodo * nuevo=(nodo *)malloc(sizeof(nodo));
	if( (*aux).primero == NULL){
		nuevo->ant=NULL;
		nuevo->sig=NULL;
		nuevo->conexiones=conexiones;
		nuevo->punto=p;
		(*aux).primero=nuevo;
		(*aux).ultimo=nuevo;
	}else{
		nuevo->ant=NULL;
		nuevo->sig=(*aux).primero;
		aux->primero->ant=nuevo;
		nuevo->conexiones=conexiones;
		nuevo->punto=p;
		(*aux).primero=nuevo;
	}
	(*aux).largo+=1;
}

//Funcion que inserta un nodo al final de la lista
//Entradas: la conexiones que se quiere ingresar y el puntero al nodo auxiliar con el que se esta manejando la lista
//Salida: no tiene
void insertarFinal(int conexiones,int p, nodoAux * aux){
	nodo * nuevo=(nodo *)malloc(sizeof(nodo));
	if((*aux).primero == NULL){
		nuevo->ant=(*aux).primero;
		nuevo->sig=NULL;
		nuevo->conexiones=conexiones;
		nuevo->punto=p;
		(*aux).primero=nuevo;
		(*aux).ultimo=nuevo;
	}else{
		nuevo->ant=(*aux).ultimo;
		nuevo->sig=(*aux).ultimo->sig;
		(*aux).ultimo->sig=nuevo;
		nuevo->conexiones=conexiones;
		nuevo->punto=p;
		(*aux).ultimo=nuevo;
	}
	(*aux).largo+=1;
}

//Funcion que inserta un nodo en cualquier lugar de la lista
//Entradas: la conexiones que se quiere ingresar y el puntero al nodo de la izquierda de donde se quiere insertar y el de la derecha
//Salida: no tiene
void insertar(int conexiones,int p, nodo * izq, nodo * der){
	nodo * nuevo=(nodo *)malloc(sizeof(nodo));
	nuevo->sig=der;
	nuevo->ant=izq;
	nuevo->conexiones=conexiones;
	nuevo->punto=p;
	izq->sig=nuevo;
	der->ant=nuevo;
}

//Funcion que muestra la lista por pantalla, se deja aca para revisar si los resultados son correctos
void mostrarLista(nodoAux aux){
	nodo * puntero= aux.primero;
	while(puntero!=NULL){
		printf("punto = %d - conexiones = %d \n", puntero->punto, puntero->conexiones);
		puntero=puntero->sig;
	}
	printf("Primero id= %d - Ultimo id= %d \n",aux.primero->punto, aux.ultimo->punto );
	printf("largo: %d \n", aux.largo);
}

//Funcion que crea un nodo auxiliar inicial para una lista doblemente enlazada.
//Entrada: ni tiene
//Salida: un nodo auxiliar
nodoAux crearLista (){
	nodoAux lista;
	lista.primero=NULL;
	lista.ultimo=NULL;
	lista.largo=0;
	return lista; 
}

//Funcion que busca una conexiones en la lista, si no existe la agrega
//Entrada: un nodo auxiliar, la conexiones que se esta buscando, y el puntero al nodo auxiliar original
//Salida: no tiene
void ordenarPuntos(nodoAux lista, int conexiones, int punto, nodoAux * original){
	if(lista.largo==1){
		if(lista.primero->conexiones<=conexiones){
			insertar(conexiones,punto,lista.primero,lista.primero->sig);
		}else{
			insertar(conexiones,punto,lista.primero->ant,lista.primero);
		}

	}else if(lista.largo==2){
		if( lista.primero->conexiones<conexiones ){
			insertar(conexiones,punto,lista.primero->ant,lista.primero);
		}else if( lista.ultimo->conexiones<=conexiones && lista.primero->conexiones>=conexiones){
			insertar(conexiones,punto,lista.primero, lista.primero->sig);
		}else{ 
			insertar(conexiones,punto,lista.ultimo,lista.ultimo->sig);
		}
	}else{
		nodo * puntero = lista.primero;
		int mitad=lista.largo /2;
		int i;
		for(i=0;i<mitad;i++){
			puntero=puntero->sig;
		}
			nodoAux listaNueva;
			if(puntero->conexiones>conexiones){ //Voy hacia la derecha a buscar
				listaNueva.primero=puntero;
				listaNueva.ultimo=lista.ultimo;
				listaNueva.largo=mitad+1;	
				  

			}else{  //Voy hacia la izquierda a buscar
				listaNueva.primero=lista.primero;
				listaNueva.ultimo=puntero;
				listaNueva.largo=mitad+1;
			}
			return ordenarPuntos(listaNueva,conexiones,punto,original);
	}

}


void agregarPunto(nodoAux * lista, int conexiones, int p){
	if((*lista).primero == NULL){
		insertarDelante(conexiones,p,lista);
	}else{
		if (lista->primero->conexiones <= conexiones){
			insertarDelante(conexiones,p,lista);
		}else if(lista->ultimo->conexiones >= conexiones){
			insertarFinal(conexiones,p,lista);
		}else{  //Esta entre medio 
			//printf("Voy a entrar a ordenarPuntos\n");
			ordenarPuntos(*lista,conexiones,p,lista);
			(*lista).largo+=1;
		}
	}
}


void crearListaPuntos(nodoAux * lista, int ** matriz, int cantidad){
	int i;
	int j;
	int contador=0;
	for(i=0;i<cantidad; i++){
		for(j=0;j<cantidad;j++){
			if(matriz[i][j]!=0){
				contador++;
			}
		}
		agregarPunto(lista,contador,i);
		contador=0;
	}
}


//Funcion que muestra la matriz de adyacencia
//Entrada: la matriz (de enteros), y la cantidad de vertices del grafo (entero)
//Salida: no tiene 
void mostrarMatriz(int ** matriz, int cantidad){
	int i;
	int j;
	int k;
	printf("   ");
	for(k=0;k<cantidad;k++){
		printf("%d ",k+1);
	}printf("\n   ");
	for(k=0;k<cantidad;k++){
		printf("- ");
	}printf("\n");
	for(i=0;i<cantidad;i++){
		printf("%d |", i+1);
		for(j=0;j<cantidad;j++){
			printf("%d ", matriz[i][j]);
		}printf("\n");
	}
}


//Funcion que libera la memoria reservada para la matriz de adyacencia
//Entrada: la matriz de enteros, y sus dimensiones (numero de vertices)
//Salida: No tiene
void liberarMatriz(int ** matriz, int cantidad){
	int i;
	for(i=0;i<cantidad;i++){
		printf("Estoy en i= %d \n", i);
		free(matriz[i]);
	}free(matriz);
}

//Funcion que libera la memoria de la lista
//Entrada: El nodo auxiliar que representa la lista
//Salida: no tiene 
void liberarLista(nodoAux lista){
	nodo * puntero=lista.primero;
	nodo * punteroAux=puntero;
	while(puntero->sig != NULL){
		punteroAux=puntero;
		puntero=puntero->sig;
		free(punteroAux);
	}
}

//#############################################################################################################################################

//Funcion que muestra la pila de puntos de la ruta mas corta
//Entrada: La lista que se desea mostrar
//Salida: No tiene 
void mostrarLista2(nodoAux aux){
	nodo * puntero= aux.primero;
	if(puntero!=NULL){     //Se comprueba que no sea nula
		printf("%d", puntero->punto);   //Esta parte es solo para que se vea bonito y no tenga un - antes del primero
		puntero=puntero->sig;
		while(puntero!=NULL){
			printf("-%d", puntero->punto);
			puntero=puntero->sig;
		}
	}	
}

typedef struct casilla casilla;
struct casilla{
	int anterior;
	int costo;
};

void buscarCamino(int inicio, int fin, int ** matriz, int cantidad){
	casilla * camino=(casilla *)malloc(sizeof(casilla)*cantidad);
	short * visitados=(short *)malloc(sizeof(short)*cantidad);
	int i;
	for(i=0;i<cantidad;i++){
		visitados[i]=0;
		camino[i].anterior= -1;
		if(i != inicio){
			camino[i].costo= infinito;  
		}else{
			camino[i].costo=0;
		}
	}
	int indice=inicio;
	int costoIndice=0;
	int nuevoCosto;
	int j;
	int k;
	int numeroVisitados=0;
	while(numeroVisitados != cantidad ){
		numeroVisitados++;
		visitados[indice]=numeroVisitados;
		for(j=0;j<cantidad;j++){
			if(visitados[j]==0 && matriz[indice][j]!=0){
				nuevoCosto = costoIndice + matriz[indice][j];
				if(nuevoCosto<camino[j].costo){ 
					camino[j].anterior=indice;
					camino[j].costo=nuevoCosto;
				}
			}
		}
		indice=-1;
		costoIndice= infinito;
		for(k=0;k<cantidad;k++){
			if(visitados[k]==0 && camino[k].costo < costoIndice){  
				costoIndice=camino[k].costo;
				indice=k;
			}if(indice==fin){
				nodoAux lista=crearLista();
				int indiceInterno = fin;
				while(indiceInterno != -1){
					insertarDelante(0,indiceInterno+1,&lista);  
					indiceInterno=camino[indiceInterno].anterior;
				}
				if(visitados[inicio]==0 || visitados[fin]==0){
					printf("Ruta mas corta: ");
					mostrarLista2(lista);
					printf("\nDistancia= %d \n\n", camino[fin].costo);
				}else{
					printf("No se ha encontrado ruta desde el punto %d al %d. \n", inicio+1, fin+1);
				}
			}
		}
	}

}


//Funcion de escritura
//Entrada: La lista de puntos ordenados, la lista de la ruta que permite pasar por todos los puntos, y la distancia que esto significa
//Salida: no tiene
void escribirArchivo(nodoAux listaOrdenados, int cantidad){
	FILE * archivo=fopen("resultados.out","a+t");
	fputs("Puntos ordenados: ",archivo);
	nodo * puntero= listaOrdenados.primero;
	if(puntero!=NULL){     //Se comprueba que no sea nula
		char * punto=(char *)malloc(sizeof(char)*cantidad);   //TENGO QUE REVISAR SI ESTA VARIABLE PUEDE SER DINAMICA O FALLARA
		sprintf(punto,"%d",puntero->punto+1);
		fputs( punto, archivo );
		puntero=puntero->sig;
		while(puntero!=NULL){
			sprintf(punto,"-%d",puntero->punto+1);
			fputs( punto, archivo );
			puntero=puntero->sig;
		}
	}
	fclose(archivo);

}


int main(){
	int programa=0;
	int respuesta;
	int inicio;
	int destino;
	int cantidad;
	int corte=0;
	int opcion;
	int * arreglo;
	int i;
	nodoAux lista=crearLista();
	int ** matriz=leerArchivo("ejemplo13.txt",&cantidad,&corte);

	clock_t t_ini, t_fin;
	double secs;

	//t_ini = clock();
	  /* ...hacer algo... */
	//t_fin = clock();

	//secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;


	if(corte != 0){
		return 0;
	}else{
		//Se ordenan los puntos segun sus conexiones
		
		crearListaPuntos(&lista,matriz,cantidad);
		

		do{
			do{
				printf("Ingrese el punto de inicio: ");
				scanf(" %d",&inicio);
			}while( inicio>cantidad || inicio<1);
			inicio=inicio-1;
			do{
				printf("Ingrese el punto de destino: ");
				scanf(" %d",&destino);

			}while(destino>cantidad || destino<1);
			destino=destino-1;
			
			//Se busca el camino mas corto desde el punto de inicio al punto de fin
			t_ini = clock();
			buscarCamino(inicio,destino,matriz,cantidad);
			t_fin = clock();
			secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
			printf("Tiempo= %f \n", secs);

			return 0;
			
			//busqueda hacia todos los puntos
			listaB caminoTodos=buscarCaminoTodos(matriz, cantidad, 0);
			if(existeRuta(caminoTodos,cantidad)==0){
				do{
				printf("Ingrese 1 si desea ver una ruta cualquiera que recorra todos los puntos, \n");
				printf("ingrese 2 si desea ver la ruta mas corta que recorra todos los puntos,\n");
				printf("O ingrese 3 si desea ver todos los caminos encontrados que recorren todos los puntos: ");
				scanf(" %d", &opcion);
				}while(opcion!=1 && opcion!= 2 && opcion!=3);

				if(opcion==1){
					estado e=*caminoTodos.primero;
					arreglo=extraerCamino(e.visitados,cantidad,e.puntoActual);
					escribirUnCamino(e,cantidad);
				}else if(opcion==2){
					estado e=encontrarMenor(caminoTodos,infinito);
					arreglo=extraerCamino(e.visitados, cantidad, e.puntoActual);
					escribirUnCamino(e,cantidad);
				}else if(opcion==3){
					escribirTodosLosCaminos(caminoTodos,cantidad,infinito);
				}
			}else{
				escribirQueNoExiste();
			}
			
			escribirArchivo(lista,cantidad);
			//REINICIO
			do{
				printf("Ingrese 1 para buscar otra ruta o 2 para salir: ");
				scanf(" %d",&respuesta);
			}while(respuesta != 1 && respuesta != 2);
			if(respuesta==2){
				programa=1;
			}
			////////////////////////////////          LIBERAR MEMORIA          /////////////////////////////////////
		}while(programa == 0);
	}

	///////////////////////////////////////           LIBERAR MEMORIA          /////////////////////////////////////
	//liberarLista(lista);
	//liberarMatriz(matriz,cantidad);
	free(matriz);
	printf("Fin del programa. \n");
	return 0;
}



