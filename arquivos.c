#include <stdio.h>
#include "estruturas.h"
#include "arquivos.h"

void salvarDados(void)
{
    FILE *arquivo = fopen("dados.dat", "wb");

    if (arquivo == NULL)
    {
        perror("Erro ao salvar dados.dat");
        return;
    }

    fwrite(&totalProdutos, sizeof(int), 1, arquivo);
    fwrite(&totalMovimentacoes, sizeof(int), 1, arquivo);
    fwrite(&proximoCodigo, sizeof(int), 1, arquivo);

    fwrite(produtos, sizeof(Produto), totalProdutos, arquivo);
    fwrite(filamentos, sizeof(Filamento), totalProdutos, arquivo);
    fwrite(personalizados, sizeof(Personalizado), totalProdutos, arquivo);
    fwrite(movimentacoes, sizeof(Movimentacao), totalMovimentacoes, arquivo);

    fclose(arquivo);
}

void carregarDados(void)
{
    FILE *arquivo = fopen("dados.dat", "rb");

    if (arquivo == NULL)
    {
        return;
    }

    if (fread(&totalProdutos, sizeof(int), 1, arquivo) != 1 ||
        fread(&totalMovimentacoes, sizeof(int), 1, arquivo) != 1 ||
        fread(&proximoCodigo, sizeof(int), 1, arquivo) != 1 ||
        totalProdutos < 0 || totalProdutos > MAX_PRODUTOS ||
        totalMovimentacoes < 0 || totalMovimentacoes > MAX_PRODUTOS * 10 ||
        proximoCodigo < 1)
    {
        totalProdutos = 0;
        totalMovimentacoes = 0;
        proximoCodigo = 1;
        fclose(arquivo);
        return;
    }

    if (fread(produtos, sizeof(Produto), totalProdutos, arquivo) != (size_t)totalProdutos ||
        fread(filamentos, sizeof(Filamento), totalProdutos, arquivo) != (size_t)totalProdutos ||
        fread(personalizados, sizeof(Personalizado), totalProdutos, arquivo) != (size_t)totalProdutos ||
        fread(movimentacoes, sizeof(Movimentacao), totalMovimentacoes, arquivo) != (size_t)totalMovimentacoes)
    {
        totalProdutos = 0;
        totalMovimentacoes = 0;
        proximoCodigo = 1;
    }

    fclose(arquivo);
}
