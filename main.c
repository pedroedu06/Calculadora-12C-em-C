#include "expressoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void formatarExpressao(const char *entrada, char *saida);


void salvarResultadoEmArquivo(const char *infixa, const char *posfixa, float valor, float valorIfixo) {
    FILE *arquivo = fopen("resultado.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(arquivo, "Expressão infixa: %s\n", infixa);
    fprintf(arquivo, "Expressão pós-fixa: %s\n", posfixa);
    fprintf(arquivo, "Valor: %.2f\n", valor);
    fprintf(arquivo, "Valor (Infixa): %.2f\n", valorIfixo);

    fclose(arquivo);
    printf("Resultado salvo em 'resultado.txt'.\n");
}

int main() {
    char entradaBruta[512];
    char entradaFormatada[1024]; // maior para garantir espaço extra

    printf("Digite a expressão infixa:\n");
    fgets(entradaBruta, sizeof(entradaBruta), stdin);
    entradaBruta[strcspn(entradaBruta, "\n")] = '\0'; // remove o '\n'

    // Formata a expressão, adicionando espaços
    formatarExpressao(entradaBruta, entradaFormatada);

    // Gera a forma pós-fixa (a função deve retornar malloced string)
    char *posfixa = getFormaPosFixa(entradaFormatada);

    // Calcula valores nas duas formas
    float valorPosFixa = getValorPosFixa(posfixa);
    float valorInFixa = getValorInFixa(entradaFormatada);

    // Salva resultados em arquivo (supondo que a função já está implementada)
    salvarResultadoEmArquivo(entradaFormatada, posfixa, valorPosFixa, valorInFixa);

    free(posfixa); // libera memória alocada

    return 0;
}