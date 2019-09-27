#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct no{
    int info;
    struct no *esq, *dir;
};

typedef struct no No;

No *criarNo(int n);
void insere(No **raiz, No *no);
void mostraPreOrdem(No *raiz);
int maior(int a, int b);
int menor(int a, int b);
int eFolha(No *no);
int noMProf(No *raiz, int var);
int noMenProf(No *raiz, int var);
int busca(No *raiz, int valor);
void libera(No **raiz);

void main(){
    srand(time(NULL));
    No *raiz = NULL, *no;
    int i, j, sort, procura = (rand() % 1000) + 1, *difProf, maisProf, menosProf;
    clock_t inicio, fim;
    difProf = (int*) calloc(sizeof(int), 40);
    for(i=0; i<30; i++){
        inicio = (long double)clock();
        for(j=0; j<1000; j++){
            sort = (rand() % 1000) + 1;
            no = criarNo(sort);
            insere(&raiz, no);
        }
        fim = (long double)clock();
        long double tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
        maisProf = noMProf(raiz, 0);
        menosProf = noMenProf(raiz, 0);
        printf("Nível da folha mais profunda %d\nNível da folha menos profunda %d\n", maisProf, menosProf);
        difProf[maisProf-menosProf]+=1;
        printf("Tempo de insercao %Lf\n", tempo);
        inicio = (long double)clock();
        busca(raiz, procura);
        fim = (long double)clock();
        tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
        printf("Tempo de busca %Lf\n", tempo);
        libera(&raiz);
    }
    printf("Diferencas entre profundidades maximas e minimas:\n");
    for(i=0; i<40; i++){
        if(difProf[i] != 0){
            printf("A diferenca %d ocorreu %d.\n", i, difProf[i]);
        }
    }
}

No *criarNo(int n){
    No *no = (No*) malloc(sizeof(No));
    no->info = n;
    no->dir = NULL;
    no->esq = NULL;
    return no;
}

int maior(int a, int b){
    int mai;
    if(a > b){
        mai = a;
    }else{
        mai = b;
    }
    return mai;
}

int menor(int a, int b){
    int men;
    if(a == -1){
        men = b;
    }else if(b == -1){
        men = a;
    }else{
        if(a < b){
            men = a;
        }else{
            men = b;
        }
    }
    return men;
}


void insere(No **raiz, No *no){
    if(*raiz == NULL){
        *raiz = no;
    }else{ 
        if(no->info < (*raiz)->info){
            insere(&(*raiz)->esq, no);
        }else{
            insere(&(*raiz)->dir, no);
        }
    }
}

void mostraPreOrdem(No *raiz){
    if(raiz != NULL){
        printf("%d ", raiz->info);
        mostraPreOrdem(raiz->esq);
        mostraPreOrdem(raiz->dir);
    }
}

int eFolha(No *no){
    int ret = 0;
    if((no->esq == NULL) && (no->dir == NULL)){
        ret = 1;
    }
    return ret;
}

int noMProf(No *raiz, int var){
    int val = -1;
    if(raiz != NULL){
        if(eFolha(raiz)){
            val = var;
        }else{
            val = maior(noMProf(raiz->esq, var+1), noMProf(raiz->dir, var+1));
        }
    }
    return val;
}

int noMenProf(No *raiz, int var){
     int val = -1;
    if(raiz != NULL){
        if(eFolha(raiz)){
            val = var;
        }else{
            val = menor(noMenProf(raiz->esq, var+1), noMenProf(raiz->dir, var+1));
        }
    }
    return val;
}

int busca(No *raiz, int valor){
    int bus = 0;
    if(raiz != NULL){
        if(valor < raiz->info){
            bus = busca(raiz->esq, valor);
        }else if(valor > raiz->info){
            bus = busca(raiz->dir, valor);
        }else if(valor == raiz->info){
            bus = 1;
        }
    }
    return bus;
}

void libera(No **raiz){
    if(*raiz != NULL){
        libera(&(*raiz)->esq);
        libera(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}
