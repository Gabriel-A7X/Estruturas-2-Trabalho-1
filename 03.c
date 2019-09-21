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
void insereLista(ing **palavra, char *p);
void mostraLista(ing *lista);
void apagarLista(ing **lista);
arv *menorDireita(arv *raiz);
void copiaLista(ing **lista, ing *lis);
void removeNo(arv **raiz, char *p);
int menu();
void lerArquivo(char *caminho, cap **uni);
void mostraTodasUnidades(cap *uni);
void mostraUnidade(cap *uni, char *p);
void mostraEmOrdem(arv *raiz);
arv* procuraUnidade(cap *uni, char *p, int *certo);

void main(){
    arv *no, *aux;
    cap *uni = NULL;
    int choice, certo;
    char procura[100], arq[100], unidade[100];
    do{
        choice = menu();
        switch(choice){
        case 0:
            break;
        case 1:
            printf("Digite o nome do arquivo com a extensao:\n");
            setbuf(stdin, NULL);
            scanf("%s", arq);
            lerArquivo(arq, &uni);
            break;
        case 2:
            printf("Digite o nome da unidade:\n");
            setbuf(stdin, NULL);
            scanf("%s", procura);
            mostraUnidade(uni, procura);
            break;
        case 3:
            mostraTodasUnidades(uni);
            break;
        case 4:
            certo = 0;
            printf("Digite o nome da unidade:\n");
            setbuf(stdin, NULL);
            scanf("%s", unidade);
            aux = procuraUnidade(uni, unidade, &certo);
            if(certo == 1){
                printf("Digite o nome da palavra:\n");
                setbuf(stdin, NULL);
                scanf("%s", procura);
                removeNo(&aux, procura);
            }else{
                printf("Essa unidade nao existe.\n");
            }
            break;
        default:
            printf("Informacao invalida!\n");
            break;
        }
    }while(choice != 0);
}

int menu(){
    int choice;
    printf("1-Ler Arquivo\n2-Mostrar Uma Unidade\n3-Mostrar Todas as Unidades\n4-Remover uma Palavra\n0-Sair\n");
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


void mostraEmOrdem(arv *raiz){
    if(raiz != NULL){
        mostraEmOrdem(raiz->esq);
        printf("%s | ", raiz->port);
        mostraLista(raiz->palavras);
        printf("\n");
        mostraEmOrdem(raiz->dir);
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
        return;        
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
        mostraEmOrdem(uni->arvore);
        mostraTodasUnidades(uni->prox);
    }
}

void mostraUnidade(cap *uni, char *p){
    cap *aux=uni;
    for(uni; aux != NULL; aux=aux->prox){
        if(strcmp(aux->unidade, p) == 0){
            printf("Unidade: %s\n", aux->unidade);
            mostraEmOrdem(aux->arvore);
        }
    }
}

arv* procuraUnidade(cap *uni, char *p, int *certo){
    arv  *caca;
    cap *aux=uni;
    for(uni; aux != NULL; aux=aux->prox){
        if(strcmp(aux->unidade, p) == 0){
            caca = aux->arvore;
            *certo = 1;
            break;
        }
    }
    return caca;
}