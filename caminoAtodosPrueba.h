
//Tipo de dato estado para la busqueda de la ruta que recorra todos los puntos (se forman listas simplemente enlazada de estados)
typedef struct estado estado;
struct estado{
	int * visitados;
	int puntoActual;
	int puntoAnterior;
 	int cantVisitados;
 	int distanciaAcum;
 	estado * sig;
};


//Tipo de dato nodo auxiliar, para manejar la lista de estados para la busqueda 
typedef struct listaB listaB;
struct listaB{
	estado * primero;
	estado * ultimo;
 	int largo;
};


//Funcion que copia un arreglo, entregando uno nuevo
//Entradas: el arreglo que se desea copiar y el largo de este
//Salida: un nuevo arreglo con la info del original
int * copiarArreglo(int * arreglo, int largo){
	int * nuevo=(int *)malloc(sizeof(int)*largo);
	int i;
	for(i=0;i<largo;i++){
		nuevo[i]=arreglo[i];
	}
	return nuevo;
}

//Funcion que crea un estado de la busqueda
//Entradas: el largo del arreglo de visitados, el arreglo de visitados del estado anterior, la cantidad de puntos visitados, 
//el nuevo punto anterior, el nuevo punto actual, la nueva distancia, para el nuevo estado
//Salida: un puntero al nuevo estado creado
estado * crearEstado(int largo, int * arreglo, int cantidadVis, int nuevoAnt, int nuevoActual, int distancia){
	estado * nuevoEstado=(estado *)malloc(sizeof(estado));
	nuevoEstado->visitados=copiarArreglo(arreglo,largo);
	nuevoEstado->visitados[nuevoActual]=nuevoAnt;
	nuevoEstado->puntoActual=nuevoActual;
	nuevoEstado->puntoAnterior=nuevoAnt;
	nuevoEstado->cantVisitados=cantidadVis+1;
	nuevoEstado->distanciaAcum=distancia;
	nuevoEstado->sig=NULL;
	return nuevoEstado;
}


//Funcion que inserta un nodo al final de la lista
//Entradas: la conexiones que se quiere ingresar y el puntero al nodo auxiliar con el que se esta manejando la lista
//Salida: no tiene
void insertarFinal2(listaB * lista, estado * nuevo){
	if(lista->primero == NULL){
		lista->primero=nuevo;
		lista->ultimo=nuevo;
	}else{
		lista->ultimo->sig=nuevo;
		lista->ultimo=nuevo;
	}
	lista->largo+=1;
}

//Funcion que elimina un estado el inicio de una lista simplemente enlazada 
//Entrada: la lista de la cual se desea eliminar el estado
//Salida: el puntero hacia el estado eliminado
estado * eliminarInicio(listaB * lista){
	estado * eliminado;
	if(lista!=NULL){
		eliminado=lista->primero;
		lista->primero=lista->primero->sig;
		lista->largo-=1;	
	}
	return eliminado;
}

//Funcion que crea un nodo auxiliar inicial para una lista doblemente enlazada.
//Entrada: ni tiene
//Salida: un nodo auxiliar
listaB crearLista2 (){
	listaB lista;
	lista.primero=NULL;
	lista.ultimo=NULL;
	lista.largo=0;
	return lista; 
}

//Funcion que muestra una lista de estados (se utiliza para visualizar el funcionamiento del procedimiento)
void mostrarLista3(listaB aux, int cant){
	if(aux.primero!=NULL){
		estado * puntero= aux.primero;
		int i;
		while(puntero!=NULL){
			printf("puntoActual=%d - puntoAnterior=%d - cantVisitados=%d - distAcum=%d \n", puntero->puntoActual, puntero->puntoAnterior, puntero->cantVisitados, puntero->distanciaAcum);
			for(i=0;i<cant;i++){
				printf(" %d ;", puntero->visitados[i]);
			}
			puntero=puntero->sig;
			printf("\n\n");
		}
		printf("Primero id= %d - Ultimo id= %d \n",aux.primero->puntoActual, aux.ultimo->puntoActual);
		printf("largo: %d \n", aux.largo);
	}
}

//Funcion que imprime las listas de estados (se utiliza para visualizar el funcionamiento del procedimiento)
void imprimirListas(listaB abiertos, listaB cerrados, int cantidad){
	if(abiertos.primero != NULL){
		printf("ABIERTOS: \n");
		mostrarLista3(abiertos, cantidad);
		printf("\n\n\n");
	}
	if(cerrados.primero != NULL){
		printf("CERRADOS: \n");
		mostrarLista3(cerrados, cantidad);
		printf("\n\n\n");
	}
}

