#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct no{
    int info, cor;
    struct no *esq, *dir;
};

typedef struct no No;

No* criaNo(int valor);
int eFolha(No *no);
void mostraPreOrdem(No *raiz);
No* rotacaoEsquerda(No *a);
No* rotacaoDireita(No *a);
No* balancear(No *h);
void trocaCor(No *h);
int cor(No *h);
int consulta(No *raiz, int valor);
void insereNo(No **raiz, No *no);
int maior(int a, int b);
int menor(int a, int b);
int noMProf(No *raiz, int var);
int noMenProf(No *raiz, int var);
void libera(No **raiz);

void main(){
    srand(time(NULL));
    No *raiz = NULL, *no;
    int i, j, sort, procura = (rand() % 1000) + 1, *difProf, maisProf, menosProf;
    clock_t inicio, fim;
    difProf = (int*) calloc(sizeof(int), 100);
    for(i=0; i<30; i++){
        inicio = (long double)clock();
        for(j=0; j<1000; j++){
            sort = (rand() % 1000) + 1;
            no = criaNo(sort);
            insereNo(&raiz, no);
        }
        fim = (long double)clock();
        long double tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
        maisProf = noMProf(raiz, 0);
        menosProf = noMenProf(raiz, 0);
        printf("Nível da folha mais profunda %d\nNível da folha menos profunda %d\n", maisProf, menosProf);
        difProf[maisProf-menosProf]+=1;
        printf("Tempo de insercao %Lf\n", tempo);
        inicio = (long double)clock();
        consulta(raiz, procura);
        fim = (long double)clock();
        tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
        printf("Tempo de busca %Lf\n", tempo);
	libera(&raiz);
	raiz=NULL;
    }
    printf("Diferencas entre profundidades maximas e minimas:\n");
    for(i=0; i<100; i++){
        if(difProf[i] != 0){
            printf("A diferenca %d ocorreu %d.\n", i, difProf[i]);
        }
    }
}

No* criaNo(int valor){
    No *new = (No*) malloc(sizeof(No));
    new->info = valor;
    new->cor = 0;
    new->dir = NULL;
    new->esq = NULL;
    return new;
}

No* rotacaoEsquerda(No *a){
    No *b = a->dir;
    a->dir = b->esq;
    b->esq = a;
    a->cor = b->cor;
    b->cor = 0;
    return b;
}

No* rotacaoDireita(No *a){
    No *b = a->esq;
    a->esq = b->dir;
    b->dir = a;
    b->cor = a->cor;
    a->cor = 0;
    return b;
}


int cor(No *h){
    int c = 1;
    if(h != NULL){
        c = h->cor;
    }
    return c;
}

void trocaCor(No *h){
    h->cor = 1 - h->cor;
    if(h->esq != NULL){
        h->esq->cor = 1 - h->esq->cor;
    }
    if(h->dir != NULL){
        h->dir->cor = 1 - h->dir->cor;
    }
}

No* balancear(No *h){
    //Nó vermelho é sempre filho da esquerda.
    if(cor(h->dir) == 0 && cor(h->esq) == 1){
        h = rotacaoEsquerda(h);
    }
    //Filho da direita e neto esquerda são vermelhos.
    if(h->esq != NULL && cor(h->esq) == 0 && cor(h->esq->esq) == 0){
        h = rotacaoDireita(h);
    }
    //2 filhos vermelhos troca cor.
    if(cor(h->esq) == 0 && cor(h->dir) == 0){
        trocaCor(h);
    }
    return h;
}

int consulta(No *raiz, int valor){
    int aux = 0;
    if(raiz != NULL){
        if(valor < raiz->info){
            aux = consulta(raiz->esq, valor);
        }else if(valor > raiz->info){
            aux = consulta(raiz->dir, valor);
        }else{
            aux = 1;
        }
    }
    return aux;
}

void insereNo(No **raiz, No *no){
    if(*raiz != NULL){
        if(no->info < (*raiz)->info){
            insereNo(&(*raiz)->esq, no);
        }else if(no->info > (*raiz)->info){
            insereNo(&(*raiz)->dir, no);
        }
        *raiz = balancear(*raiz);
    }else{
        *raiz = no;
    }
}

void mostraPreOrdem(No *raiz){
    if(raiz != NULL){
        if(raiz->cor == 0){
            printf("Info: %d | Cor: Vermelho\n", raiz->info);
        }else{
            printf("Info: %d | Cor: Preto\n", raiz->info);
        }
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

void libera(No **raiz){
    if(*raiz != NULL){
        libera(&(*raiz)->esq);
        libera(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}
