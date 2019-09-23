#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

struct ingles{
    char *eng;
    struct ingles *prox;
};

typedef struct ingles ing;

struct arv{
    char *info1;
    char *info2;
    ing *l1,*l2;
    struct arv *esq,*cen,*dir;
    int qtd;
};

typedef struct arv arv;

struct capitulo{
    char *unidade;
    arv *arvore;
    struct capitulo *prox;
};

typedef struct capitulo cap;


int menu();
void mostraLista(ing *lista);
void insereLista(ing **palavra, char *p);
arv *criaNo(char *port,char *ingl,arv *esq,arv *cen,arv *dir);
arv *criaNoL(char *port,ing *ingl,arv *esq,arv *cen,arv *dir);
arv *quebraNo(arv **raiz,char *info,ing *ingl,char *promovePal,ing **promoveL,arv *sub);
arv *adcNO(arv *no,char *info,ing *lis,arv *filho);
int efolha(arv *no);
arv *insere(arv **raiz,char *info,char *ingl,char *promove,ing **promoveL,arv *pai);
void mostraInOrdem(arv *raiz);
void Mostrar(cap *uni);
void lerArquivo(char *caminho, cap **uni);
void *inserirCapitulo(cap **uni, char *nome);
int remove23(arv **raiz, char *pal, arv **pai);
void apagarLista(ing **lista);
void copiaLista(ing **lista, ing *lis);
void menorInfoDir(arv *raiz, arv **no, arv **paiNo);
void maiorInfoEsq(arv *raiz, arv **no, arv **paiNo);
void mostraUnidade(cap *uni, char *p);
cap* procuraUnidade(cap *uni, char *p, int *certo);
int procuraPalavra(arv *uni, char *p);