//Funcion que buscar los caminos que recorrar todos los puntos
//Entrada: la matriz de adyacencia del grafo asociado, la cantidad de puntos de entrega, y el punto inicial
//Salida: una lista simplemente enlazada con todas las rutas encontradas
listaB buscarCaminoTodos(int ** matriz, int cantidad, int puntoInicial){
	int contador=0;
	listaB abiertos=crearLista2();
	listaB cerrados=crearLista2();

	int i;
	int * arreglo=(int *)malloc(sizeof(int)*cantidad);
	for(i=0;i<cantidad; i++){
		arreglo[i]=-2;
	}
	int distancia=0;
	estado * inicial=crearEstado(cantidad, arreglo, 0,-1,puntoInicial,distancia);
	insertarFinal2(&abiertos,inicial);

	estado * actual;
	estado * siguiente;

	while(abiertos.primero!=NULL){
		contador++;
		actual=eliminarInicio(&abiertos);
		if(actual->cantVisitados==cantidad){
			insertarFinal2(&cerrados,actual);
		}
		for(i=0;i<cantidad;i++){
			if(matriz[actual->puntoActual][i]!= 0 && actual->visitados[i] == -2){
				siguiente=crearEstado(cantidad,actual->visitados,actual->cantVisitados, actual->puntoActual, i, actual->distanciaAcum+matriz[actual->puntoActual][i]);
				insertarFinal2(&abiertos,siguiente);
				
			} 
		}
	}
	//imprimirListas(abiertos, cerrados, cantidad);
	printf("Contador= %d \n",contador );
	return cerrados;
}


//Funcion que extrae la informacion obtenida en la funcion buscarCaminoTodos, a partir de la representacion de un camino en el arreglo
//de visitados, entrega un arreglo con (modificandolo) con los puntos recorridos en orden
//Entrada: el arreglo que se desea interpretar, el largo de este, y el punto de inicio para interpretarlo (ultimo punto visitado)
//Salida: un arreglo con la informacion interpretada y los puntos ordenados
int * extraerCamino(int * arreglo, int largo, int inicio){
	int * nuevo=(int *)malloc(sizeof(int)*largo);
	nuevo[0]=inicio;
	int i;
	for(i=1;i<largo;i++){
		nuevo[i]=arreglo[inicio];
		inicio=arreglo[inicio];
	}
	int j=largo-1;
	for(i=0;i<largo;i++){
		arreglo[i]=nuevo[j];
		j--;
	}
	return arreglo;
}


//Funcion que escribe un camino en un archivo de texto llamado resultados.out
//Entradas: el estado del cual se extraera el camino, y el largo del arreglo de visitados
//Salida: no tiene
void escribirUnCamino(estado e, int largo){
	FILE * archivo1=fopen("resultados.out","w");   //Para borrarlo si existe 
	fclose(archivo1);
	FILE * archivo=fopen("resultados.out","a+t");
	fputs("Ruta: ",archivo);
	int i;
	char * texto=(char *)malloc(sizeof(char)*largo);
	sprintf(texto,"%d",e.visitados[0]+1);
	fputs(texto,archivo);
	for(i=1;i<largo;i++){
		sprintf(texto,"-%d",e.visitados[i]+1);
		fputs(texto,archivo);
	}
	fputs("  Distancia: ",archivo);
	sprintf(texto, "%d\n", e.distanciaAcum);
	fputs(texto, archivo);
	fclose(archivo);
}


//Funcion que escribe todos los caminos encontrados en un archivo de texto llamado resultados.out
//Entradas: el estado del cual se extraera el camino, y el largo del arreglo de visitados
//Salida: no tiene
void escribirTodosLosCaminos(listaB lista, int largo, int infinito){
	FILE * archivo1=fopen("resultados.out","w");   //Para borrarlo si existe 
	fclose(archivo1);
	FILE * archivo=fopen("resultados.out","a+t");
	estado * puntero=lista.primero;
	int i;
	char * texto=(char *)malloc(sizeof(char)*largo);
	int * arreglo;
	while(puntero!=NULL){
		fputs("Ruta: ",archivo);
		arreglo=extraerCamino(puntero->visitados, largo, puntero->puntoActual);	
		sprintf(texto,"%d",arreglo[0]+1);
		fputs(texto,archivo);
		for(i=1;i<largo;i++){
			sprintf(texto,"-%d",arreglo[i]+1);
			fputs(texto,archivo);
		}
		fputs("  Distancia: ",archivo);
		sprintf(texto, "%d \n", puntero->distanciaAcum);
		fputs(texto, archivo);
		puntero=puntero->sig;
	}
	fclose(archivo);
}


//Funcion que encuentra camino que recorre la menor distancia entre los encontrados
//Entrada: la lista de estados con los caminos encontrados, y el valor de la suma de todas las distancias de la matriz de adyacencia
//para hacer la comparacion
//Salida: el estado de la menor distancia recorrida
estado encontrarMenor(listaB lista, int infinito){
	int menor=infinito;
	estado * punteroActual=lista.primero;
	estado * puntero=lista.primero;
	while(puntero != NULL){
		if(puntero->distanciaAcum<menor){
			punteroActual=puntero;
			menor=puntero->distanciaAcum;
		}
		puntero=puntero->sig;
	}
	return *punteroActual;
}

//Funcion que revisa si existe al menos una ruta que recorra todos los puntos 
//Entrada: la lista de los caminos encontrados, el largo de la lista de visitados
//Salida: Un numero entero (0=Existe ; 1=No existe)
int existeRuta(listaB caminoTodos, int cantidad){
	int i;
	if(caminoTodos.primero!=NULL){
		for(i=0;i<cantidad;i++){
			if(caminoTodos.primero->visitados[i]==-2){
				return 1;
			}
		}
	}
	return 0;
}

void escribirQueNoExiste(){
	FILE * archivo1=fopen("resultados.out","w");   //Para borrarlo si existe 
	fclose(archivo1);
	FILE * archivo=fopen("resultados.out","a+t");
	fputs("No se ha encontrado ninguna ruta que recorra todos los puntos.\n",archivo);
	fclose(archivo);
}