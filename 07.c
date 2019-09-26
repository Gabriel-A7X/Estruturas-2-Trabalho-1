#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct No{
    int altura, inicio, fim, enderecoInicio, enderecoFim;
    char status;
    struct No *esq, *dir;
};

typedef struct No No;

No* criaNo(int *status, int inicio, int fim,int enderecoInicio, int enderecoFim);
void rotacaoRR(No** raiz);
void insere(No** raiz, No* Novo);
void mostra(No* raiz);
int maior(int a, int b);
int altNo(No* raiz);
int alocar(No** raiz, No** pai, int qtdBlocos, int *status);
int liberar(No** raiz, No** pai, int qtdBlocos, int* status);
int buscaAloca(No* raiz, int valor);
int buscaLibera(No* raiz, int valor);


int main(){
	No* raiz = NULL;
    // 1 para ocupado e 0 para livre.
	int  status=0, choice;
	clock_t tempoBuscaI, tempoBuscaF;
	clock_t tempoAlterarI, tempoAlterarF;
	float tempoBuscaDecorrido, tempoAlterarDecorrido;
	while(1==1){
		printf("1-Criar Arvore\n2-Mostra Arvore\n3-Alocar No\n4-Liberar No\n0-Sair\n");
		scanf("%d", &choice);
		switch(choice){
			case 1:
				free(raiz);
				raiz = NULL;
				printf("Insira como esta o primeiro bloco (1-Ocupado 0-Livre)\n");
				scanf("%d", &status);
				int inicio, fim;
				printf("Insira o endereço incial: ");
				scanf("%d", &inicio);
				printf("Insira o endereço final: ");
				scanf("%d", &fim);

				int auxinicio=0, auxfim=0, cont=0;

				while(auxfim < fim){
					printf("Insira o endereço[%d] incial: ",cont);
					scanf("%d", &auxinicio);
					printf("Insira o endereço[%d] final: ", cont++);
					scanf("%d", &auxfim);
					insere(&raiz, criaNo(&status, inicio,fim,auxinicio,auxfim));
				}
				break;
			case 2:
				mostra(raiz);
				break;
			case 3:
				
				printf("Digite a quantidade de blocos para Alocar: ");
				int auxalocar;
				scanf("%d", &auxalocar);


				tempoBuscaI = clock();
				int procuraAloc = buscaAloca(raiz, auxalocar);
				tempoBuscaF = clock();
				tempoBuscaDecorrido = (tempoBuscaF- tempoBuscaI) / (CLOCKS_PER_SEC/1000) ;
				printf("Tempo gasto BUSCAR: %lf \n", tempoBuscaDecorrido);

				
				tempoAlterarI = clock();
				if( alocar(&raiz,NULL, auxalocar, &status) == 0)
					printf("Espaço insuficiente [%d].\n", procuraAloc);
				else
					printf("Alocação bem sucedida [%d].\n", procuraAloc);
				
				tempoAlterarF = clock();
				tempoAlterarDecorrido = (tempoAlterarF-tempoAlterarI)/(CLOCKS_PER_SEC/1000);
				printf("Tempo gasto ALTERAR: %lf \n", tempoBuscaDecorrido);

				break;
			case 4:

				printf("Quantidade de blocos para liberar: ");
				int auxliberar;
				scanf("%d", &auxliberar);

				tempoBuscaI = clock();
				
				int procuraLib = buscaLibera(raiz, auxliberar);

				tempoBuscaF = clock();
				tempoBuscaDecorrido = (tempoBuscaF- tempoBuscaI) / (CLOCKS_PER_SEC/1000) ;
				printf("Tempo gasto BUSCAR: %lf \n", tempoBuscaDecorrido);
				

				tempoAlterarI = clock();
				if( liberar(&raiz,NULL, auxliberar, &status) == 0)
					printf("Espaço insuficiente [%d].\n", procuraLib);
				else
					printf("Liberação bem sucedida [%d].\n", procuraLib);
				
				tempoAlterarF = clock();
				tempoAlterarDecorrido = (tempoAlterarF-tempoAlterarI)/(CLOCKS_PER_SEC/1000);
				printf("Tempo gasto ALTERAR: %lf \n", tempoBuscaDecorrido);
				break;
			case 0:
				exit(0);
		}
	}
    return 0;
}


