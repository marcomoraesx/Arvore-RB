#ifndef TREE_RB_H
#define TREE_RB_H

enum cor { VERMELHO, PRETO, DUPLO_PRETO };

typedef int tipo_dado;

typedef struct no {
	tipo_dado dado;
	enum cor cor;
	struct no *esq, *dir, *pai;
} no;

typedef no * arvore;

void inicializar(arvore *raiz);
void inserir (arvore *raiz, int valor);
void remover (arvore *raiz, int valor);

int maior(int a, int b);
void imprimir_elemento(arvore raiz);
void imprimir(arvore raiz);

void ajustar(arvore *raiz, arvore elemento);
void reajustar(arvore *raiz, arvore elemento);
void rotacao_simples_esquerda(arvore *raiz, arvore pivo);
void rotacao_simples_direita(arvore *raiz, arvore pivo);

void preorder(arvore raiz);
void inorder(arvore raiz);
void posorder(arvore raiz);
void reverso(arvore raiz);

enum cor cor(arvore elemento);
int eh_raiz(arvore elemento);
int eh_filho_esquerdo(arvore elemento);
arvore irmao(arvore elemento);
arvore tio(arvore elemento);
arvore avo(arvore elemento);
void retira_duplo_preto(arvore *raiz, arvore elemento);

int qtde_par(arvore raiz);
int antecessor(arvore raiz, int n);
int pai(arvore raiz, int n);
int soma_par(arvore raiz);
void dobro(arvore raiz);
int busca(arvore raiz, int valor);
void descendentes(arvore raiz, int valor);
int altura(arvore raiz);
int menor_valor(arvore raiz);
int maior_valor(arvore raiz);

int somatorio(arvore raiz);
void mesclar (arvore raiz1, arvore raiz2);
void ascendentes(arvore raiz, int valor);

#endif
