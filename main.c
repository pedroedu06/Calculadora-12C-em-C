#include "expressoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char posfixa[] = "3 12 4 + *";
    char *infixa = getFormaInFixa(posfixa);
    printf("Expressão infixa: %s\n", infixa); 
    free(infixa);
    return 0;
}