No* criaNo(int *status, int inicio, int fim,int enderecoInicio, int enderecoFim){
	No* No = (No*) malloc(sizeof(No));
    No->altura = 0;
	No->inicio = inicio;
	No->fim = fim;
	No->enderecoInicio = enderecoInicio;
	No->enderecoFim = enderecoFim;
    No->esq = NULL;
	No->dir = NULL;
	if( *status == 0)
		No->status = 'L';
	else
		No->status = 'O';
	*status = 1- (*status);
	return No;
}

int maior(int a, int b){
	if(a>b)
		return a;
	return b;
}

int altNo(No* raiz){
    int alt;
    if(raiz == NULL){
        alt = -1;
    }else{
        alt = raiz->altura;
    }
    return alt;
}

void rotacaoRR(No** raiz){
	No *aux;
	aux = (*raiz)->dir;
	(*raiz)->dir = aux->esq;
	aux->esq = *raiz;
	*raiz = aux;
}

void insere(No** raiz, No* Novo){
	if(*raiz==NULL){
		*raiz = Novo;
	}else{
		if( (*raiz)->fim >= Novo->enderecoInicio){
			if(Novo->enderecoInicio > (*raiz)->enderecoFim){
				insere(&(*raiz)->dir, Novo);
				if( abs( altNo((*raiz)->esq)-altNo((*raiz)->dir)) == 2)
					rotacaoRR(raiz);
			}
		}
        (*raiz)->altura = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1;
	}
}

void mostra(No* raiz){
	if( raiz != NULL){
		mostra(raiz->esq);
		printf("(%c) |%d-%d| |%d-%d|\n",raiz->status, raiz->inicio, raiz->fim, raiz->enderecoInicio, raiz->enderecoFim);
		mostra(raiz->dir);
	}
}

int alocar(No** raiz, No** pai, int qtdBlocos, int *status){

	//Resul 1 para alocação concluida
	int resul = 0;

	if( *raiz != NULL){
		resul = alocar(&(*raiz)->esq,raiz, qtdBlocos, status);

		if(resul == 0){
			
			if((*raiz)->status == 'L'){
				int qtdLocal = (*raiz)->enderecoFim - (*raiz)->enderecoInicio +1;
				
				if( pai == NULL && (*raiz)->esq == NULL && (*raiz)->dir == NULL){		//apenas um nó
					
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'O';
                        *status = 1 - (*status);
					}else{
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int auxfim = (*raiz)->enderecoFim;
						(*raiz)->enderecoFim = newfim;

						int newinicio = newfim+1;

						insere(raiz, criaNo(status, (*raiz)->inicio,(*raiz)->fim,newinicio,auxfim));

					}
					resul = 1;
				}else if((*raiz)->esq == NULL && (*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						//Buscar os mais interNos para unir;
						int newfim = (*pai)->enderecoFim + qtdBlocos;
						(*pai)->enderecoFim = newfim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newfim = (*raiz)->enderecoFim - qtdBlocos;
							int newinicio = (*pai)->enderecoInicio - qtdBlocos;
							(*raiz)->enderecoFim = newfim;
							(*pai)->enderecoInicio = newinicio;
						}else{
							int newinicio = (*raiz)->enderecoInicio + qtdBlocos;
							int newfim = (*pai)->enderecoFim + qtdBlocos;
							(*raiz)->enderecoInicio = newinicio;
							(*pai)->enderecoFim = newfim;
						}
						resul = 1;
					}
				}else{
			
					//Buscando o mais a esquerda Ocupado.
					No** paiMaisEsq = raiz;
					No** maisEsq = &((*raiz)->dir);
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'O'){
						maisEsq = paiMaisEsq;
					}
					//free(*paiMaisEsq);		
					
					if(qtdLocal == qtdBlocos){

						//Buscando o mais a direita Ocupado.
						No** paiMaisDir = raiz;
						No** maisDir = &((*raiz)->esq);
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'O'){
							maisDir = paiMaisDir;
						}
						//free(*paiMaisDir);
						
						
						//Buscar os mais interNos para unir;
						int newinicio = (*maisDir)->enderecoInicio;
						int newfim = (*maisEsq)->enderecoFim;
						(*raiz)->enderecoInicio = newinicio;
						(*raiz)->enderecoFim = newfim;
						(*raiz)->status = 'O';

						
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int newinicio = (*maisEsq)->enderecoInicio - qtdBlocos;
						(*raiz)->enderecoFim = newfim;
						(*maisEsq)->enderecoInicio = newinicio;
						resul = 1;
					
					}else{
						resul = alocar(&(*raiz)->dir,raiz, qtdBlocos, status);
					}	
				}
			}else{
				resul = alocar(&(*raiz)->dir,raiz, qtdBlocos, status);
			}
		}
	}
	return resul;
}

