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

int ehOperadorStr(char *str) {
    return strcmp(str, "+") == 0 || strcmp(str, "-") == 0 ||
           strcmp(str, "*") == 0 || strcmp(str, "/") == 0 ||
           strcmp(str, "%") == 0 || strcmp(str, "^") == 0 ||
           strcmp(str, "log") == 0 || strcmp(str, "sqrt") == 0 ||
           strcmp(str, "sen") == 0 || strcmp(str, "cos") == 0 ||
           strcmp(str, "tan") == 0;
}

// Função para verificar se o caractere é um operador básico
int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

//precedencia dos operadores
int precedenciaStr(char *op) {
    if (strcmp(op, "log") == 0 || strcmp(op, "sqrt") == 0 ||
        strcmp(op, "sen") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tan") == 0)
        return 4;

    return 0;
}
int precedencia(char *op) {
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;

    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/" ) == 0 || strcmp(op, "%" ) == 0) return 2;
    if (strcmp(op, "+" ) == 0 || strcmp(op, "-" ) == 0) return 1;
    return 0;
}

int ehInfixa(char *expr) {
    // Pode melhorar essa função conforme seu caso, mas básico já ajuda
    return (strchr(expr, '(') != NULL) || (strchr(expr, ')') != NULL) || 
           (strchr(expr, '+') != NULL) || (strchr(expr, '-') != NULL) || 
           (strchr(expr, '*') != NULL) || (strchr(expr, '/') != NULL) || 
           (strchr(expr, '^') != NULL);
}

float calculaExpressao(char *expr) {
    if (ehInfixa(expr)) {
        char *posfixa = getFormaPosFixa(expr);
        float resultado = getValorPosFixa(posfixa);
        // Se getFormaPosFixa alocar memória, free(posfixa);
        return resultado;
    } else {
        return getValorPosFixa(expr);
    }
}

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



float getValorPosFixa(char *StrPosFixa) {
    PilhaString pilha;
    inicializaPilha(&pilha);

    char expressao[512];
    strcpy(expressao, StrPosFixa);

    char *token = strtok(expressao, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            empilha(&pilha, token); // número
        } else if (
            strcmp(token, "sqrt") == 0 ||
            strcmp(token, "sen") == 0 ||
            strcmp(token, "cos") == 0 ||
            strcmp(token, "tan") == 0 ||
            strcmp(token, "log") == 0) {

            char valorStr[64];
            strcpy(valorStr, desempilha(&pilha));
            float valor = atof(valorStr);
            if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tan") == 0) {
                valor = valor * (M_PI / 180);
            }

            float resultado;

            if (strcmp(token, "sqrt") == 0) resultado = sqrt(valor);
            else if (strcmp(token, "sen") == 0) resultado = sin(valor);
            else if (strcmp(token, "cos") == 0) resultado = cos(valor);
            else if (strcmp(token, "tan") == 0) resultado = tan(valor);
            else if (strcmp(token, "log") == 0) resultado = log10(valor);

            char resultadoStr[64];
            sprintf(resultadoStr, "%f", resultado);
            empilha(&pilha, resultadoStr);

        } else if (ehOperador(token[0]) && strlen(token) == 1) {
            char bStr[64], aStr[64];
            strcpy(bStr, desempilha(&pilha));
            strcpy(aStr, desempilha(&pilha));

            float a = atof(aStr);
            float b = atof(bStr);
            float resultado;

            switch (token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': resultado = a / b; break;
                case '%': resultado = (int)a % (int)b; break;
                case '^': resultado = pow(a, b); break;
                default: resultado = 0; break;
            }

            char resultadoStr[64];
            sprintf(resultadoStr, "%f", resultado);
            empilha(&pilha, resultadoStr);
        }

        token = strtok(NULL, " ");
    }

    if (!pilhaVazia(&pilha)) {
        return atof(desempilha(&pilha));
    }
    return 0;
}

float getValorInFixa(char *StrInFixa) {
    char *posfixa = getFormaPosFixa(StrInFixa);
    float resultado = getValorPosFixa(posfixa);

    free(posfixa);
    return resultado;
}


