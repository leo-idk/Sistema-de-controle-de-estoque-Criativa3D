#include <stdio.h>
#include "estruturas.h"
#include "login.h"
#include "utils.h"
#include "arquivos.h"
#include "estoque.h"

int main(void)
{
    if (!autenticar())
    {
        return 0;
    }

    int opcao;

    carregarDados();

    limparTela();

    do
    {
        printf("\n\n===== SISTEMA DE CONTROLE DE ESTOQUE =====\n\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Consultar produtos\n");
        printf("3 - Registrar movimentacao\n");
        printf("4 - Consultar historico de movimentacoes\n");
        printf("5 - Alterar produto\n");
        printf("6 - Alterar localizacao\n");
        printf("7 - Alterar estoque minimo/maximo\n");
        printf("8 - Excluir produto\n");
        printf("0 - Sair\n");
        printf("Digite a opcao desejada: ");

        if (!lerInt(&opcao))
        {
            if (cancelouOperacao)
            {
                cancelouOperacao = 0;
                continue;
            }
            printf("Opcao invalida! Digite uma opcao do menu.\n");
            continue;
        }

        cancelouOperacao = 0;

        switch (opcao)
        {
            case 1:
                limparTela();
                cadastrarProduto();
                break;

            case 2:
                limparTela();
                ordenarProdutos();
                break;

            case 3:
                limparTela();
                registrarMovimentacao();
                break;

            case 4:
                limparTela();
                consultarHistoricoMovimentacoes();
                break;

            case 5:
                limparTela();
                alterarProduto();
                break;

            case 6:
                limparTela();
                alterarLocalizacao();
                break;

            case 7:
                limparTela();
                alterarEstoque();
                break;

            case 8:
                limparTela();
                excluirProduto();
                break;

            case 0:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

        if (!cancelouOperacao)
            salvarDados();

        if (opcao != 0 && !cancelouOperacao)
        {
            printf("\nPressione Enter para voltar ao menu principal.");
            getchar();
        }

        if (opcao != 0)
            limparTela();

    } while (opcao != 0);

    return 0;
}
