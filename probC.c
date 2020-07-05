#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define MAX_EQUIPAMENTOS 1000
#define INFINITO INT_MAX/2

typedef struct {
	int src;
	int dest;
	int peso;
}Ligacao;


int rede[1000][1000];
int conta = 0;
bool PAs[1000] = {false};
int soma=0;

int nrArestas = 0;

int dist[1000][1000];

int set[1000] = {0};
int rank[1000] = {0};


int minimo(int a, int b){
	if(a>b){
		return b;
	}
	return a;
}

void DepthFirstSearch(int *dfs, int *low, bool *visited, int *parent, bool *AP, int v, int n){
    int i=0;
    visited[v] = true; //visita no inicial
    dfs[v] = low[v] = conta;
    conta = conta + 1;
    //printf("%d\n", conta);
    int child = 0;
    for (i = 1; i<n+1; i++){
    	/*printf("$$%d\n", rede[v][i]);*/
        if (rede[v][i] > 0) {
        	/*printf("~~~~~~~~~~\n"); */
            if (visited[i] == false) {
            	//verticeAtual++;
            	//printf("verticeAtual: %d\n", verticeAtual);
            	/*printf("... por visitar...\n");*/
                parent[i] = v;

                child = child + 1;

                /*printf("##%d %d %d\n", dfs[i], parent[i], child);*/
                DepthFirstSearch(dfs, low, visited, parent, AP, i, n);

                low[v] = minimo(low[v], low[i]);

                /*verifica se root é PA ou nao */
                if (parent[v] == -1 && child > 1){
                	//printf("$ %d\n",v);
                    AP[v] = true;
                }
                
                //verifica se outros nos sao PA ou nao
                if (parent[v] != -1 && low[i] >= dfs[v]){
                	//printf("- %d\n",v);
                    AP[v] = true;
                }  
            }
            else if (parent[v] != i){
                low[v] = minimo(low[v], dfs[i]);
            }
        }
    }/*
    if(v > verticeMax){
		verticeMax = v;
	}*/
}


int articulationPoints(int n){
	//verticeMax = 1;
	conta = 0;
	int *dfs,*low,*parent;
	bool *visited,*AP;

	dfs = malloc((n+1)*sizeof(int));
    low = malloc((n+1)*sizeof(int));
    parent= malloc((n+1)*sizeof(int));
    AP = malloc((n+1)*sizeof(bool));
    visited = malloc((n+1)*sizeof(bool));

	memset(dfs, -1, (n+1)*sizeof(int));
	memset(low,-1,(n+1)*sizeof(int));
	memset(parent,-1,(n+1)*sizeof(int));
	memset(AP,false,(n+1)*sizeof(bool));
	memset(visited,false,(n+1)*sizeof(bool));

	int i=0;
	soma = 0;
	for(i=1; i<n+1; i++){
		if(i==1){
			DepthFirstSearch(dfs,low,visited,parent,AP,i,n);
		}else{
			if(visited[i]==false){ //redes desconexas--> visita outros nós
				//printf("#%d\n", verticeAtual);
				DepthFirstSearch(dfs,low,visited,parent,AP,i,n);
			}
		}
		if(AP[i]){
			//printf("		>> %d is PA\n", i);
			PAs[i] = true;
			soma++; //nr de pontos de articulacao
		}
	}

	/*printf("N=%d\n", n);
	//printf("## conta = %d\n", conta);
	*/
	free(dfs);
	free(low);
	free(parent);
	free(AP);
	free(visited);
	return soma;
}


int floydWarshall(int n){
	/*
    int dist[1000][1000];
    */
    int i=0, j=0, k=0; 
    int totalCabo = 0;
  	memset(dist, 0, sizeof(dist));

  	// INICILIAZAR MATRIZ DE DISTANCIAS MINIMOS ENTRE NÓS
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++) {
        	if(i != j && rede[i][j] == 0){
        		dist[i][j] = INFINITO;
        	}else{
        		dist[i][j] = rede[i][j]; 
        	}
        }
    }
    
    for (k = 1; k <= n; k++) { 
        for (i = 1; i <= n; i++){ 
            for (j = 1; j <= n; j++){ 
            	dist[i][j] = minimo(dist[i][j], dist[i][k] + dist[k][j]);
            } 
        } 
    } 
  
    /*matriz com as distancias minimas entre pontos*/
   	totalCabo = 0;
    for (i = 1; i <= n; i++){ 
        for (j = i; j <= n; j++){ 	
        	//printf ("%3d ", dist[i][j]); 
        	/*if(dist[i][j] == INFINITO){
        		printf("%3s","INF ");
        	}else{
        		printf ("%3d ", dist[i][j]); 
        		*/
	        	//printf("----- ##%d , i=%d, j=%d\n", dist[i][j], i, j);
        		if(dist[i][j] != INFINITO && PAs[i] == true && PAs[j] == true && i!=j){
					totalCabo += dist[i][j];
					nrArestas++;
				}
	        	
	        //}
        } 
        //printf("\n"); 
    } 
    return totalCabo;
}


int find(int i){
	if(set[i] != i){
		//printf("$$$$$$$\n");
		//printf("find# %d %d\n", set[i], i);
		set[i] = find(set[i]);
	}

	//printf("$$$cc$$$$\n");
	return set[i];
}

void link(int a,int b){
	if(rank[a] > rank[b]){
		set[b] = a;
	}else{
		set[a] = b;
	}
	if(rank[a] == rank[b]){
		rank[b]++;
	}
	//printf("link~~ %d %d\n", a, b);
}

void uniao(int a, int b){
	//printf("union$$ %d %d\n", find(a), find(b));
	link(find(a),find(b));
}

