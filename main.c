#include "expressoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void formatarExpressao(const char *entrada, char *saida);


void salvarResultadoEmArquivo(const char *infixa, const char *posfixa, float valor) {
    FILE *arquivo = fopen("resultado.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(arquivo, "Expressão infixa: %s\n", infixa);
    fprintf(arquivo, "Expressão pós-fixa: %s\n", posfixa);
    fprintf(arquivo, "Valor: %.2f\n", valor);

    fclose(arquivo);
    printf("Resultado salvo em 'resultado.txt'.\n");
}

int main() {
    char entrada[512];

    printf("Digite a expressao pos-fixa:\n");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0'; // remove '\n'

    float valor = getValorPosFixa(entrada);
    char *infixa = getFormaInFixa(entrada);

    salvarResultadoEmArquivo(infixa, entrada, valor);

    free(infixa);
    return 0;
}