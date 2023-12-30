#include <stdio.h>
#include <stdlib.h>
#include "tree_rb.h"

int main(int argc, char *argv) {
    arvore a;
    inicializar(&a);
    int valor;
    int opc = 0;

    do {
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                scanf("%d", &valor);
                inserir(&a, valor);
                preorder(a);
                printf("\n");
                break;
            case 2:
                preorder(a);
                printf("\n");
                break;
            case 3:
                inorder(a);
                printf("\n");
                break;
            case 4:
                posorder(a);
                printf("\n");
                break;
            case 5:
                reverso(a);
                printf("\n");
                break;
            case 6:
                printf("%d\n", qtde_par(a));
                break;
            case 7:
                scanf("%d", &valor);
                printf("%d\n", antecessor(a, valor));
                break;
            case 8:
                scanf("%d", &valor);
                printf("%d\n", pai(a, valor));
                break;
            case 9:
                scanf("%d", &valor);
                remover(a, valor);
                preorder(a);
                printf("\n");
                break;
            case 10:
                printf("%d\n", soma_par(a));
                break;
            case 11:
                dobro(a);
                preorder(a);
                printf("\n");
                break;
            case 12:
                scanf("%d", &valor);
                printf("%d\n", busca(a, valor));
                break;
            case 15:
                scanf("%d", &valor);
                descendentes(a, valor);
                printf("\n");
                break;
            case 16:
                printf("%d\n", altura(a));
                break;
            case 99:
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opc != 99);

    return 0;
}
