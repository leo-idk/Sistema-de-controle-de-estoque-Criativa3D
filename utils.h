#ifndef UTILS_H
#define UTILS_H

void limparTela(void);
int lerInput(char entrada[], int tamanho);
int lerInt(int *valor);
int lerDouble(double *valor);
int lerTexto(char destino[], int tamanho);
int verificarProdutosAtivos(void);
int selecionarProduto(void);

#endif