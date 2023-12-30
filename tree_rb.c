#include "tree_rb.h"
#include <stdlib.h>
#include <stdio.h>

arvore no_null;

void inicializar(arvore *raiz) {
    *raiz = NULL;
    no_null = (arvore) malloc(sizeof(struct no));
    no_null->cor = DUPLO_PRETO;
    no_null->dado = 0;
    no_null->esq = NULL;
    no_null->dir = NULL;
    no_null->pai = NULL;
}

void inserir (arvore *raiz, int valor){
    arvore posicao, pai, novo;
    posicao = *raiz;
    pai = NULL;

    while(posicao != NULL) {
        pai = posicao;
        if (valor > posicao->dado) {
            posicao = posicao->dir;
        } else {
            posicao = posicao->esq;
        }
    }

    novo = (arvore) malloc(sizeof(struct no));
    novo->dado = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = VERMELHO;

    if (eh_raiz(novo)) {
        *raiz = novo;
    } else {
        if (valor > pai->dado) {
            pai->dir = novo;
        } else {
            pai->esq = novo;
        }
    }
    ajustar(raiz, novo);
}

void ajustar (arvore *raiz, arvore elemento) {

}

enum cor cor(arvore elemento) {
    enum cor c;
    if (elemento == NULL) {
        return PRETO;
    } else {
        return elemento->cor;
    }
    return c;
}

int eh_raiz(arvore elemento) {
    return (elemento->pai == NULL);
}

int eh_filho_esquerdo(arvore elemento) {
    return (elemento->pai != NULL && elemento == elemento->pai->esq);
}

int eh_filho_direito (arvore elemento) {
    return (elemento->pai != NULL && elemento == elemento->pai->dir);
}

arvore irmao(arvore elemento) {
    if (eh_filho_direito(elemento)) {
        return elemento->pai->esq;
    } else {
        return elemento->pai->dir;
    }
}

arvore tio (arvore elemento) {
    return irmao(elemento->pai);
}

arvore avo (arvore elemento) {
    if (elemento->pai->pai != NULL) {
        return elemento->pai->pai;
    }
}

void imprimir_elemento(arvore raiz) {
    switch (raiz->cor) {
        case PRETO:
            printf("\x1b[30m[%d]\x1b[0m", raiz->dado);
            break;
        case VERMELHO:
            printf("\x1b[31m[%d]\x1b[0m", raiz->dado);
            break;
        case DUPLO_PRETO:
            printf("\x1b[32m[%d]\x1b[0m", raiz->dado);
            break;
    }
}

void imprimir (arvore raiz) {
    printf("(");
    if (raiz != NULL) {
        imprimir_elemento(raiz);
        imprimir(raiz->esq);
        imprimir(raiz->dir);
    }
    printf(")");
}

arvore rotacao_simples_esquerda(arvore pivo) {
    arvore u, t1, t2, t3;
    u = pivo->dir;
    t1 = pivo->esq;
    t2 = u->esq;
    t3 = u->dir;
    u->esq = pivo;
    pivo->dir = t2;
    return u;
}

arvore rotacao_simples_direita(arvore pivo) {
    arvore u, t1, t2, t3;
    u = pivo->esq;
    t1 = pivo->dir;
    t2 = u->esq;
    t3 = u->dir;
    u->dir = pivo;
    pivo->esq = t3;
    return u;
}

arvore rotacao_dupla_direita(arvore pivo) {
    pivo->esq = rotacao_simples_esquerda(pivo->esq);
    return rotacao_simples_direita(pivo);
}

arvore rotacao_dupla_esquerda(arvore pivo) {
    pivo->dir = rotacao_simples_direita(pivo->dir);
    return rotacao_simples_esquerda(pivo);
}

void preorder(arvore raiz){
    if(raiz != NULL) {
        printf("[%d]", raiz->dado);
        preorder(raiz->esq);
        preorder(raiz->dir);
    }
}

void inorder(arvore raiz){
    if(raiz != NULL) {
        inorder(raiz->esq);
        printf("[%d]", raiz->dado);
        inorder(raiz->dir);
    }
}

void posorder(arvore raiz) {
    if(raiz != NULL) {
        posorder(raiz->esq);
        posorder(raiz->dir);
        printf("[%d]", raiz->dado);
    }
}

