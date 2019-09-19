#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ingles{
    char eng[50];
    struct ingles *prox;
};

typedef struct ingles ing;

struct no{
    char port[50];
    ing *palavras;
    struct no *esq, *dir;
};

typedef struct no arv;

struct capitulo{
    char unidade[50];
    arv *arvore;
    struct capitulo *prox;
};

typedef struct capitulo cap;

arv *criarNo(char *p, char *another);
void *inserirCapitulo(cap **uni, char *nome);
ing *criaPalavra(char *p);
int eFolha(arv *no);
void insereArv(arv **raiz, arv *no);
void mostraPreOrdem(arv *raiz);
void insereLista(ing **palavra, char *p);
void mostraLista(ing *lista);
void apagarLista(ing **lista);
arv *menorDireita(arv *raiz);
void copiaLista(ing **lista, ing *lis);
void removeNo(arv **raiz, char *p);
int menu();
void lerArquivo(char *caminho, cap **uni);
void mostraTodasUnidades(cap *uni);
void mostraUnidade(cap *uni);
void procuraUnidade(cap *uni, char *p);


void main(){
    arv *no;
    cap *uni = NULL;
    int choice;
    char procura[100];
    lerArquivo("Teste.txt", &uni);
    do{
        choice = menu();
        switch(choice){
        case 0:
            break;
        case 1:
            printf("Digite o nome da unidade:\n");
            scanf("%s", procura);
            procuraUnidade(uni, procura);
            break;
        case 2:
            mostraTodasUnidades(uni);
            break;
        case 3:
            printf("Digite o nome da palavra:\n");
            scanf("%s", procura);
            removeNo(&uni->arvore, procura);
            break;
        default:
            printf("Informacao invalida!\n");
            break;
        }
    }while(choice != 0);
}

int menu(){
    int choice;
    printf("1-Mostrar Uma Unidade\n2-Mostrar Todas as Unidades\n3-Remover uma Palavra\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
}

arv *criarNo(char *p, char *another){
    arv *no = (arv*) malloc(sizeof(arv));
    strcpy(no->port, p);
    no->palavras = NULL;
    insereLista(&(no->palavras), another);
    no->dir = NULL;
    no->esq = NULL;
    return no;
}

int eFolha(arv *no){
    int ret = 0;
    if((no->esq == NULL) && (no->dir == NULL)){
        ret = 1;
    }
    return ret;
}

void insereArv(arv **raiz, arv *no){
    if(*raiz == NULL){
        *raiz = no;
    }else{ 
        int compara = strcmp((*raiz)->port, no->port);
        if(compara > 0){
            insereArv(&(*raiz)->esq, no);
        }else if(compara < 0){
            insereArv(&(*raiz)->dir, no);
        }else{
             insereLista(&(*raiz)->palavras, no->palavras->eng);
        }
    }
}

void mostraPreOrdem(arv *raiz){
    if(raiz != NULL){
        printf("%s | ", raiz->port);
        mostraLista(raiz->palavras);
        printf("\n");
        mostraPreOrdem(raiz->esq);
        mostraPreOrdem(raiz->dir);
    }
}

void mostraLista(ing *lista){
    if(lista != NULL){
        printf("%s", lista->eng);
        if(lista->prox != NULL){
            printf(", ");
        }else{
            printf(".");
        }
        mostraLista(lista->prox);
    }
}

void *inserirCapitulo(cap **uni ,char *nome){
    cap *new = (cap*) malloc(sizeof(cap));
    strcpy(new->unidade, nome);
    new->arvore = NULL;
    new->prox = *uni;
    *uni = new; 
}

ing *criaPalavra(char *p){
    ing *new = (ing*) malloc(sizeof(ing));
    strcpy(new->eng, p);
    new->prox = NULL;
    return new;
}

void insereLista(ing **palavra, char *p){
    ing *new = (ing*) malloc(sizeof(ing));
    strcpy(new->eng, p);
    new->prox = *palavra;
    *palavra = new;
}

void apagarLista(ing **lista){
    if(*lista != NULL){
        apagarLista(&(*lista)->prox);
        free(*lista);
    }
    *lista = NULL;
}

arv *menorDireita(arv *raiz){
    arv *aux = (arv*) malloc(sizeof(arv));
    if(raiz->esq != NULL){
        aux = menorDireita(raiz->esq);
    }else{
        aux = raiz;
    }
    return aux;
}

void copiaLista(ing **lista, ing *lis){
    if(lis != NULL){
        insereLista(lista, lis->eng);
        copiaLista(lista, lis->prox);
    }
}

void removeNo(arv **raiz, char *p){
    if(*raiz != NULL){
        int comp = strcmp(p, (*raiz)->port);
        if(comp < 0){
            removeNo(&(*raiz)->esq, p);
        }else if(comp > 0){
            removeNo(&(*raiz)->dir, p);
        }else{
            if(eFolha(*raiz)){
                apagarLista(&(*raiz)->palavras);
                free(*raiz);
                *raiz = NULL;
            }else{
                arv *aux;
                if((*raiz)->esq == NULL){
                    aux = (*raiz)->dir;
                    apagarLista(&(*raiz)->palavras);
                    free(*raiz);
                    *raiz = aux;
                }else if((*raiz)->dir == NULL){
                    aux = (*raiz)->esq;
                    apagarLista(&(*raiz)->palavras);
                    free(*raiz);
                    *raiz = aux;
                }else{
                    aux = menorDireita((*raiz)->dir);
                    strcpy((*raiz)->port, aux->port);
                    apagarLista(&(*raiz)->palavras);
                    copiaLista(&(*raiz)->palavras, aux->palavras);
                    removeNo(&(*raiz)->dir, (*raiz)->port);
                }
            }
        }
    }
}

void lerArquivo(char *caminho, cap **uni){
    FILE *fptr;
    int i, j;
    char palavras[100], nome[100], ingles[100];
    fptr = fopen(caminho, "r");
    if( fptr == NULL ){
        printf("Erro na abertura de arquivo!\n");
        exit(1);        
    }else{
        printf("Arquivo aberto com sucesso!\n");
    }
    while (!feof(fptr)){
        fscanf(fptr, "%s", palavras);
        j = 0;
        if(palavras[0] == '%'){
            for(i=1; palavras[i] != '\0'; i++){
                nome[j] = palavras[i];
                j++;
            }
            nome[j] = '\0';
            inserirCapitulo(uni, nome);
        }else{
            for(i=0; palavras[i] != ':'; i++){
                ingles[j] = palavras[i];
                j++;
            }
            ingles[j] = '\0';
            j = 0;
            for(i=i+1; palavras[i] != '\0'; i++){
                nome[j] = palavras[i];
                j++;
                if(palavras[i] == ','){
                    nome[j-1] = '\0';
                    insereArv(&(*uni)->arvore, criarNo(nome, ingles));
                    j = 0;
                }
            }
            nome[j] = '\0';
            insereArv(&(*uni)->arvore, criarNo(nome, ingles));
        }
    }
}

void mostraTodasUnidades(cap *uni){
    if(uni != NULL){
        printf("Unidade: %s\n", uni->unidade);
        mostraPreOrdem(uni->arvore);
        mostraTodasUnidades(uni->prox);
    }
}

void procuraUnidade(cap *uni, char *p){
    int valor, i;
    cap *aux=uni;
    for(uni; aux != NULL; aux=aux->prox){
        if(strcmp(aux->unidade, p) == 0){
            printf("Unidade: %s\n", aux->unidade);
            mostraPreOrdem(aux->arvore);
        }
    }
}