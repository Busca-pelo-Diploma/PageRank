#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct{
	int indice;        // Índice da pag
	int out;           // Nume de paginas que ela aponta
	int nin;           // Num de paginas que apontam para ela
	long double rank;       // PageRank da pagina
}pagina;

// --------------- FUNÇÃO RESPONSÁVEL PELA COMPARAÇÃO --------------- //

int comparaValor(const void *p,const void *q){
	pagina x = *(const pagina *)p;              // Função que ordena os PageRanks
    pagina y = *(const pagina *)q;

	if (x.rank > y.rank)
        return -1;                      // Retorna -1 se já estiver ordenado
	else if (x.rank < y.rank)
        return 1;                         // Retorna 1 se precisar ser ordenado
	else if (x.rank == y.rank)
		{
			if (x.indice < y.indice)
				return -1;                // Ordena os indices caso os PageRanks sejam iguais
			if (x.indice > y.indice)
				return 1;
		}
}

// --------------- FUNÇÃO PRINCIPAL --------------- //

int main()
{
	int n, i, j, notadecalculo = 0, cont = 0;     // Declaração de n de pags e contadores
	long double somatorio = 0.0, condparada = 0.0;     //      ||    do somatorio e cond. de parada para o calculo
	int **pags;        // Vetor que salva as paginas que apontam para a pagina i
	long double *prankaux;     //  Auxiliar para o PageRank 
	char *aux1, *aux2;    //  Vetores auxiliares para o strtok
	pagina *pr;  // Declaração do struct


	// --------------- LEITURA DA ENTRADA E ALOCAÇÃO --------------- //

	scanf("%d", &n);	                          //Lê o número de páginas

	aux1 = (char*)malloc(n*10*sizeof(char));
	pags = (int **)malloc(n*sizeof(int *));       // Aloca os principais vetores
	prankaux = (long double*)malloc(n*sizeof(long double));
	pr = (pagina *)malloc(n*sizeof(pagina));

	for(i=0; i<n; i++){
		pr[i].nin=0;                    // Envia o numero de paginas que apontam para i 
		pr[i].out=0;                    // Envia o numero de paginas que i aponta    
		pr[i].indice=i;                 //Envia índice da página para o struct
		pags[i] = (int *)malloc(sizeof(int)); // Aloca a 2a parte dos vetores
	}

	while(scanf("%s", aux1)==1){         //Lê a pag e as que estão ligadas com ela

		aux2 = aux1;
		aux2 = strtok(aux2, ":");		//Quebra da entrada
		j = atoi(aux2);
		while(aux2!=NULL){
			aux2 = strtok(NULL, ",");
			if(aux2==NULL)
				break;           // Para evitar que o contador conte mais uma vez
			pr[atoi(aux2)].nin++;
			pags[atoi(aux2)] = (int *)realloc(pags[atoi(aux2)], (pr[atoi(aux2)].nin)*sizeof(int));
			pags[atoi(aux2)][pr[atoi(aux2)].nin-1] = j;      //Realoca a 2a parte para cada tamanho específico
			cont++;                                          // e salva o os "in"s no indices 
		}
		pr[j].out=cont;
		notadecalculo++;
		cont = 0;                     //Reinicia o contador do tamanho específico
	}

	// --------------- PAGE RANK --------------- //

	for(i=0; i<n; i++){
		pr[i].rank = 1.0/n;         // Inicia os page ranks
		prankaux[i] = pr[i].rank;   // Copia os PRs para um auxiliar que será usado na comparação
	}

	while(true){

		for(i=0; i<n; i++){
			somatorio = 0.0;   // Reinicia o somatorio e a cond. de parada para refazer a conta
			condparada = 0.0;
			if(pr[i].out == 0){
				for(j=0; j<pr[i].nin; j++)
					somatorio += (prankaux[pags[i][j]])/(pr[pags[i][j]].out);		//Somatorio no caso do
                                                                                    // numero de outs ser 0 
				pr[i].rank = (0.15/n) + (0.85 * pr[i].rank) + (0.85 * somatorio);
			}
			else{
				for(j=0; j<pr[i].nin; j++)
					somatorio += prankaux[pags[i][j]]/pr[pags[i][j]].out;
				                                                             //Somatorio no caso do numero
				pr[i].rank = (0.15/n) + 0.85 * somatorio;                    //de outs ser diferente de 0

			}
		}

		for(i=0;i<n;i++)
			condparada += ((pr[i].rank - prankaux[i]))*((pr[i].rank) - (prankaux[i]));	//Calcula o somatorio de PR(i)-PR(i-1)

		condparada = sqrt(condparada);	//Tira a raiz do somatorio

		if(condparada<0.0000000000000001){			//Se o somatorio for menor que 10^-16, sai da repetição
			break;
		}

		for(i=0; i<n; i++){
			prankaux[i] = pr[i].rank;   // Copia os novos PRs para um auxiliar que será usado na comparação
		}
	}

	// --------------- ORDENAÇÃO --------------- //

	qsort(pr, n, sizeof(pagina), comparaValor); // Ordena os PageRanks e Índices do maior para o menor
	
	// --------------- DESALOCAÇÃO --------------- //

	int z;
	for (z=0; z < n ; z++)                           // Imprime os Indices e PRs ordenados
		printf("%d %lf\n",pr[z].indice, pr[z].rank);

	for(i=0; i<n; i++){
		free (pags[i]);         	// Desaloca os vetores principais do mais interno para fora
	}
	free(pags);                   
	free(aux1);                   // Desaloca os vetores auxiliares
	free(aux2);
	free(prankaux);
	free(pr);

	// ------------------- // -------------------- //

	return (0);
}
