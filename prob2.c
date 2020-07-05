#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double px;
    double py;
}Ponto;
 
double solve(int nPontos, int k, Ponto pontos[]){
	int j=0;
	int linha=0;
	int col=0;
	double maximo=0.0;
	double resultado = 0.0;
	
	/* alocar memoria */
	int i=0;
	double ** dp = malloc(sizeof(*dp) * (k+1));
    for(i=0; i<=k; i++) {
        dp[i] = malloc(sizeof(**dp) * (nPontos+1));
    }
	
	for(linha=0; linha <= k; linha++){
		printf(" \n"); 
		for(col=0 ; col <= nPontos; col++){
			
			if (linha==0){
				dp[linha][col] = 0.0;
			}
			/* valor das areas totais */
			else if(linha==1){
				dp[linha][col] = pontos[col].px * pontos[col].py;
			}
			/* a considerar k>1*/
			else if(linha>=2){
				/* calcula novo maximo */
				maximo = 0.0;
				/* compara subareas possiveis até ao ponto atual inclusive */
				for(j = 0; j < col ; j++){ 
					/* soma valor da linha anterior com a comparacao feita */
					double tmp = dp[linha-1][j] +((pontos[col].px - pontos[j].px) * pontos[col].py);
					/* compara se é maior que o maximo anterior*/
					if (tmp > maximo){
						maximo = tmp;
					}
					/*estabelece novo maximo*/
					dp[linha][col] = maximo;
				}		
			}
			printf(" %.12lf  ",dp[linha][col]); 

			/* quando chega ao valor de K, estabelece a area maxima para K pontos */
			if(linha == k){
				if(dp[linha][col] > resultado){
					resultado = dp[linha][col];
				}
			}
		}	
	}
	/* liberta memoria */
  	for(i=0;i<k+1;i++){
  		free(dp[i]);
  	}
  	free(dp);
	return resultado;
}


void ordenarPx(int nPontos, Ponto pontos[]){
    int i,j;

    for(i=1;i<nPontos;i++){
        for(j = 0; j < nPontos-i; j++){
            if(pontos[j].px > pontos[j+1].px){
                Ponto aux = pontos[j];
                pontos[j] = pontos[j+1];
                pontos[j+1] = aux;
            }
        }
    }
}

int main(){
	int i=0;
	int nPontos=0;
	int k=0;

	double px,py;

	scanf("%d %d",&nPontos,&k);

	/*Ponto pontos[nPontos];*/
	Ponto *pontos = malloc((nPontos+1)*sizeof(Ponto));
	
	for(i=0;i<nPontos;i++){
		scanf("%lf %lf", &px, &py);
		pontos[i].px = px;
		pontos[i].py = py;
	}

	/* ordenar pontos por xx */
	ordenarPx(nPontos, pontos);

    /* print input */
    /*
	printf("N = %d \t K = %d \n", nPontos,k);
	int j;
	for(j=0; j< nPontos; j++){
		printf("Px: %.12lf \t Py: %.12lf\n", pontos[j].px, pontos[j].py);
		printf("\t -> %.12lf \n", pontos[j].px * pontos[j].py);
	}
	*/

	printf("%.12lf\n",solve(nPontos, k, pontos));
	free(pontos);
	return 0;
}