void reverso(arvore raiz){
    if(raiz != NULL) {
        reverso(raiz->dir);
        printf("[%d]", raiz->dado);
        reverso(raiz->esq);
    }
}

int qtde_par(arvore raiz){
    int qtde = 0;
    if(raiz != NULL) {
        if (raiz->dado%2 == 0) {
            qtde += 1;
        }
        qtde += qtde_par(raiz->esq) + qtde_par(raiz->dir);
    }
    return qtde;
}

/*int antecessor(arvore raiz, int n){
    int ant = -1;
    if (raiz != NULL) {
        if (n == raiz->valor){
            ant = antecessor(raiz->esq, n);
        } else if (n < raiz->valor) {
            ant = antecessor(raiz->esq, n);
        } else {
            ant = raiz->valor;
            int result = antecessor(raiz->dir, n);
            if (result != -1) {
                ant = result;
            }
        }
    }
    return ant;
}*/

int maior_valor(arvore raiz) {
    int maior = raiz->dado ? raiz->dado : NULL;
    if (raiz != NULL && raiz->dir != NULL) {
        maior = maior_valor(raiz->dir);
    }
    return maior;
}

int antecessor(arvore raiz, int valor) {
    arvore candidato = NULL;
    arvore raiz_atual = raiz;
    while (raiz_atual != NULL) {
        if (valor < raiz_atual->dado) {
            raiz_atual = raiz_atual->esq;
        } else if (valor > raiz_atual->dado) {
            candidato = raiz_atual;
            raiz_atual = raiz_atual->dir;
        } else {
            if (raiz_atual->esq != NULL) {
                return maior_valor(raiz_atual->esq);
            } else {
                if (candidato != NULL) {
                    return candidato->dado;
                }
                break;
            }
        }
    }
    return -1;
}

int pai(arvore raiz, int n){
    int father = -1;
    if (raiz != NULL) {
        if (n < raiz->dado) {
            if (raiz->esq != NULL && raiz->esq->dado == n) {
                father = raiz->dado;
            } else {
                father = pai(raiz->esq, n);
            }
        }
        if (n > raiz->dado) {
            if (raiz->dir != NULL && raiz->dir->dado == n) {
                father = raiz->dado;
            } else  {
                father = pai(raiz->dir, n);
            }
        }
    }
    return father;
}

int soma_par(arvore raiz) {
    int soma = 0;
    if (raiz != NULL) {
        if (raiz->dado%2 == 0){
            soma += raiz->dado;
        }
        soma += soma_par(raiz->esq) + soma_par(raiz->dir);
    }
    return soma;
}

void dobro(arvore raiz) {
    if (raiz != NULL) {
        raiz->dado = raiz->dado * 2;
        dobro(raiz->esq);
        dobro(raiz->dir);
    }
}

int busca(arvore raiz, int valor) {
    int encontrado = 0;
    if (raiz != NULL) {
        if (raiz->dado == valor) {
            encontrado = 1;
        }
        if (valor < raiz->dado) {
            encontrado = busca(raiz->esq, valor);
        }
        if (valor > raiz->dado) {
            encontrado = busca(raiz->dir, valor);
        }
    }
    return encontrado;
}

void descendentes(arvore raiz, int valor) {
    if (raiz != NULL) {
        if (raiz->dado == valor) {
            if (raiz->esq != NULL) {
                inorder(raiz->esq);
                inorder(raiz->dir);
            } else {
                inorder(raiz->dir);
            }
        }
        if (raiz->dado > valor) {
            descendentes(raiz->esq, valor);
        }
        if (raiz->dado < valor) {
            descendentes(raiz->dir, valor);
        }
    }
}

int altura(arvore raiz) {
    int bigger;
    if (raiz != NULL) {
        int left = altura(raiz->esq);
        int right = altura(raiz->dir);
        if (left > right) {
            bigger = left;
        } else {
            bigger = right;
        }
        return 1 + bigger;
    }
    return 0;
}

int somatorio(arvore raiz){
    int soma = 0;
    if(raiz != NULL) {
        soma += raiz->dado + somatorio(raiz->esq) + somatorio(raiz->dir);
    }
    return soma;
}

void remover(arvore *raiz, int valor) {

}

int menor_valor(arvore raiz) {
    int menor = raiz->dado ? raiz->dado : NULL;
    if (raiz->esq != NULL) {
        menor = menor_valor(raiz->esq);
    }
    return menor;
}
