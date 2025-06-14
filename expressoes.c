#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressoes.h"

#define MAX_PILHA 100
#define TAM_TOKEN 512

// Pilha de strings
typedef struct {
    char itens[MAX_PILHA][TAM_TOKEN];
    int topo;
} PilhaString;

char *topo(PilhaString *p) {
    if (p->topo == -1) return NULL;
    return p->itens[p->topo];
}

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

char* desempilha(PilhaString *p) {
    if (!pilhaVazia(p)) {
        return p->itens[p->topo--];
    }
    return NULL;
}

char topoChar(PilhaString *p) {
    if (!pilhaVazia(p)) {
        return p->itens[p->topo][0];
    }
    return 0;
}

int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';    
}

//precedencia dos operadores
int precedencia(char op) {
    if (op == '^') return 3; // Exponenciação tem a maior precedência
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void espacamentoParenteses(char *destino, const char *origem) {
    while (*origem) {
        if (*origem == '(' || *origem == ')') {
            strcat(destino, " ");
            strncat(destino, origem, 1);
            strcat(destino, " ");
        } else {
            strncat(destino, origem, 1);
        }
        origem++;
    }
}

// forma infixa
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


//forma posfixa
char *getFormaPosFixa(char *Str) {
    static char saida[512];
    saida[0] = '\0';

    PilhaString operadores;
    inicializaPilha(&operadores);

    char expressaoEspacada[512] = "";
    espacamentoParenteses(expressaoEspacada, Str); 

    char *token = strtok(expressaoEspacada, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            strcat(saida, token);
            strcat(saida, " ");
        } else if (strcmp(token, "(") == 0) {
            empilha(&operadores, token);
        } else if (strcmp(token, ")") == 0) {
            while (!pilhaVazia(&operadores) && strcmp(topo(&operadores), "(") != 0) {
                strcat(saida, desempilha(&operadores));
                strcat(saida, " ");
            }
            if (!pilhaVazia(&operadores) && strcmp(topo(&operadores), "(") == 0) {
                desempilha(&operadores); // descarta o "("
            }
        } else if (ehOperador(token[0])) {
            while (!pilhaVazia(&operadores) &&
                   ehOperador(topo(&operadores)[0]) &&
                   precedencia(topo(&operadores)[0]) >= precedencia(token[0])) {
                strcat(saida, desempilha(&operadores));
                strcat(saida, " ");
            }
            empilha(&operadores, token);
        }
        token = strtok(NULL, " ");
    }

    while (!pilhaVazia(&operadores)) {
        strcat(saida, desempilha(&operadores));
        strcat(saida, " ");
    }

    return saida;
}


