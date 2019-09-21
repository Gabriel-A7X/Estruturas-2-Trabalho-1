#include <stdio.h>
#include <stdlib.h>

//Preto = 1 Vermelho = 0
struct no{
    int info, cor;
    struct no *esq, *dir;
};

typedef struct no No;

int menu();
No* criaNo(int valor);
void mostraPreOrdem(No *raiz);
No* rotacaoEsquerda(No *a);
No* rotacaoDireita(No *a);
No* move2EsqRed(No* h);
No* move2DirRed(No* h);
No* balancear(No *h);
int removeVB(No **raiz, int valor);
No *removeMenor(No *h);
No* procuraMenor(No *atual);
No* removeNo(No *h, int valor);
void trocaCor(No *h);
int cor(No *h);
int consulta(No *raiz, int valor);
void insereNo(No **raiz, No *no);

void main(){
    No *raiz = NULL, *no;
    int choice, n;
    do{
        choice = menu();
        switch (choice){
            case 0:
                break;
            case 1:
                printf("Digite o valor para inserir:\n");
                scanf("%d", &n); 
                no = criaNo(n);
                insereNo(&raiz, no);
                raiz->cor = 1; 
                break;
            case 2:
                mostraPreOrdem(raiz);
                break;
            case 3:
                printf("Digite o valor para remover:\n");
                scanf("%d", &n); 
                raiz = removeNo(raiz, n);
                break;
        }
    }while(choice != 0);
}

int menu(){
    int choice;
    printf("1-Inserir No\n2-Mostrar Pre-Ordem\n3-Excluir No\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
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

No* move2EsqRed(No* h){
    trocaCor(h);
    if(cor(h->dir->esq) == 0){
        h->dir = rotacaoDireita(h->dir);
        h = rotacaoEsquerda(h);
        trocaCor(h);
    }
    return h;
}

No* move2DirRed(No* h){
    trocaCor(h);
    if(cor(h->esq->dir) == 0){
        h->esq = rotacaoDireita(h->esq);
        h = rotacaoEsquerda(h);
        trocaCor(h);
    }
    return h;
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
    if(cor(h->dir) == 0){
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

int removeVB(No **raiz, int valor){
    int removeu = 0;
    if(consulta(*raiz, valor)){
        No *h = *raiz;
        *raiz = removeNo(h, valor);
        if(*raiz != NULL){
            (*raiz)->cor = 1;
            removeu =  1;
        }
    }
    return removeu;
}

No *removeMenor(No *h){
    if(h->esq == NULL){
        free(h);
        return NULL;
    }
    if(cor(h->esq) == 1 && cor(h->esq->esq) == 1){
        h = move2EsqRed(h);
    }
    h->esq = removeMenor(h->esq);
    return balancear(h);
}

No* procuraMenor(No *atual){
    No *no1 = atual, *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

No* removeNo(No *h, int valor){
    if(valor < h->info){
        if(cor(h->esq) == 1 && cor(h->esq->esq) == 1){
            h = move2EsqRed(h);
        }
        h->esq = removeNo(h->esq, valor);
    }else{
        if(cor(h->esq) == 0){
            h = rotacaoDireita(h);
        }
        if(valor == h->info && h->dir == NULL){
            free(h);
            return NULL;
        }
        if(cor(h->dir) == 1 && cor(h->dir->esq) == 1){
            h = move2DirRed(h);
        }
        if(valor == h->info){
            No *x = procuraMenor(h->dir);
            h->info = x->info;
            h->dir = removeMenor(h->dir);
        }else{
            h->dir = removeNo(h->dir, valor);
        }
    }
    return balancear(h);
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