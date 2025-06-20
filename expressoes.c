#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

int precedencia(char *op) {
    if (op == NULL) return 0;
    char c = op[0];
    switch (c) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

int ehFuncao(char *token) {
    return strcmp(token, "sqrt") == 0 || strcmp(token, "sen") == 0 ||
           strcmp(token, "cos") == 0 || strcmp(token, "tan") == 0 ||
           strcmp(token, "log") == 0;
}

// Formata a expressão de entrada, separando números, operadores e funções
// Exemplo: "3+5*sen(30)" -> "3 + 5
void formatarExpressao(const char *entradaBruta, char *saidaFormatada) {
    saidaFormatada[0] = '\0';  // Zera a saída

    char buffer[64];
    int i = 0, j = 0;

    while (entradaBruta[i] != '\0') {
        // Se for letra (parte de função)
        if (isalpha(entradaBruta[i])) {
            j = 0;
            while (isalpha(entradaBruta[i])) {
                buffer[j++] = entradaBruta[i++];
            }
            buffer[j] = '\0';
            strcat(saidaFormatada, buffer);
            strcat(saidaFormatada, " ");
        }
        // Se for número (inclusive com ponto decimal)
        else if (isdigit(entradaBruta[i]) || entradaBruta[i] == '.') {
            j = 0;
            while (isdigit(entradaBruta[i]) || entradaBruta[i] == '.') {
                buffer[j++] = entradaBruta[i++];
            }
            buffer[j] = '\0';
            strcat(saidaFormatada, buffer);
            strcat(saidaFormatada, " ");
        }
        // Se for operador ou parêntese
        else if (strchr("()+-*/%^", entradaBruta[i])) {
            buffer[0] = entradaBruta[i++];
            buffer[1] = '\0';
            strcat(saidaFormatada, buffer);
            strcat(saidaFormatada, " ");
        }
        // Se for espaço ou caractere desconhecido, pula
        else {
            i++;
        }
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
            char *op2 = desempilha(&pilha);
            char *op1 = desempilha(&pilha);

            char nova[256];
            snprintf(nova, sizeof(nova), "(%s %s %s)", op1, token, op2);
            empilha(&pilha, nova);
        } else if (ehFuncao(token)) {
            char *arg = desempilha(&pilha);
            char nova[256];
            snprintf(nova, sizeof(nova), "%s(%s)", token, arg);
            empilha(&pilha, nova);
        }
        token = strtok(NULL, " ");
    }

    char *res = malloc(strlen(pilha.itens[pilha.topo]) + 1);
    if (res) strcpy(res, pilha.itens[pilha.topo]);
    return res;
}



//forma posfixa
char *getFormaPosFixa(char *Str) {
        static char saida[512];
    saida[0] = '\0';

    PilhaString operadores;
    inicializaPilha(&operadores);

    char expressao[512];
    strcpy(expressao, Str);

    char *token = strtok(expressao, " ");
    while (token != NULL) {

        // Se for número
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            strcat(saida, token);
            strcat(saida, " ");
        }
        // Se for função (operador unário)
        else if (
            strcmp(token, "sqrt") == 0 ||
            strcmp(token, "sen") == 0 ||
            strcmp(token, "cos") == 0 ||
            strcmp(token, "tan") == 0 ||
            strcmp(token, "log") == 0
        ) {
            empilha(&operadores, token);
        }
        // Parêntese esquerdo
        else if (strcmp(token, "(") == 0) {
            empilha(&operadores, token);
        }
        // Parêntese direito
        else if (strcmp(token, ")") == 0) {
            while (!pilhaVazia(&operadores) && strcmp(topo(&operadores), "(") != 0) {
                char *op = desempilha(&operadores);
                strcat(saida, op);
                strcat(saida, " ");
            }
            if (!pilhaVazia(&operadores) && strcmp(topo(&operadores), "(") == 0) {
                desempilha(&operadores); // remove o "("
            }

            // Se depois do ")" tiver função no topo, ela deve ser adicionada à saída
            if (!pilhaVazia(&operadores)) {
                char *topoOp = topo(&operadores);
                if (
                    strcmp(topoOp, "sqrt") == 0 ||
                    strcmp(topoOp, "sen") == 0 ||
                    strcmp(topoOp, "cos") == 0 ||
                    strcmp(topoOp, "tan") == 0 ||
                    strcmp(topoOp, "log") == 0
                ) {
                    strcat(saida, desempilha(&operadores));
                    strcat(saida, " ");
                }
            }
        }
        // Operadores binários
        else if (ehOperador(token[0]) && strlen(token) == 1) {
            while (!pilhaVazia(&operadores) && strcmp(topo(&operadores), "(") != 0 &&
                   precedencia(topo(&operadores)) >= precedencia(token)) {
                strcat(saida, desempilha(&operadores));
                strcat(saida, " ");
            }
            empilha(&operadores, token);
        }

        token = strtok(NULL, " ");
    }

    // Desempilha o que sobrou
    while (!pilhaVazia(&operadores)) {
        strcat(saida, desempilha(&operadores));
        strcat(saida, " ");
    }

    return saida;
}


// Calcula o valor da expressão na forma pos-fixa
// Exemplo: "3 12 4 + *" -> 3 * (12 + 4) = 3 * 16 = 48
float getValorPosFixa(char *StrPosFixa) {
    PilhaString pilha;
    inicializaPilha(&pilha);

    char expressao[512];
    strcpy(expressao, StrPosFixa);

    char *token = strtok(expressao, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            empilha(&pilha, token);
        } else if (ehOperador(token[0]) && strlen(token) == 1) {
            char *bStr = desempilha(&pilha);
            char *aStr = desempilha(&pilha);
            float a = atof(aStr), b = atof(bStr), resultado;

            switch (token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': resultado = a / b; break;
                case '%': resultado = (int)a % (int)b; break;
                case '^': resultado = pow(a, b); break;
                default: resultado = 0;
            }

            char resultadoStr[64];
            sprintf(resultadoStr, "%f", resultado);
            empilha(&pilha, resultadoStr);
        } else if (ehFuncao(token)) {
            char *valorStr = desempilha(&pilha);
            float valor = atof(valorStr);
            float resultado;

            if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tan") == 0) {
                valor = valor * M_PI / 180.0;
            }

            if (strcmp(token, "sqrt") == 0) resultado = sqrt(valor);
            else if (strcmp(token, "sen") == 0) resultado = sin(valor);
            else if (strcmp(token, "cos") == 0) resultado = cos(valor);
            else if (strcmp(token, "tan") == 0) resultado = tan(valor);
            else if (strcmp(token, "log") == 0) resultado = log10(valor);

            char resultadoStr[64];
            sprintf(resultadoStr, "%f", resultado);
            empilha(&pilha, resultadoStr);
        }
        token = strtok(NULL, " ");
    }

    return atof(desempilha(&pilha));
}
// Calcula o valor da expressão na forma infixa
// Exemplo: "3 * (12 + 4)" -> 3 * 16 = 48
float getValorInFixa(char *StrInFixa) {
    char *posfixa = getFormaPosFixa(StrInFixa);
    float resultado = getValorPosFixa(posfixa);

    free(posfixa);
    return resultado;
}


