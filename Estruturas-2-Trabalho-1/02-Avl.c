#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct no{
    int info, altura;
    struct no *esq, *dir;
};

typedef struct no No;

No *criarNo(int n);
int altNo(No *raiz);
void rotacaoRR(No **no);
void rotacaoLL(No **no);
int insere(No **raiz, No *no);
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
        for(j=0; j<100000; j++){
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
    no->altura = 0;
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

int altNo(No *raiz){
    int alt;
    if(raiz == NULL){
        alt = -1;
    }else{
        alt = raiz->altura;
    }
    return alt;
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

void rotacaoRR(No **no){
    No *aux;
    aux = (*no)->dir;
    (*no)->dir = aux->esq;
    aux->esq = *no;
    (*no)->altura = maior(altNo((*no)->esq), altNo((*no)->dir)) + 1;
    *no = aux;
}

void rotacaoLL(No **no){
    No *aux;
    aux = (*no)->esq;
    (*no)->esq = aux->dir;
    aux->dir = *no;
    (*no)->altura = maior(altNo((*no)->esq), altNo((*no)->dir)) + 1;
    *no = aux;
}

int insere(No **raiz, No *no){
    int inseriu = 1;
    if(*raiz == NULL){
        *raiz = no;
    }else{
        if(no->info < (*raiz)->info){
            if(insere(&(*raiz)->esq, no) == 1){
                if(abs(altNo((*raiz)->esq) - altNo((*raiz)->dir)) == 2){
                    if(no->info < (*raiz)->esq->info){
                        rotacaoLL(raiz);
                    }else{
                        rotacaoRR(&(*raiz)->esq);
                        rotacaoLL(raiz);
                    }
                }
            }
        }else if(no->info > (*raiz)->info){
            if(insere(&(*raiz)->dir, no) == 1){
                if(abs(altNo((*raiz)->esq) - altNo((*raiz)->dir)) == 2){
                    if(no->info > (*raiz)->dir->info){
                        rotacaoRR(raiz);
                    }else{
                        rotacaoLL(&(*raiz)->dir);
                        rotacaoRR(raiz);
                    }
                }
            }
        }else{
            inseriu = 0;
        }
        (*raiz)->altura = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1;
    }
    return inseriu;
}

void libera(No **raiz){
    if(*raiz != NULL){
        libera(&(*raiz)->esq);
        libera(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}
