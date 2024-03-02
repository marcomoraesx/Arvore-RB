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

int valida_rb(arvore raiz) {
    if (raiz == NULL) {
        return 1;
    }
    if (raiz->pai == NULL) {
        if (cor(raiz) != PRETO) {
            return 0;
        }
    }
    if (cor(raiz) != PRETO && cor(raiz) != VERMELHO) {
        return 0;
    }
    if (cor(raiz) == VERMELHO && (cor(raiz->esq) != PRETO || cor(raiz->dir) != PRETO)) {
        return 0;
    }
    if (((caminho_pretos(raiz->esq) + 1) != (caminho_pretos(raiz->dir) + 1)) && cor(raiz) == PRETO) {
        return 0;
    }
    if (valida_rb(raiz->esq) && valida_rb(raiz->dir)) {
        return 1;
    } else {
        return 0;
    }
}

int caminho_pretos(arvore raiz) {
    if (raiz == NULL) {
        return 1;
    } else if (cor(raiz) == PRETO) {
        int esq = caminho_pretos(raiz->esq);
        int dir = caminho_pretos(raiz->dir);
        return 1 + (esq > dir ? esq : dir);
    } else {
        int esq = caminho_pretos(raiz->esq);
        int dir = caminho_pretos(raiz->dir);
        return (esq > dir ? esq : dir);
    }
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
    //Buscar todas as sequências de elementos que tem a cor VERMELHO e corrigir
    while (cor(elemento->pai) == VERMELHO && cor(elemento) == VERMELHO) {
        //Recolorir
        if (cor(tio(elemento)) == VERMELHO) {
            tio(elemento)->cor = PRETO;
            elemento->pai->cor = PRETO;
            avo(elemento)->cor = VERMELHO;
            elemento = avo(elemento);
            continue;
        }
        //Rotação simples à direita
        if (eh_filho_esquerdo(elemento) && eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_direita(raiz, avo(elemento));
            elemento->pai->cor = PRETO;
            elemento->pai->dir->cor = VERMELHO;
            continue;
        }
        //Rotação simples à esquerda
        if (eh_filho_direito(elemento) && eh_filho_direito(elemento->pai)) {
            rotacao_simples_esquerda(raiz, avo(elemento));
            elemento->pai->cor = PRETO;
            elemento->pai->esq->cor = VERMELHO;
            continue;
        }
        //Rotação dupla à direita
        if (eh_filho_direito(elemento) && eh_filho_esquerdo(elemento->pai)) {
            rotacao_simples_esquerda(raiz, elemento->pai);
            rotacao_simples_direita(raiz, elemento->pai);
            elemento->cor = PRETO;
            elemento->dir->cor = VERMELHO;
            continue;
        }
        //Rotação dupla à esquerda
        if (eh_filho_esquerdo(elemento) && eh_filho_direito(elemento->pai)) {
            rotacao_simples_direita(raiz, elemento->pai);
            rotacao_simples_esquerda(raiz, elemento->pai);
            elemento->cor = PRETO;
            elemento->esq->cor = VERMELHO;
            continue;
        }
    }
    //Reajustar a cor da raiz para PRETO
    (*raiz)->cor = PRETO;
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
    return elemento->pai->pai;
}

