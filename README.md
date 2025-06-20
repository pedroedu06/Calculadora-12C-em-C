# 1 - Introdução
Este projeto é uma implementação em linguagem C inspirada na calculadora HP 12C, 
muito conhecida por seu uso no setor financeiro e por operar com a notação pós-fixa (RPN - Reverse Polish Notation). 
A calculadora foi desenvolvida como exercício acadêmico com foco em manipulação de expressões matemáticas, 
uso de pilhas (TADs) e aplicação de funções matemáticas como seno, cosseno, logaritmo, raiz quadrada, exponenciação, entre outras.
Esse projeto e voltado para minha formacao academica.


# 2 - Implementações

- Implementação da biblioteca ctype.h, para verificar digito de caracter na entrada, usando tokens.
- Implementação dos operadores sqrt, cos, sin, tan e log da biblioteca math.h.
- Formatacao da entrada da operação.

  
**2.1 uso da TDA**
- Estrutura da pilha implementada com a struct PilhaString, que armazena uma sequência de strings (tokens da expressão).
- Inicialização da pilha.
- Operações de empilhar e desempilhar elementos.
- Verificação do elemento no topo da pilha.

  
# 3 Teste

**IMPORTANTE !**
A entrada das operacoes deve ser feita no modo posfixo.
Todos os resultados sai em numero float, e sao mostrados no resultado.txt.

Fiz 8 testes de diferentes tipos.

## Aplicando testes

**3.1 - Teste 01**: Entrada `3 4 + 5 *` Saidas: infixo:`( 3 + 4 ) * 5`, pos-fixa:`3 4 + 5 *`, Resultado:`35.00`;</br>
**3.2 - Teste 02**: Entrada `7 2 * 4 +` Saidas: infixo:`7 * 2 + 4`, pos-fixa:`7 2 * 4 +`, Resultado:`18.00`;</br>
**3.3 -Teste 03**: Entrada `8 5 2 4 + * +` Saidas: infixo:`8 + ( 5 * ( 2 + 4 ) ) `, pos-fixa:`8 5 2 4 + * +`, Resultado:`38.00`;</br>
**3.4 -Teste 04**: Entrada `6 2 / 3 + 4 *` Saidas: infixo:`( 6 / 2 + 3 ) * 4 `, pos-fixa:`6 2 / 3 + 4 *`, Resultado:`24.00`;</br>
**3.5 -Teste 05**: Entrada `9 5 2 8 4 * + * +` Saidas: infixo:`9 + ( 5 * ( 2 + 8 * 4 ) )`, pos-fixa:`9 5 2 8 4 * + * +`, Resultado:`109`</br>
**3.6 -Teste 06**: Entrada `10 log 3 ^ 2 +` Saidas: infixo:`( log 10 ) ^ 3 + 2 `, pos-fixa:`10 log 3 ^ 2 + `, Resultado:`3.00`;</br>
**3.7 -Teste 07**: Entrada `45 60 + 30 cos *` Saidas: infixo:`( 45 + 60 ) * cos ( 30 )`, pos-fixa:`45 60 + 30 cos *`, Resultado:`90.93`;</br>
**3.8 -Teste 08**: Entrada `45 sen 2 ^ 0.5 +` Saidas: infixo:`sen ( 45 ) ^ 2 + 0.5 `, pos-fixa:`45 sen 2 ^ 0.5 +`, Resultado:`1.00`;</br>

# 4 Conclusão

Para mim esse trabalho foi bem dificil tive que procurar bastante coisa, porem foi um trabalho com muito aprendizado. 

**Referencias**

https://www-w3schools-com.translate.goog/c/c_ref_ctype.php?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc
https://www-w3schools-com.translate.goog/c/c_ref_math.php?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc







