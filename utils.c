#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estruturas.h"
#include "utils.h"

void limparTela(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int lerInput(char entrada[], int tamanho)
{
    if (fgets(entrada, tamanho, stdin) == NULL)
        return 0;

    entrada[strcspn(entrada, "\n")] = '\0';

    if (strcmp(entrada, "V") == 0 ||
        strcmp(entrada, "v") == 0)
    {
        cancelouOperacao = 1;
        return 0;
    }

    return 1;
}

int lerInt(int *valor)
{
    char entrada[50];
    char *fim;
    long numero;

    while (1)
    {
        if (!lerInput(entrada, sizeof(entrada)))
            return 0;

        numero = strtol(entrada, &fim, 10);

        if (fim != entrada && *fim == '\0')
        {
            *valor = (int)numero;
            return 1;
        }

        printf("Entrada invalida! Digite um numero inteiro: ");
    }
}

int lerDouble(double *valor)
{
    char entrada[50];
    char *fim;

    while (1)
    {
        if (!lerInput(entrada, sizeof(entrada)))
            return 0;

        *valor = strtod(entrada, &fim);

        if (fim != entrada && *fim == '\0')
            return 1;

        printf("Entrada invalida! Digite um numero: ");
    }
}

int lerTexto(char destino[], int tamanho)
{
    return lerInput(destino, tamanho);
}

int verificarProdutosAtivos(void)
{
    int i;

    for (i = 0; i < totalProdutos; i++)
    {
        if (produtos[i].estado_produto == 1)
            return 1;
    }

    return 0;
}

int selecionarProduto(void)
{
    int codigo, i;

    while (1)
    {
        printf("\nDigite o ID do produto: ");

        if (!lerInt(&codigo))
            return -1;

        if (codigo <= 0)
        {
            printf("Codigo invalido! Tente novamente.\n");
            continue;
        }

        for (i = 0; i < totalProdutos; i++)
        {
            if (produtos[i].codigo == codigo &&
                produtos[i].estado_produto == 1)
            {
                return i;
            }
        }

        printf("Produto nao encontrado! Tente novamente.\n");
    }
}