void imprimir_elemento(arvore raiz) {
    if (raiz != NULL) {
        switch (raiz->cor) {
            case PRETO:
                printf("\x1b[33m[%d]\x1b[0m", raiz->dado);
                break;
            case VERMELHO:
                printf("\x1b[31m[%d]\x1b[0m", raiz->dado);
                break;
            case DUPLO_PRETO:
                printf("\x1b[32m[%d]\x1b[0m", raiz->dado);
                break;
        }
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

void rotacao_simples_esquerda(arvore *raiz, arvore pivo) {
    arvore u, t1;
    u = pivo->dir;
    t1 = u->esq;
    int posicao_pivo_dir = eh_filho_direito(pivo);
    pivo->dir = t1;
    if (t1 != NULL) {
        t1->pai = pivo;
    }
    u->esq = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;
    if (eh_raiz(u)) {
        *raiz = u;
    } else {
        if (posicao_pivo_dir) {
            u->pai->dir = u;
        } else {
            u->pai->esq = u;
        }
    }
}

void rotacao_simples_direita(arvore *raiz, arvore pivo) {
    arvore u, t1;
    u = pivo->esq;
    t1 = u->dir;
    int posicao_pivo_esq = eh_filho_esquerdo(pivo);
    pivo->esq = t1;
    if (t1 != NULL) {
        t1->pai = pivo;
    }
    u->dir = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;
    if (eh_raiz(u)) {
      *raiz = u;
    } else {
       if (posicao_pivo_esq) {
          u->pai->esq = u;
       } else {
          u->pai->dir = u;
       }
    }
}

void preorder(arvore raiz){
    if(raiz != NULL) {
        imprimir_elemento(raiz);
        preorder(raiz->esq);
        preorder(raiz->dir);
    }
}

void inorder(arvore raiz){
    if(raiz != NULL) {
        inorder(raiz->esq);
        imprimir_elemento(raiz);
        inorder(raiz->dir);
    }
}

void posorder(arvore raiz) {
    if(raiz != NULL) {
        posorder(raiz->esq);
        posorder(raiz->dir);
        imprimir_elemento(raiz);
    }
}

void reverso(arvore raiz){
    if(raiz != NULL) {
        reverso(raiz->dir);
        imprimir_elemento(raiz);
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

int maior_elemento(arvore raiz) {
	if(raiz == NULL)
        return -1;
	if(raiz->dir == NULL)
		return raiz->dado;
	else
		return maior_elemento(raiz->dir);
}

int menor_elemento(arvore raiz) {
	if(raiz == NULL)
        return -1;
	if(raiz->esq == NULL)
		return raiz->dado;
	else
		return maior_elemento(raiz->esq);
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
                return maior_elemento(raiz_atual->esq);
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

void remover(arvore *raiz, arvore temp, int valor) {
    arvore posicao;
    posicao = temp;
    while (posicao != NULL) {
        if (valor == posicao->dado) {
            if (posicao->esq != NULL && posicao->dir != NULL) {
                posicao->dado = maior_elemento(posicao->esq);
                remover(raiz, posicao->esq, posicao->dado);
                break;
            }
            //Possui apenas o filho direito
            if (posicao->esq == NULL && posicao->dir != NULL) {
                posicao->dir->cor = PRETO;
                posicao->dir->pai = posicao->pai;
                if (eh_raiz(posicao)) {
                    *raiz = posicao->dir;
                } else {
                    if (eh_filho_esquerdo(posicao)) {
                        posicao->pai->esq = posicao->dir;
                    } else {
                        posicao->pai->dir = posicao->dir;
                    }
                }
                free(posicao);
                break;
            }
            //Possui apenas o filho esquerdo
            if (posicao->esq != NULL && posicao->dir == NULL) {
                posicao->esq->cor = PRETO;
                posicao->esq->pai = posicao->pai;
                if (eh_raiz(posicao)) {
                    *raiz = posicao->esq;
                } else {
                    if (eh_filho_esquerdo(posicao)) {
                        posicao->pai->esq = posicao->esq;
                    } else {
                        posicao->pai->dir = posicao->esq;
                    }
                }
                free(posicao);
                break;
            }
            //Possui os dois filhos
            if (posicao->esq == NULL && posicao->dir == NULL) {
                if (eh_raiz(posicao)) {
                    *raiz = NULL;
                    free(posicao);
                    break;
                }
                if (posicao->cor == VERMELHO) {
                    if (eh_filho_esquerdo(posicao)) {
                        posicao->pai->esq = NULL;
                    } else {
                        posicao->pai->dir = NULL;
                    }
                    free(posicao);
                    break;
                } else {
                    no_null->cor = DUPLO_PRETO;
                    no_null->pai = posicao->pai;
                    if (eh_filho_esquerdo(posicao)) {
                        posicao->pai->esq = no_null;
                    } else {
                        posicao->pai->dir = no_null;
                    }
                    free(posicao);
                    reajustar(raiz, no_null);
                    break;
                }
            }
        }
        if (valor > posicao->dado) {
            posicao = posicao->dir;
        } else {
            posicao = posicao->esq;
        }
    }
}

void reajustar(arvore *raiz, arvore elemento) {
    //CASO 1 - Elemento é a raiz
    if (eh_raiz(elemento)) {
        elemento->cor = PRETO;
        if (elemento == no_null) {
            *raiz = NULL;
        }
        return;
    }
    //CASO 2 - Seu pai é PRETO, seu irmão é VERMELHO e seus sobrinhos são PRETOS
    if (cor(irmao(elemento)) == VERMELHO && cor(elemento->pai) == PRETO && cor(irmao(elemento)->esq) == PRETO && cor(irmao(elemento)->dir) == PRETO) {
        if (eh_filho_esquerdo(elemento)) {
            rotacao_simples_esquerda(raiz, elemento->pai);
        } else {
            rotacao_simples_direita(raiz, elemento->pai);
        }
        elemento->pai->cor = VERMELHO;
        avo(elemento)->cor = PRETO;
        reajustar(raiz, elemento);
        return;
    }
    //CASO 3 - Seu pai é PRETO, seu irmão é PRETO e seus sobrinhos são PRETOS
    if (cor(elemento->pai) == PRETO && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == PRETO && cor(irmao(elemento)->dir) == PRETO) {
        elemento->pai->cor = DUPLO_PRETO;
        irmao(elemento)->cor = VERMELHO;
        retira_duplo_preto(raiz, elemento);
        reajustar(raiz, elemento->pai);
        return;
    }
    //CASO 4 - Seu pai é VERMELHO, seu irmão é PRETO e seus sobrinhos são PRETOS
    if (cor(elemento->pai) == VERMELHO && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == PRETO && cor(irmao(elemento)->dir) == PRETO) {
        elemento->pai->cor = PRETO;
        irmao(elemento)->cor = VERMELHO;
        retira_duplo_preto(raiz, elemento);
        return;
    }
    //CASO 5a - Independe da cor do pai, seu irmão é PRETO, seu sobrinho esquerdo é VERMELHO e seu sobrinho direito é PRETO
    if (eh_filho_esquerdo(elemento) && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == VERMELHO && cor(irmao(elemento)->dir) == PRETO) {
        rotacao_simples_direita(raiz, irmao(elemento));
        irmao(elemento)->cor = PRETO;
        irmao(elemento)->dir->cor = VERMELHO;
        reajustar(raiz, elemento);
        return;
    }
    //CASO 5b - Independe da cor do pai, seu irmão é PRETO, seu sobrinho direito é VERMELHO e seu sobrinho esquerdo é PRETO
    if (eh_filho_direito(elemento) && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == PRETO && cor(irmao(elemento)->dir) == VERMELHO) {
        rotacao_simples_esquerda(raiz, irmao(elemento));
        irmao(elemento)->cor = PRETO;
        irmao(elemento)->esq->cor = VERMELHO;
        reajustar(raiz, elemento);
        return;
    }
    //CASO 6a - Independe da cor do pai, seu irmão é PRETO, independe da cor do sobrinho esquerdo e seu sobrinho direito é VERMELHO
    if (eh_filho_esquerdo(elemento) && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == VERMELHO) {
        rotacao_simples_esquerda(raiz, elemento->pai);
        avo(elemento)->cor = elemento->pai->cor;
        elemento->pai->cor = PRETO;
        irmao(elemento->pai)->cor = PRETO;
        retira_duplo_preto(raiz, elemento);
        return;
    }
    //CASO 6b - Independe da cor do pai, seu irmão é PRETO, independe da cor do sobrinho direito e seu sobrinho esquerdo é VERMELHO
    if (eh_filho_direito(elemento) && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == VERMELHO) {
        rotacao_simples_direita(raiz, elemento->pai);
        avo(elemento)->cor = elemento->pai->cor;
        elemento->pai->cor = PRETO;
        irmao(elemento->pai)->cor = PRETO;
        retira_duplo_preto(raiz, elemento);
        return;
    }
}

void retira_duplo_preto(arvore *raiz, arvore elemento) {
    if (elemento == no_null) {
        if (eh_filho_esquerdo(elemento)) {
            elemento->pai->esq = NULL;
        } else {
            elemento->pai->dir = NULL;
        }
    } else {
        elemento->cor = PRETO;
    }
}
