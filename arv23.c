#include<stdio.h>
#include<stdlib.h>

struct arv{
    int info1,info2;
    struct arv *esq,*cen,*dir;
    int qtd;
};

typedef struct arv arv;

arv *criaNo(int info,arv *esq,arv *cen,arv *dir);
arv *quebraNo(arv **raiz,int info,int *promove,arv *sub);
arv *adcNO(arv *no,int info,arv *filho);
int efolha(arv *no);
arv *insere(arv **raiz,int info,int *promove,arv *pai);
int menorInfoDir(arv *raiz, arv **no, arv **paiNo);
int maiorInfoEsq(arv *raiz, arv **no, arv **paiNo);
int remove23(arv **raiz, int valor, arv **pai);
int menu();
void mostraPreOrdem(arv *raiz);

void main(){
    arv *raiz = NULL, *no;
    int choice, n, *promove;
    do{
        choice = menu();
        switch (choice){
            case 0: 
                break;
            case 1:
                printf("Digite o valor para inserir:\n");
                scanf("%d", &n);
                insere(&raiz, n, promove, NULL);
                break;
            case 2:
                mostraPreOrdem(raiz);
                break;
            case 3:
                printf("Digite o valor para remover:\n");
                scanf("%d", &n);
                arv **p=(arv**)malloc(sizeof(arv*));
                *p=NULL;
                remove23(&raiz, n, p);
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

arv *criaNo(int info,arv *esq,arv *cen,arv *dir){
    arv *no=(arv*)malloc(sizeof(arv));
    no->info1=info;
    no->qtd=1;
    no->esq=esq;
    no->cen=cen;
    no->dir=dir;
    return no;
}

arv *quebraNo(arv **raiz,int info,int *promove,arv *sub){
    arv *aux;
    if(info<(*raiz)->info1){
        *promove=(*raiz)->info1;
        (*raiz)->info1=info;
        (*raiz)->qtd-=1;
        aux=criaNo((*raiz)->info2,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->cen=sub;
        (*raiz)->dir=NULL;
    }else if(info<=(*raiz)->info2){
        *promove=info;
        (*raiz)->qtd-=1;
        aux=criaNo((*raiz)->info2,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->cen=sub;
        (*raiz)->dir=NULL;
    }else{
        *promove=(*raiz)->info2;
        (*raiz)->qtd-=1;
        aux=criaNo(info,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->cen=sub;
        (*raiz)->dir=NULL;
    }
    return aux;
}

arv *adcNO(arv *no,int info,arv *filho){
    if(no->info1<info){
        no->info2=info;
        no->dir=filho;
    }else{
        no->info2=no->info1;
        no->dir=no->cen;
        no->info1=info;
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

arv *insere(arv **raiz,int info,int *promove,arv *pai){
    arv *aux;
    if(*raiz == NULL){
        *raiz=criaNo(info,NULL,NULL,NULL);
        aux=NULL;
    }else if(efolha(*raiz)){
        if((*raiz)->qtd == 1){
            *raiz=adcNO(*raiz,info,NULL);
            aux=NULL;
        }else{
            aux=quebraNo(raiz,info,promove,NULL);
        }
    }else{
        if(info<(*raiz)->info1){
            aux=insere(&((*raiz)->esq),info,promove,*raiz);
        }else if((*raiz)->qtd==1 || info<(*raiz)->info2){
            aux=insere(&((*raiz)->cen),info,promove,*raiz);
        }else{
            aux=insere(&((*raiz)->dir),info,promove,*raiz);
        }
    }
    if(aux!=NULL){
        if(pai==NULL){
            *raiz=criaNo(*promove,*raiz,aux,NULL);
            aux=NULL;
        }else if(pai->qtd==1){
            pai = adcNO(pai,*promove,aux);
            aux=NULL;
        }else{
            aux=quebraNo(&pai,*promove,promove,aux);
        }
    }
    return aux;
}

int menorInfoDir(arv *raiz, arv **no, arv **paiNo){
    int info;
    if(raiz->esq != NULL){
        *paiNo = raiz;
        info = menorInfoDir(raiz->esq, no, paiNo);
    }else{
        *no = raiz;
        info = raiz->info1;
    }
    return info;
}

int maiorInfoEsq(arv *raiz, arv **no, arv **paiNo){
    int info;
    if(raiz->dir != NULL){
        *paiNo = raiz;
        info = maiorInfoEsq(raiz->dir, no, paiNo);
    }else{
        *no = raiz;
        if(raiz->qtd == 2){
            info = raiz->info2;
        }else{
            info = raiz->info1;
        }
        
    }
    return info;
}

int remove23(arv **raiz, int valor, arv **pai){
    int removeu = 0;
    arv *no = NULL, *no1, *paiNo = NULL, *paiNo1 = NULL, *aux;
    no1 = (arv*) malloc(sizeof(arv));
    if(*raiz != NULL){
        if(efolha(*raiz)){
            if((*raiz)->qtd == 2){
                if(valor == (*raiz)->info2){
                    (*raiz)->info2 = 0;
                    (*raiz)->qtd = 1;
                    removeu = 1;
                }else if(valor == (*raiz)->info1){
                    (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->info2 = 0;
                    (*raiz)->qtd = 1;
                    removeu = 1;
                }
            }else if(valor == (*raiz)->info1){
                if(*pai == NULL){
                    free(*raiz);
                    *raiz = NULL;
                    removeu = 1;
                }else if(*raiz == (*pai)->esq){
                    (*raiz)->info1 = (*pai)->info1;
                    paiNo = *pai;
                    (*pai)->info1 = menorInfoDir((*pai)->cen, &no, &paiNo);
                    removeu = 1;
                    if(no->qtd == 2){
                        no->info1 = no->info2;
                        no->info2 = 0;
                        no->qtd = 1;
                    }else{
                        if(paiNo->qtd == 1){
                            (*raiz)->info2 = no->info1;
                            (*raiz)->qtd = 2;
                            free(no);
                            (*pai) = *raiz;
                        }else{
                            no->info1 = paiNo->info2;
                            paiNo1 = paiNo;
                            paiNo->info2 = menorInfoDir(paiNo->dir, &no1, &paiNo1);
                            if(no1->qtd == 2){
                                no1->info1 = no1->info2;
                                no1->qtd = 1;
                            }else{
                                no->info2 = paiNo->info2;
                                no->qtd=2;
                                paiNo->info2 = 0;
                                paiNo->qtd = 1;

                                //free(no1);
                                //no1 = NULL;
                                free(paiNo1->dir);
                                paiNo1->dir = NULL;
                            }
                        }
                    }
                }else if(*raiz == (*pai)->cen){
                    removeu = 1;
                    if((*pai)->qtd == 1){
                        if(((*pai)->esq)->qtd == 2){
                            (*raiz)->info1 = (*pai)->info1;
                            (*pai)->info1 = ((*pai)->esq)->info2;
                            ((*pai)->esq)->info2 = 0;
                            ((*pai)->esq)->qtd = 1;
                        }else{
                            ((*pai)->esq)->info2 = (*pai)->info1;
                            free(*raiz);
                            ((*pai)->esq)->qtd = 2;
                            aux = (*pai)->esq;
                            free(*pai);
                            *pai = aux;
                        }
                    }else{
                        (*raiz)->info1 = (*pai)->info2;
                        paiNo = *pai;
                        (*pai)->info2 = menorInfoDir((*pai)->dir, &no, &paiNo);
                        if(no->qtd == 2){
                            no->info1 = no->info2;
                            no->info2 = 0;
                            no->qtd = 1;
                        }else{
                            (*raiz)->qtd = 2;
                            (*raiz)->info2 = (*pai)->info2;
                            (*pai)->info2 = 0;
                            (*pai)->qtd = 1;
                            free(no);
                            (*pai)->dir = NULL;
                        }
                    }
                }else{
                    removeu = 1;
                    paiNo = *pai;
                    valor = maiorInfoEsq((*pai)->cen, &no, &paiNo);
                    if(no->qtd == 1){
                        no->info2 = (*pai)->info2;
                        (*pai)->info2 = 0;
                        (*pai)->qtd = 1;
                        no->qtd = 2;
                        free(*raiz);
                        *raiz = NULL;
                    }else{
                        (*raiz)->info1 = (*pai)->info2;
                        (*pai)->info2 = valor;
                        no->info2 = 0;
                        no->qtd = 1;
                    }
                }
            }
        }else{
            if(valor < (*raiz)->info1){
                removeu = remove23(&(*raiz)->esq, valor, raiz);
            }else if(valor == (*raiz)->info1){
                paiNo = *raiz;
                (*raiz)->info1 = menorInfoDir((*raiz)->cen, &no, &paiNo);
                remove23(&(*raiz)->cen, (*raiz)->info1, raiz);
                removeu = 1;
            }else if((*raiz)->qtd == 1 || valor < (*raiz)->info2){
                removeu = remove23(&(*raiz)->cen, valor, raiz);
            }else if(valor == (*raiz)->info2){
                paiNo = *pai;
                (*raiz)->info2 = menorInfoDir((*pai)->dir, &no, &paiNo);
                remove23(&(*raiz)->dir, (*raiz)->info2, raiz);
                removeu = 1;
            }else{
                removeu = remove23(&(*raiz)->dir, valor, raiz);
            }
        }
    }
    return removeu;
}

void mostraPreOrdem(arv *raiz){
    if(raiz != NULL){
        if(raiz->qtd == 2){
            printf("%d | %d", raiz->info1, raiz->info2);
        }else{
            printf("%d | ", raiz->info1);
        }
        printf("\n");
        mostraPreOrdem(raiz->esq);
        mostraPreOrdem(raiz->cen);
        mostraPreOrdem(raiz->dir);
    }
}


