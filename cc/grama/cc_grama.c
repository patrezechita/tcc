#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "comum.h"

// retorna o minimo entre dois valores
#define MINIMO(x, y) (((x) < (y)) ? (x) : (y))


#define maxINT 99999
int nThread = 4;



typedef struct node {
	int val;
	struct node * next;
} node_t;

typedef struct grafo {
	int val;
	char cor;
	int pai;
	struct node * next;
} grafo_t;

int root = 0;
//int *vetorDFS;
int vetorDFS[999][999];
int tID;

void push(grafo_t G[], int val1, int val2);
void DFS_Visita(grafo_t G[], int i, int tBegin, int tLast);

void main() {
	int **matrizEntrada, qtdVertices, qtdArestas;

	// recebe a matriz de entrada da leitura
	matrizEntrada = lerEntrada();

	// guarda a quantidade de vertices e arestas
	qtdVertices = matrizEntrada[0][0];
	qtdArestas = matrizEntrada[0][1];

	// aloca espaco para o vetor dos componentes
	//vetorDFS = (int *)malloc(qtdVertices * sizeof(int));


	// vetor com todos os grafos
	grafo_t G[qtdVertices];

	// inicializa o grafo
	for(int i=0; i<qtdVertices; i++) {	
		G[i].next = NULL;
	}

	// insere as arestas
	for(int i=1; i<qtdArestas; i++) {
		push(G, matrizEntrada[i][0], matrizEntrada[i][1]);
		push(G, matrizEntrada[i][1], matrizEntrada[i][0]);
	}
	
	// DFS
	// para cada vertice, pintar de branco
	for(int i=0; i<qtdVertices; i++) {
		G[i].cor = 'B';
		G[i].pai = -1;
		G[i].val = i;
	}



	// criar vetor para cada thr e armazenar a qtd de vertice
	int qtdVetorTh[nThread];
	// calcula o piso e teto
	int piso = floor((float)qtdVertices/nThread);
	int teto = ceil((float)qtdVertices/nThread);
	for(int i=0; i<nThread; i++) {
		qtdVetorTh[i] = piso;
	}
	int aux = qtdVertices - (piso*nThread);
	for(int i=0; i<aux; i++) {
		qtdVetorTh[i] = teto;
	}

	// printf("%d aux %d teto\n", aux, teto);

	// for(int i=0; i<nThread; i++) {
	// 	printf("<%d|%d>\n", i, qtdVetorTh[i]);
	// }




	// int qtdLastTh;
	// // calcula a quantidade de vertice por thread
	// int NTH, NVT;
	// NVT = qtdVertices;
	// NTH = nThread;
	




	// int qtdTh;



	// // calcula a quantidade de vertices por thread
	// qtdTh = ceil((float)NVT/NTH); //pega o divisor
	// qtdLastTh = qtdTh; //o ultimo e igual a todos
	// NTH = NVT / qtdTh; // se tiver mais thread que vertice
	// if(NVT%qtdTh != 0) { //se o ultimo for diferente
	// 	NTH++;
	// 	qtdLastTh = NVT%qtdTh;
	// }

	// nThread = NTH; //so atualizando



	// printf("<%d th> %d ver por thr e a ultima tem %d\n", NTH, qtdTh, qtdLastTh);



	
	// int qtdPorThread = qtdTh;
	// int resto = qtdLastTh;

	//int qtdTh = ceil(qtdVertices/nThread);
	
	 

	int tBegin = 0;
	//para cada thread
	for(int i=0; i<nThread; i++) {
		int qtdPorThread = qtdVetorTh[i];
		//int tBegin = i * qtdPorThread; //primeiro elemento da thread
		
		int tLast = tBegin + qtdPorThread - 1; //ultimo elemento da thread
		tID = i;
		

		printf("\n<th %d | %d vert, do %d ao %d>\n", i, qtdPorThread, tBegin, tLast);

	
		// limpa o vetor
		for(int i=0; i<qtdVertices; i++) {
			vetorDFS[tID][i]=i;

		}

		// executa o DFS de cada thread
		for(int i=tBegin; i<=tLast; i++) {
			if(G[i].cor == 'B') {
				root = i;
				//printf("entrei no %d\n", i);
				DFS_Visita(G, i, tBegin, tLast);
			}
		}
		
		// // imprime pra debug
		// printf("[ ");
		// for(int i=0; i<qtdVertices; i++) {
		// 	printf("%d ", vetorDFS[tID][i]);
		// }
		// printf("]\n");

		// /////////////////////////////////
		// // acha os pais
		// int vetorAux[qtdVertices]; // vetor auxiliar
		// //preenche com -1
		// for(int i=0; i<qtdVertices; i++) { vetorAux[i] = -1;}

		// //calcula os pais
		// for(int i=0; i<qtdVertices; i++) { //dentro do vetor da thread
			
			
			
		// 		if(vetorAux[vetorDFS[tID][i]] == -1){
		// 			vetorAux[vetorDFS[tID][i]] = i;
		// 			vetorDFS[tID][i] = i;
		// 		}
		// 		else {
		// 			vetorDFS[tID][i] = vetorAux[vetorDFS[tID][i]];
		// 		}

			
		// 	//printf("%d ", vetorDFS[tID][i]);
		// }

		// // imprime pra debug
		// printf("[ ");
		// for(int i=0; i<qtdVertices; i++) {
		// 	printf("%d ", vetorDFS[tID][i]);
		// }
		// printf("]\n");

		tBegin += qtdPorThread;
	}


		//calcula o minimo
		printf("\nvetor dos minimos\n[ ");
		for (int i = 0; i < qtdVertices; i++) {
			int min = 999;

			for (int j = 0; j < nThread; j++) {
				min = MINIMO(vetorDFS[j][i], min);
			}
			printf("%d ", min);
		}
		printf("]\n");
	//imprimeSaida("saida_dfs.txt", vetorDFS, qtdVertices);
}

void push(grafo_t G[], int val1, int val2) {
	if(G[val1].next == NULL){
		G[val1].next = malloc(sizeof(node_t));
		G[val1].next->val = val2;
		G[val1].next->next = NULL;
		return;
	}

	node_t * current = G[val1].next;

	// percorre os no da lista ate chegar no ultimo
	while (current->next != NULL) {
		current = current->next;
	}

	//aloca um no
	current->next = malloc(sizeof(node_t));
	current->next->val = val2;
	current->next->next = NULL;
}


void DFS_Visita(grafo_t G[], int i, int tBegin, int tLast){

	
	G[i].cor = 'C';

	// coloca no vetor quem é o pai
	vetorDFS[tID][i]=root;

	//printf("colocando %d de pai em %d\n", root, i);

	node_t * current = G[i].next;

	while (current != NULL) {


		int u = i;
		int v = current->val;
		//printf("vi o %d\n", v);
		vetorDFS[tID][v]=root;
		//printf("proximo e o %d\n", v);


		if(G[v].cor == 'B'){
			G[v].pai = u;
			if(v >= tBegin && v <= tLast) {
				//printf("estou entrando no %d\n", v);

				DFS_Visita(G, v, tBegin, tLast);
			}
		}

		current = current->next;
	}

	G[i].cor = 'P';
}