int liberar(No** raiz, No** pai, int qtdBlocos, int* status){

	//Resul 1 para alocação concluida
	int resul = 0;

	if( *raiz != NULL){
		resul = liberar(&(*raiz)->esq,raiz, qtdBlocos, status);
	
		if(resul == 0){
				
			if((*raiz)->status == 'O'){
				int qtdLocal = (*raiz)->enderecoFim - (*raiz)->enderecoInicio +1;
				
				if( pai == NULL && (*raiz)->esq == NULL && (*raiz)->dir == NULL){
					
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'L';
					}else{
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int auxfim = (*raiz)->enderecoFim;
						(*raiz)->enderecoFim = newfim;

						int newinicio = newfim+1;

						insere(raiz, criaNo(status, (*raiz)->inicio,(*raiz)->fim,newinicio,auxfim));

					}
					resul = 1;
				}else if((*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						//Buscar os mais interNos para unir;
						int newfim = (*pai)->enderecoFim + qtdBlocos;
						(*pai)->enderecoFim = newfim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newfim = (*raiz)->enderecoFim - qtdBlocos;
							int newinicio = (*pai)->enderecoInicio - qtdBlocos;
							(*raiz)->enderecoFim = newfim;
							(*pai)->enderecoInicio = newinicio;
						}else{
							int newinicio = (*raiz)->enderecoInicio + qtdBlocos;
							int newfim = (*pai)->enderecoFim + qtdBlocos;
							(*raiz)->enderecoInicio = newinicio;
							(*pai)->enderecoFim = newfim;
						}
						resul = 1;
					}
				}else{
			
					//Buscando o mais a esquerda Ocupado.
					No** paiMaisEsq = raiz;
					No** maisEsq = &((*raiz)->dir);
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'L'){
						maisEsq = paiMaisEsq;
					}
					//free(*paiMaisEsq);		
					
					if(qtdLocal == qtdBlocos){

						//Buscando o mais a direita Ocupado.
						No** paiMaisDir = raiz;
						No** maisDir = &((*raiz)->esq);
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'L'){
							maisDir = paiMaisDir;
						}
						//free(*paiMaisDir);
	
						//Buscar os mais interNos para unir;
						int newinicio = (*maisDir)->enderecoInicio;
						int newfim = (*maisEsq)->enderecoFim;
						(*raiz)->enderecoInicio = newinicio;
						(*raiz)->enderecoFim = newfim;
						(*raiz)->status = 'L';

						
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int newinicio = (*maisEsq)->enderecoInicio - qtdBlocos;
						(*raiz)->enderecoFim = newfim;
						(*maisEsq)->enderecoInicio = newinicio;
						resul = 1;
					
					}else{
						resul = liberar(&(*raiz)->dir,raiz, qtdBlocos, status);
					}	
				}
			}else{
				resul = liberar(&(*raiz)->dir,raiz, qtdBlocos, status);
			}
		}
	}
	return resul;
}

int buscaAloca(No* raiz, int valor){
	int procura = 0;

	if(raiz != NULL){
		procura = buscaAloca(raiz->esq, valor);
		if((raiz)->status == 'L'){
			int qtdLocal = (raiz)->enderecoFim - (raiz)->enderecoInicio +1;
			if(valor <= qtdLocal){
				procura = 1;
			}
		}
		if(procura==0){
			procura = buscaAloca(raiz->dir, valor);
		}
	}
	return procura;
}

int buscaLibera(No* raiz, int valor){
	int procura = 0;

	if(raiz != NULL){
		procura = buscaLibera(raiz->esq, valor);
		if((raiz)->status == 'L'){
			int qtdLocal = (raiz)->enderecoFim - (raiz)->enderecoInicio +1;
			if(valor <= qtdLocal){
				procura = 1;
			}
		}
		if(procura==0){
			procura = buscaLibera(raiz->dir, valor);
		}
	}
	return procura;
}