int main(){
    arv *no;
    cap *uni = NULL, **aux;
    aux=(cap**)malloc(sizeof(cap*));
    int choice, certo;
    char procura[100], arq[100], unidade[100];
    clock_t inicio, fim;
    long double tempo;
    do{
        choice = menu();
        switch(choice){
        case 0:
            break;
        case 1:
            printf("Digite o nome do arquivo com a extensao:\n");
            setbuf(stdin, NULL);
            scanf("%s", arq);
            inicio = (long double)clock();
            lerArquivo(arq, &uni);
            fim = (long double)clock();
            tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
            printf("Tempo de insercao %Lf\n", tempo);
            break;
        case 2:
            printf("Digite o nome da unidade:\n");
            scanf("%s", procura);
            mostraUnidade(uni, procura);
            break;
        case 3:
            Mostrar(uni);
            break;
        case 4:
            certo = 0;
            printf("Digite o nome da unidade:\n");
            setbuf(stdin, NULL);
            scanf("%s", unidade);
            *aux = procuraUnidade(uni, unidade, &certo);
            if(certo == 1){
                printf("Digite o nome da palavra:\n");
                setbuf(stdin, NULL);
                scanf("%s", procura);
                arv **pai=(arv**)malloc(sizeof(arv*));
                *pai=NULL;
                remove23(&(*aux)->arvore, procura, pai);
            }else{
                printf("Essa unidade nao existe.\n");
            }
            break;
        case 5:
            certo = 0;
            printf("Digite o nome da unidade:\n");
            setbuf(stdin, NULL);
            scanf("%s", unidade);
            inicio = (long double)clock();
            *aux = procuraUnidade(uni, unidade, &certo);
            fim = (long double)clock();
            tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
            printf("Tempo de busca da unidade %Lf\n", tempo);
            if(certo == 1){
                printf("Digite o nome da palavra:\n");
                setbuf(stdin, NULL);
                scanf("%s", procura);
                inicio = (long double)clock();
                if(procuraPalavra((*aux)->arvore, procura) == 1){
                    printf("Palavra Encontrada!\n");
                }else{
                    printf("Palavra nao encontrada!\n");
                }
                fim = (long double)clock();
                tempo = ((fim - inicio)/((long double)CLOCKS_PER_SEC/1000.0));
                printf("Tempo de busca da palavra %Lf\n", tempo);
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
    printf("1-Ler Arquivo\n2-Mostrar Uma Unidade\n3-Mostrar Todas as Unidades\n4-Remover uma Palavra\n5-Buscar Palavra\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
}

void Mostrar(cap *uni){
    if(uni!=NULL){
        Mostrar(uni->prox);
        printf("%s\n",uni->unidade);
        mostraInOrdem(uni->arvore);
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

void apagarLista(ing **lista){
    if(*lista != NULL){
        apagarLista(&(*lista)->prox);
        free(*lista);
    }
    *lista = NULL;
}

void insereLista(ing **palavra, char *p){
    ing *new = (ing*) malloc(sizeof(ing));
    new->eng=(char*)malloc(sizeof(char)*100);
    strcpy(new->eng,p);
    new->prox = *palavra;
    *palavra = new;
}

arv *criaNo(char *port,char *ingl,arv *esq,arv *cen,arv *dir){
    arv *no=(arv*)malloc(sizeof(arv));
    no->info1=(char*)malloc(sizeof(char)*100);
    no->info2=(char*)malloc(sizeof(char)*100);
    strcpy(no->info1,port);
    no->l1=NULL;
    no->l2=NULL;
    insereLista(&(no->l1),ingl);
    no->qtd=1;
    no->esq=esq;
    no->cen=cen;
    no->dir=dir;
    return no;
}

arv *criaNoL(char *port,ing *ingl,arv *esq,arv *cen,arv *dir){
    arv *no=(arv*)malloc(sizeof(arv));
    no->info1=(char*)malloc(sizeof(char)*100);
    no->info2=(char*)malloc(sizeof(char)*100);
    strcpy(no->info1,port);
    copiaLista(&no->l1,ingl);
    no->l2=NULL;
    no->qtd=1;
    no->esq=esq;
    no->cen=cen;
    no->dir=dir;
    return no;
}

arv *quebraNo(arv **raiz,char *info,ing *ingl,char *promovePal,ing **promoveL,arv *sub){
    arv *aux;
    if(strcmp(info,(*raiz)->info1)<0){
        strcpy(promovePal,(*raiz)->info1);
        apagarLista(promoveL);
        copiaLista(promoveL,(*raiz)->l1);
        strcpy((*raiz)->info1,info);
        apagarLista(&(*raiz)->l1);
        copiaLista(&(*raiz)->l1,ingl);
        (*raiz)->qtd-=1;
        aux=criaNoL((*raiz)->info2,(*raiz)->l2,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->l2=NULL;
        (*raiz)->dir=NULL;
        (*raiz)->cen=sub;
    }else if(strcmp(info,(*raiz)->info2)<0){
        strcpy(promovePal,info);
        apagarLista(promoveL);
        copiaLista(promoveL,ingl);
        (*raiz)->qtd-=1;
        aux=criaNoL((*raiz)->info2,(*raiz)->l2,sub,(*raiz)->dir,NULL);

        (*raiz)->l2=NULL;
        (*raiz)->dir=NULL;
    }else{
        strcpy(promovePal,(*raiz)->info2);
        apagarLista(promoveL);
        *promoveL=NULL;
        copiaLista(promoveL,(*raiz)->l2);
        apagarLista(&(*raiz)->l2);
        (*raiz)->qtd-=1;
        aux=criaNoL(info,ingl,(*raiz)->dir,sub,NULL);
        (*raiz)->l2=NULL;
        (*raiz)->dir=NULL;
    }
    return aux;
}

arv *adcNO(arv *no,char *info,ing *lis,arv *filho){
    if(strcmp(no->info1,info)<0){
        strcpy(no->info2,info);
        no->l2=lis;
        no->dir=filho;
    }else{
        strcpy(no->info2,no->info1);
        no->l2=no->l1;
        no->dir=no->cen;
        strcpy(no->info1,info);
        no->l1=lis;
        no->cen=filho;
    }
    no->qtd=2;
    return no;
}

int efolha(arv *no){
    int r=0;
    if((no)->esq == NULL && (no)->cen == NULL && (no)->dir == NULL){
        r = 1;
    }
    return r;
}

arv *insere(arv **raiz,char *info,char *ingl,char *promove,ing **promoveL,arv *pai){
    arv *aux = NULL;
    ing *aux2=NULL;
    if(*promoveL==NULL){
        insereLista(&aux2,ingl);
    }    
    if(*raiz == NULL){
        *raiz=criaNo(info,ingl,NULL,NULL,NULL);
        aux=NULL;
    }else if(strcmp(info,(*raiz)->info1)==0){
        insereLista(&(*raiz)->l1,ingl);
    }else if((*raiz)->qtd == 2 && strcmp(info,(*raiz)->info2) == 0){
        insereLista(&(*raiz)->l2,ingl);
    }else if(efolha(*raiz)){
        if((*raiz)->qtd == 1){
            *raiz=adcNO(*raiz,info,aux2,NULL);
            aux=NULL;
        }else{
            aux=quebraNo(raiz,info,aux2,promove,promoveL,NULL);
        }
    }else{
        if(strcmp(info,(*raiz)->info1)<0){
            aux=insere(&((*raiz)->esq),info,ingl,promove,promoveL,*raiz);
        }else if((*raiz)->qtd==1 || strcmp(info,(*raiz)->info2)<0){
            aux=insere(&((*raiz)->cen),info,ingl,promove,promoveL,*raiz);
        }else{
            aux=insere(&((*raiz)->dir),info,ingl,promove,promoveL,*raiz);
        }
    }
    if(aux!=NULL){
        char *copia=(char*)malloc(sizeof(char)*100);
        strcpy(copia,promove);
        ing *lista=NULL;
        copiaLista(&lista,*promoveL);
        if(pai==NULL){
            *raiz=criaNoL(promove,*promoveL,*raiz,aux,NULL);
            aux=NULL;
        }else if(pai->qtd==1){
            pai=adcNO(pai,promove,*promoveL,aux);
            aux=NULL;
        }else{
            aux=quebraNo(&pai,copia,lista,promove,promoveL,aux);
        }
    }
    return aux;
}



void mostraInOrdem(arv *raiz){
    if(raiz!=NULL){
        mostraInOrdem(raiz->esq);
        printf("%s | ",raiz->info1);
        mostraLista(raiz->l1);
        printf("\n");
        mostraInOrdem(raiz->cen);
        if(raiz->qtd == 2){
            printf("%s | ",raiz->info2);
            mostraLista(raiz->l2);
            printf("\n");
            mostraInOrdem(raiz->dir);
        }
    }
}

void mostraUnidade(cap *uni, char *p){
    cap *aux=uni;
    for(uni; aux != NULL; aux=aux->prox){
        if(strcmp(aux->unidade, p) == 0){
            printf("Unidade: %s\n", aux->unidade);
            mostraInOrdem(aux->arvore);
        }
    }
}

void *inserirCapitulo(cap **uni ,char *nome){
    cap *new = (cap*) malloc(sizeof(cap));
    new->unidade=(char*)malloc(sizeof(char)*100);
    strcpy(new->unidade, nome);
    new->arvore = NULL;
    new->prox = *uni;
    *uni = new; 
}

void lerArquivo(char *caminho, cap **uni){
    FILE *fptr;
    int i, j;
    char *palavras, *nome, *ingles, *promove;
    promove=(char*)malloc(sizeof(char)*100);
    palavras=(char*)malloc(sizeof(char)*100);
    nome=(char*)malloc(sizeof(char)*100);
    ingles=(char*)malloc(sizeof(char)*100);
    ing *proLista = NULL;
    fptr = fopen(caminho, "r");
    if( fptr == NULL ){
        printf("Erro na abertura de arquivo!\n");
        return;        
    }else{
        printf("Arquivo aberto com sucesso!\n");
    }
    fscanf(fptr, "%s", palavras);
    while (!feof(fptr)){
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
                    insere(&(*uni)->arvore, nome, ingles, promove, &proLista, NULL);
                    proLista = NULL;
                    j = 0;
                }
            }
            nome[j] = '\0';
            insere(&(*uni)->arvore, nome, ingles, promove, &proLista, NULL);
            proLista = NULL;
        }
        fscanf(fptr, "%s", palavras);
    }
}

void copiaLista(ing **lista, ing *lis){
    if(lis != NULL){
        insereLista(lista, lis->eng);
        copiaLista(lista, lis->prox);
    }
}

void menorInfoDir(arv *raiz, arv **no, arv **paiNo){
    if(raiz->esq != NULL){
        *paiNo = raiz;
        menorInfoDir(raiz->esq, no, paiNo);
    }else{
        *no = raiz;
    }
}

void maiorInfoEsq(arv *raiz, arv **no, arv **paiNo){
    if(raiz->dir != NULL){
        *paiNo = raiz;
        maiorInfoEsq(raiz->dir, no, paiNo);
    }else{
        *no = raiz;
    }
}

int remove23(arv **raiz, char *pal, arv **pai){
    int removeu = 0;
    arv *no = NULL, *no1, *paiNo = NULL, *paiNo1 = NULL, **aux;
    aux=(arv**) malloc(sizeof(arv*));
    no1 = (arv*) malloc(sizeof(arv));
    if(*raiz != NULL){
        if(efolha(*raiz)){
            if((*raiz)->qtd == 2){
                if(strcmp(pal, (*raiz)->info2) == 0){
                    strcpy((*raiz)->info2, " ");
                    apagarLista(&(*raiz)->l2);
                    (*raiz)->qtd = 1;
                    removeu = 1;
                }else if(strcmp(pal, (*raiz)->info1) == 0){
                    strcpy((*raiz)->info1, (*raiz)->info2);
                    apagarLista(&(*raiz)->l1);
                    copiaLista(&(*raiz)->l1, (*raiz)->l2);
                    apagarLista(&(*raiz)->l2);
                    strcpy((*raiz)->info2, " ");
                    (*raiz)->qtd = 1;
                    removeu = 1;
                }
            }else if(strcmp(pal, (*raiz)->info1) == 0){
                if(*pai == NULL){
                    free(*raiz);
                    *raiz = NULL;
                    removeu = 1;
                }else if(*raiz == (*pai)->esq){
                    strcpy((*raiz)->info1, (*pai)->info1);
                    apagarLista(&(*raiz)->l1);
                    copiaLista(&(*raiz)->l1, (*pai)->l1);
                    paiNo = *pai;
                    menorInfoDir((*pai)->cen, &no, &paiNo);
                    strcpy((*pai)->info1, no->info1);
                    apagarLista(&(*pai)->l1);
                    copiaLista(&(*pai)->l1, no->l1);
                    removeu = 1;
                    if(no->qtd == 2){
                        strcpy(no->info1,no->info2);
                        apagarLista(&no->l1);
                        copiaLista(&no->l1, no->l2);
                        strcpy(no->info2," ");
                        apagarLista(&no->l2);
                        no->qtd = 1;
                    }else{
                        if(paiNo->qtd == 1){
                            strcpy((*raiz)->info2, no->info1);
                            apagarLista(&(*raiz)->l2);
                            copiaLista(&(*raiz)->l2,no->l1);
                            (*raiz)->qtd = 2;
                            apagarLista(&no->l1);
                            free(no);
                            *pai = *raiz;
                            printf("aq\n");
                        }else{
                            strcpy(no->info1,paiNo->info2);
                            apagarLista(&no->l1);
                            copiaLista(&no->l1,paiNo->l2);
                            paiNo1 = paiNo;
                            menorInfoDir(paiNo->dir, &no1, &paiNo1);
                            strcpy(paiNo->info2,no1->info1);
                            apagarLista(&paiNo->l2);
                            copiaLista(&paiNo->l2,no1->l1);
                            if(no1->qtd == 2){
                                strcpy(no1->info1,no1->info2);
                                apagarLista(&no1->l1);
                                copiaLista(&no1->l1,no1->l2);
                                strcpy(no1->info2, " ");
                                apagarLista(&no1->l2);
                                no1->qtd = 1;
                            }else{
                                strcpy(no->info2,paiNo->info2);
                                apagarLista(&no->l2);
                                copiaLista(&no->l2,paiNo->l2);
                                no->qtd=2;
                                strcpy(paiNo->info2, " ");
                                apagarLista(&paiNo->l2);
                                paiNo->qtd = 1;
                                apagarLista(&no1->l1);
                                apagarLista(&no1->l2);
                                free(no1);
                                paiNo1->dir = NULL;
                            }
                        }
                    }
                }else if(*raiz == (*pai)->cen){
                    removeu = 1;
                    if((*pai)->qtd == 1){
                        if(((*pai)->esq)->qtd == 2){
                            strcpy((*raiz)->info1, (*pai)->info1);
                            apagarLista(&(*raiz)->l1);
                            copiaLista(&(*raiz)->l1, (*pai)->l1);
                            strcpy((*pai)->info1, ((*pai)->esq)->info2);
                            apagarLista(&(*pai)->l1);
                            copiaLista(&(*pai)->l1, ((*pai)->esq)->l2);
                            strcpy(((*pai)->esq)->info2, " ");
                            apagarLista(&((*pai)->esq)->l2);
                            ((*pai)->esq)->qtd = 1;
                        }else{
                            strcpy(((*pai)->esq)->info2, (*pai)->info1);
                            apagarLista(&((*pai)->esq)->l2);
                            copiaLista(&((*pai)->esq)->l2, (*pai)->l1);
                            apagarLista(&(*raiz)->l1);
                            apagarLista(&(*raiz)->l2);
                            free(*raiz);
                            ((*pai)->esq)->qtd = 2;
                            *aux = (*pai)->esq;
                            apagarLista(&(*pai)->l1);
                            apagarLista(&(*pai)->l1);
                            free(*pai);
                            *pai = *aux;
                        }
                    }else{
                        strcpy((*raiz)->info1, (*pai)->info2);
                        apagarLista(&(*raiz)->l1);
                        copiaLista(&(*raiz)->l1, (*pai)->l2);
                        paiNo = *pai;
                        menorInfoDir((*pai)->dir, &no, &paiNo);
                        strcpy((*pai)->info2, no->info1);
                        apagarLista(&(*pai)->l2);
                        copiaLista(&(*pai)->l2,no->l1);
                        if(no->qtd == 2){
                            strcpy(no->info1, no->info2);
                            apagarLista(&no->l1);
                            copiaLista(&no->l1, no->l2);
                            strcpy(no->info2, " ");
                            apagarLista(&no->l2);
                            no->qtd = 1;
                        }else{
                            (*raiz)->qtd = 2;
                            strcpy((*raiz)->info2, (*pai)->info2);
                            apagarLista(&(*raiz)->l2);
                            copiaLista(&(*raiz)->l2, (*pai)->l2);
                            strcpy((*pai)->info2, " ");
                            apagarLista(&(*pai)->l2);
                            (*pai)->qtd = 1;
                            free(no);
                            (*pai)->dir = NULL;
                        }
                    }
                }else{
                    removeu = 1;
                    paiNo = *pai;
                    maiorInfoEsq((*pai)->cen, &no, &paiNo);
                    if(no->qtd == 1){
                        strcpy(no->info2, (*pai)->info2);
                        apagarLista(&no->l2);
                        copiaLista(&no->l2, (*pai)->l2);
                        strcpy((*pai)->info2, " ");
                        apagarLista(&(*pai)->l2);
                        (*pai)->qtd = 1;
                        no->qtd = 2;
                        free(*raiz);
                        *raiz = NULL;
                    }else{
                        strcpy((*raiz)->info1, (*pai)->info2);
                        apagarLista(&(*raiz)->l1);
                        copiaLista(&(*raiz)->l1, (*pai)->l2);
                        strcpy((*pai)->info2, no->info2);
                        apagarLista(&(*pai)->l2);
                        copiaLista(&(*pai)->l2,no->l2);
                        strcpy(no->info2, " ");
                        apagarLista(&no->l2);
                        no->qtd = 1;
                    }
                }
            }
        }else{
            if(strcmp(pal, (*raiz)->info1)<0){
                removeu = remove23(&(*raiz)->esq, pal, raiz);
            }else if(strcmp(pal, (*raiz)->info1)==0){
                paiNo = *raiz;
                menorInfoDir((*raiz)->cen, &no, &paiNo);
                strcpy((*raiz)->info1,no->info1);
                apagarLista(&(*raiz)->l1);
                copiaLista(&(*raiz)->l1, no->l1);
                mostraInOrdem(*raiz);
                printf("\n\n");
                remove23(&(*raiz)->cen, (*raiz)->info1, raiz);
                printf("\n\n");
                mostraInOrdem(*raiz);
                removeu = 1;
            }else if((*raiz)->qtd == 1 || strcmp(pal, (*raiz)->info2)<0){
                removeu = remove23(&(*raiz)->cen, pal, raiz);
            }else if(pal == (*raiz)->info2){
                paiNo = *pai;
                menorInfoDir((*pai)->dir, &no, &paiNo);
                strcpy((*raiz)->info2,no->info1);
                apagarLista(&(*raiz)->l2);
                copiaLista(&(*raiz)->l2, no->l1);
                remove23(&(*raiz)->dir, (*raiz)->info2, raiz);
                removeu = 1;
            }else{
                removeu = remove23(&(*raiz)->dir, pal, raiz);
            }
        }
    }
    return removeu;
}

cap* procuraUnidade(cap *uni, char *p, int *certo){
    cap *aux=uni;
    for(uni; aux != NULL; aux=aux->prox){
        if(strcmp(aux->unidade, p) == 0){
            *certo = 1;
            break;
        }
    }
    return aux;
}

int procuraPalavra(arv *uni, char *p){
    int c = 0;
    if(uni != NULL){
        if(strcmp(p, uni->info1) < 0){
            c = procuraPalavra(uni->esq, p);
        }else if(strcmp(p, uni->info1) > 0 && strcmp(p, uni->info2) < 0){
            c = procuraPalavra(uni->cen, p);
        }else if(strcmp(p, uni->info1) == 0){
            c = 1;
        }else if(strcmp(p, uni->info2) > 0){
            c = procuraPalavra(uni->dir, p);
        }else if(strcmp(p, uni->info2) == 0){
            c = 1;
        }
    }
    return c;
}