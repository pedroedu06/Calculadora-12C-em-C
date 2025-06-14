#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expressoes.h"

#define MAX_PILHA 100
#define TAM_TOKEN 512

// Pilha de strings
typedef struct {
    char itens[MAX_PILHA][TAM_TOKEN];
    int topo;
} PilhaString;

void inicializaPilha(PilhaString *p) {
    p->topo = -1;
}

int pilhaVazia(PilhaString *p) {
    return p->topo == -1;
}

void empilha(PilhaString *p, const char *str) {
    if (p->topo < MAX_PILHA - 1) {
        strcpy(p->itens[++p->topo], str);
    }
}

char *desempilha(PilhaString *p) {
    if (!pilhaVazia(p)) {
        return p->itens[p->topo--];
    }
    return NULL;
}

int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

char *getFormaInFixa(char *Str) {
    PilhaString pilha;
    inicializaPilha(&pilha);

    char copia[512];
    strcpy(copia, Str);

    char *token = strtok(copia, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            empilha(&pilha, token);
        } else if (ehOperador(token[0]) && strlen(token) == 1) {
            char operando2[TAM_TOKEN], operando1[TAM_TOKEN];
            strcpy(operando2, desempilha(&pilha));
            strcpy(operando1, desempilha(&pilha));

            char novaExpressao[TAM_TOKEN];
            snprintf(novaExpressao, TAM_TOKEN, "(%s %s %s)", operando1, token, operando2);
            empilha(&pilha, novaExpressao);
        }
        token = strtok(NULL, " ");
    }

    // O topo da pilha agora é a expressão infixa final
    char *resultado = malloc(strlen(pilha.itens[pilha.topo]) + 1);
    if (resultado != NULL) {
        strcpy(resultado, pilha.itens[pilha.topo]);
    }
    return resultado;
}


