#include <stdio.h>
#include <stdlib.h>

struct no{
    int altura, inicio, fim;
    char ol;
    struct no *esq, *dir;
};

typedef struct no No;

int menu();
No *criarNo(int inicio, int fim, char ol);
void rotacaoRR(No **no);
void rotacaoLL(No **no);
void criarArvore(No **raiz);
int insere(No **raiz, No *no);
int altNo(No *raiz);
int maior(int a, int b);
int eFolha(No *no);
void mostraPreOrdem(No *raiz);
void mostraEmOrdem(No *raiz);
void LiberaNo(No **raiz, int qut, No *pai);

void main(){
    No *arv = NULL, *no;
    int tam, inicio = 0, fim, choice, valor;
    char ol;
    printf("Digite o tamanho da memoria em Megabytes:\n");
    scanf("%d", &tam);
    tam--;
    printf("Informe se primeiro endereco e livre ou ocupado:\n");
    setbuf(stdin, NULL);
    scanf("%c", &ol);
    while(inicio <= tam){
        printf("Digite o fim do bloco:\n");
        scanf("%d", &fim);
        if(fim > tam){
            fim = tam;
        }
        no = criarNo(inicio, fim, ol);
        insere(&arv, no);
        if(ol == 'O'){
            ol = 'L';
        }else{
            ol = 'O';
        }
        inicio = fim + 1;
    }
    do{
        choice = menu();
        switch (choice){
            case 0:
                break;
            case 1:
                printf("Informe a quantidade e nos que deseja alocar:\n");
                scanf("%d", &valor);
                if(valor > tam){
                    printf("Nao e possivel alocar essa quantidade de nos.\n");
                }
                break;
            case 3:
                mostraEmOrdem(arv);
                break;
            case 4:
                mostraPreOrdem(arv);
                break;
        }
    }while(choice != 0);
}

int menu(){
    int choice;
    printf("1-Alocar\n2-Liberar\n3-Mostrar Em Ordem\n4-Mostrar Pre-Ordem\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
}

No *criarNo(int inicio, int fim, char ol){
    No *no = (No*) malloc(sizeof(No));
    no->inicio = inicio;
    no->fim = fim;
    no->ol = ol;
    no->altura = 0;
    no->dir = NULL;
    no->esq = NULL;
    return no;
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

int maior(int a, int b){
    int mai;
    if(a > b){
        mai = a;
    }else{
        mai = b;
    }
    return mai;
}

int eFolha(No *no){
    int ret = 0;
    if((no->esq == NULL) && (no->dir == NULL)){
        ret = 1;
    }
    return ret;
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
        if(no->fim < (*raiz)->inicio){
            if(insere(&(*raiz)->esq, no) == 1){
                if(abs(altNo((*raiz)->esq) - altNo((*raiz)->dir)) == 2){
                    if(no->fim < (*raiz)->esq->inicio){
                        rotacaoLL(raiz);
                    }else{
                        rotacaoRR(&(*raiz)->esq);
                        rotacaoLL(raiz);
                    }
                }
            }
        }else if(no->inicio > (*raiz)->fim){
            if(insere(&(*raiz)->dir, no) == 1){
                if(abs(altNo((*raiz)->esq) - altNo((*raiz)->dir)) == 2){
                    if(no->inicio > (*raiz)->dir->fim){
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

void mostraPreOrdem(No *raiz){
    if(raiz != NULL){
        if(raiz->ol == 'O'){
            printf("Inicio: %d Fim: %d Ocupado.\n", raiz->inicio, raiz->fim);
        }else{
            printf("Inicio: %d Fim: %d Livre.\n", raiz->inicio, raiz->fim);
        }
        mostraPreOrdem(raiz->esq);
        mostraPreOrdem(raiz->dir);
    }
}

void mostraEmOrdem(No *raiz){
    if(raiz != NULL){
        mostraEmOrdem(raiz->esq);
        if(raiz->ol == 'O'){
            printf("Inicio: %d Fim: %d Ocupado.\n", raiz->inicio, raiz->fim);
        }else{
            printf("Inicio: %d Fim: %d Livre.\n", raiz->inicio, raiz->fim);
        }
        mostraEmOrdem(raiz->dir);
    }
}

void LiberaNo(No **raiz, int qut, No *pai){
    int inicio;
    No *no;
    if(raiz != NULL){
        if(efolha(*raiz)){
            if((*raiz)->ol == 'O' && (*raiz)->fim - (*raiz)->inicio >= qut){
                if((*raiz)->inicio + qut == (*raiz)->fim){
                    pai->fim = (*raiz)->fim;
                    free(*raiz);
                    *raiz = NULL;
                }else if(pai->dir == *raiz){
                    inicio = (*raiz)->inicio + qut;
                    (*raiz)->inicio = inicio;
                    pai->fim = inicio - 1;
                }else{
                    inicio = (*raiz)->inicio;
                    (*raiz)->inicio += qut;
                    no = criarNo(inicio, (*raiz)->inicio-1, 'L');
                    (*raiz)->esq = no;
                }
            }
        }
    }
}