int kruskal(int n){
	//printf(">>%d\n",nrArestas);
	//Aresta arrayArestas[nrArestas]; 
	//aresta* arrayArestas = malloc(nrArestas*sizeof(*arrayArestas));
	
	Ligacao *arrayArestas = (Ligacao *)malloc((nrArestas+1)*sizeof(Ligacao));
	//Aresta *arrayArestas = malloc(nrArestas*sizeof(*arrayArestas));

	int totalCaboArvore=0;
	memset(arrayArestas, 0, (nrArestas+1)*sizeof(arrayArestas));

	int i=0,j=0;
	for(i=1; i<n; i++){
		//make set
		set[i] = i;
		rank[i] = 0;
	}
	int indice = 0;
	for (i = 1; i <= n; i++){ 
        for (j = i; j <= n; j++){ 	
        	if(dist[i][j] > 0 && dist[i][j]!=INFINITO && PAs[i] == true && PAs[j] == true && i!=j){
    			indice++;
    			arrayArestas[indice].src = i;
    			arrayArestas[indice].dest = j;
    			arrayArestas[indice].peso = dist[i][j];

    			//printf(">		MATRIX:\t %d\n",dist[i][j]);
    			/*
    			printf(">		SRC:\t %d\n", arrayArestas[indice].src);
				printf(">		DEST:\t %d\n", arrayArestas[indice].dest);
    			printf(">			PESO:\t %d\n", arrayArestas[indice].peso);
    			*/
    		
        	}
        }
    }
    //printf("########## %d %d\n",indice, nrArestas);

    // ORDENAR POR ORDEM CRESCENTE O PESO DAS ARESTAS
    // BUBLE SORT
	Ligacao temp;
	for (i = 0; i < indice; i++){
	    for (j = 0; j < (indice - i); j++){
	        if (arrayArestas[j].peso > arrayArestas[j+1].peso){
	            temp = arrayArestas[j];
                arrayArestas[j] = arrayArestas[j + 1];
                arrayArestas[j + 1] = temp;
	        }
	    }
	}

    //printf("---------------------------------------------------\n");
    for(i=1; i<=indice; i++){
    	//printf("####....######\n");
		/*
		printf(">		~~~	SRC:\t %d\n", arrayArestas[i].src);
    	printf(">		~~~	DEST:\t %d\n", arrayArestas[i].dest);
    	printf(">		~~~  	     PESO: %d\n", arrayArestas[i].peso);
    	*/
    	//printf("\n");
    	if(find(arrayArestas[i].src) != find(arrayArestas[i].dest)){
    		/*
			printf("---------------------------------------------------\n");
			
			printf(">			SRC:\t %d\n", arrayArestas[i].src);
			printf(">			DEST:\t %d\n", arrayArestas[i].dest);
			printf(">		    	\tPESO:\t %d\n", arrayArestas[i].peso);
			
			printf("---------------------------------------------------\n");
			*/
			uniao(arrayArestas[i].src,arrayArestas[i].dest);
			totalCaboArvore += arrayArestas[i].peso;
		}    
    	
    }
    free(arrayArestas);
    return totalCaboArvore;
}


int main(){
	int n=0;
	int id, connect, tamanho;
	int firstConnect, firstTamanho;
	char line[MAX_EQUIPAMENTOS+1] = "";

	scanf("%d[^\n]",&n);
    
	/*printf("	- > N: %d\n", n);*/
	memset(rede, 0, sizeof(rede));
	
	while(fgets(line, sizeof(line),stdin)){ 
		scanf("%d %d %d[^\n]", &id, &connect, &tamanho);
		
		if(id==0){
			/*printf("================================\n");*/
			/*printf("FROM: %d\t TO: %d\t LENGHT: %d\n", id, connect, tamanho);	*/
			/* imprime tabela de adjacencia */
			/*
			int i, j;
			for(i = 0; i < n+1; i++){
				for(j = 0; j < n+1; j++){
				    printf("%3d ", rede[i][j]);
				}
			printf("\n");
			}
			printf("================================\n");
			*/
			int AP = articulationPoints(n);

			if(AP == 0){
				printf("no server\n");
			}else if(AP == 1){
				printf("1 0 0\n");
			}
			else{
				/*
				printf("-> AP =  %d\n",AP);
				int totalCabo = floydWarshall(n);
				printf("-> Total Cabo = %d\n",totalCabo);
				printf("-> Total Cabo Arvore = %d\n",totalCaboArvore);
				*/
				int direto = floydWarshall(n);
				int tree = kruskal(n);

				printf("%d %d %d\n",AP,direto,tree);
			}

			/* start new case */
			if(scanf("%d",&firstConnect) != EOF && scanf("%d",&firstTamanho) != EOF){
				n=0;
				n = connect;

				memset(rede, 0, sizeof(rede));
				memset(dist,0,sizeof(dist));
				memset(PAs, false, sizeof(PAs));
				//memset(parent, 0, sizeof(parent));
				memset(set, 0, sizeof(set));
				memset(rank, 0, sizeof(rank));

				conta=0;
				nrArestas = 0;
				//nrArestas++;
				/*printf("	- > N: %d\n", n);*/
				id = tamanho;
				rede[id][firstConnect] = firstTamanho;
				rede[firstConnect][id] = firstTamanho;
				/*printf("FROM: %d\t TO: %d\t LENGHT: %d\n", id, firstConnect, firstTamanho);*/
			}	
		}else{
			/*printf("FROM: %d\t TO: %d\t LENGHT: %d\n", id, connect, tamanho);*/
			rede[id][connect] = tamanho;
			rede[connect][id] = tamanho;
			//nrArestas++;
		}
	}
	
	return 0;
}