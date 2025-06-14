#include "expressoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void salvarResultadoEmArquivo(const char *infixa, const char *posfixa) {
    FILE *arquivo = fopen("resultado.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(arquivo, "Expressão infixa: %s\n", infixa);
    fprintf(arquivo, "Expressão pós-fixa: %s\n", posfixa);
    //fprintf(arquivo, "Valor: %.2f\n", valor);

    fclose(arquivo);
    printf("Resultado salvo em 'resultado.txt'.\n");
}

int main() {
 char entrada[512];

    printf("Digite a expressão infixa (com espaços entre os tokens):\n");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0'; // remove o '\n'

    char *posfixa = getFormaPosFixa(entrada);
    //float valor = getValorInFixa(entrada);

    salvarResultadoEmArquivo(entrada, posfixa);

    free(posfixa); // libera memória alocada

    return 0;
}