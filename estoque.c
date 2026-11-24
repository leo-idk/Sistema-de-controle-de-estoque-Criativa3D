#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "estruturas.h"
#include "utils.h"
#include "estoque.h"

// Definicao real das variaveis globais
Produto produtos[MAX_PRODUTOS];
Filamento filamentos[MAX_PRODUTOS];
Personalizado personalizados[MAX_PRODUTOS];
Movimentacao movimentacoes[MAX_PRODUTOS * 10];

int totalMovimentacoes = 0;
int totalProdutos = 0;
int proximoCodigo = 1;
int cancelouOperacao = 0;

void cadastrarProduto(void)
{
    if (totalProdutos >= MAX_PRODUTOS)
    {
        printf("Limite de produtos atingido!\n");
        return;
    }

    Produto p = {0};
    Filamento f = {0};
    Personalizado pe = {0};

    int opcao, j, quantidadeInteira;

    // Selecionar a categoria e cadastrar os dados especificos
    do
    {
        printf("Selecione a categoria:\n1 - Filamento\n2 - Personalizado\n");
        printf("Digite a opcao desejada: ");
        if (!lerInt(&opcao))
            return;

        if (opcao == 1)
        {
            strcpy(p.categoria, "Filamento");
            strcpy(p.unidade, "gramas");

            printf("Digite o tipo do filamento: ");
            if (!lerTexto(f.tipo, sizeof(f.tipo)))
                return;

            printf("Digite a marca do filamento: ");
            if (!lerTexto(f.marca, sizeof(f.marca)))
                return;

            printf("Digite a cor do filamento: ");
            if (!lerTexto(f.cor, sizeof(f.cor)))
                return;
        }
        else if (opcao == 2)
        {
            strcpy(p.categoria, "Personalizado");
            strcpy(p.unidade, "unidades");

            printf("Digite o nome do produto: ");
            if (!lerTexto(pe.nome, sizeof(pe.nome)))
                return;

            printf("Digite a descricao do produto: ");
            if (!lerTexto(pe.descricao, sizeof(pe.descricao)))
                return;
        }
        else
        {
            printf("Categoria invalida!\n");
        }

    } while (opcao != 1 && opcao != 2);

    // Cadastrar a quantidade
    do
    {
        printf("Digite a quantidade do produto: ");

        if (opcao == 1)
        {
            if (!lerDouble(&p.quantidade))
                return;
        }
        else
        {
            if (!lerInt(&quantidadeInteira))
                return;

            p.quantidade = quantidadeInteira;
        }

        if (p.quantidade < 0)
        {
            printf("Erro: a quantidade nao pode ser negativa!\n");
        }

    } while (p.quantidade < 0);

    // Cadastrar a localizacao
    printf("Digite a localizacao do produto: ");
    if (!lerTexto(p.localizacao, sizeof(p.localizacao)))
        return;

    // Converter localizacao para letras maiusculas
    for (j = 0; p.localizacao[j] != '\0'; j++)
    {
        p.localizacao[j] = toupper((unsigned char)p.localizacao[j]);
    }

    // Cadastrar o estoque minimo
    do
    {
        printf("Digite o estoque minimo: ");
        if (opcao == 2)
        {
            if (!lerInt(&quantidadeInteira))
                return;

            p.estoqueMinimo = quantidadeInteira;
        }
        else
        {
            if (!lerDouble(&p.estoqueMinimo))
                return;
        }

        if (p.estoqueMinimo < 0)
        {
            printf("Erro: o estoque minimo nao pode ser negativo!\n");
        }

    } while (p.estoqueMinimo < 0);

    // Cadastrar o estoque maximo
    do
    {
        printf("Digite o estoque maximo: ");
        if (opcao == 2)
        {
            if (!lerInt(&quantidadeInteira))
                return;

            p.estoqueMaximo = quantidadeInteira;
        }
        else
        {
            if (!lerDouble(&p.estoqueMaximo))
                return;
        }

        if (p.estoqueMaximo < 0)
        {
            printf("Erro: o estoque maximo nao pode ser negativo!\n");
        }
        else if (p.estoqueMaximo < p.estoqueMinimo)
        {
            printf("Erro: o estoque maximo nao pode ser menor que o estoque minimo!\n");
        }
        else if (p.estoqueMaximo < p.quantidade)
        {
            printf("Erro: o estoque maximo nao pode ser menor que a quantidade!\n");
        }

    } while (p.estoqueMaximo < 0 || p.estoqueMaximo < p.estoqueMinimo || p.estoqueMaximo < p.quantidade);

    // Cadastrar o preco
    do
    {
        printf("Digite o preco do produto: ");
        if (!lerDouble(&p.preco))
            return;

        if (p.preco < 0)
        {
            printf("Erro: o preco nao pode ser negativo!\n");
        }

    } while (p.preco < 0);

    // Salvar os dados na matriz
    p.codigo = proximoCodigo;
    p.estado_produto = 1;

    produtos[totalProdutos] = p;

    if (opcao == 1)
    {
        f.codigo_produto = p.codigo;
        filamentos[totalProdutos] = f;
    }
    else
    {
        pe.codigo_produto = p.codigo;
        personalizados[totalProdutos] = pe;
    }

    totalProdutos++;
    proximoCodigo = proximoCodigo + 1;

    // Exibir mensagem de sucesso
    printf("\nProduto cadastrado com sucesso!\n");

    return;
}

void excluirProduto(void)
{
    int opcao, i;

    // Verificar se existem produtos ativos
    if (verificarProdutosAtivos() == 0)
    {
        printf("Nenhum produto ativo cadastrado!\n");
        return;
    }

    listarProdutos(-1);
    i = selecionarProduto();
    if (i == -1)
        return;
    limparTela();

    exibirDadosProduto(i);

    // Confirmar exclusao
    do
    {
        printf("\nDeseja realmente excluir este produto?\n\n");
        printf("1 - Sim\n");
        printf("2 - Nao\n");
        printf("Digite a opcao: ");

        if (!lerInt(&opcao))
            return;

        if (opcao != 1 && opcao != 2)
        {
            printf("Opcao invalida! Digite novamente.\n");
        }

    } while (opcao != 1 && opcao != 2);

    // Excluir ou cancelar
    if (opcao == 1)
    {
        produtos[i].estado_produto = 0;

        printf("Item removido com sucesso!\n");
    }
    else
    {
        printf("Exclusao cancelada!\n");
    }

    return;
}

void alterarProduto(void)
{
    int i;
    Filamento fTemp;
    Personalizado peTemp;
    double novoPreco;

    // Verificar se existem produtos ativos
    if (verificarProdutosAtivos() == 0)
    {
        printf("Nenhum produto ativo cadastrado!\n");
        return;
    }

    listarProdutos(-1);
    i = selecionarProduto();
    if (i == -1)
        return;
    limparTela();

    fTemp = filamentos[i];
    peTemp = personalizados[i];
    novoPreco = produtos[i].preco;

    // Alterar dados conforme a categoria do produto
    if (strcmp(produtos[i].categoria, "Filamento") == 0)
    {
        printf("Digite o novo tipo: ");
        if (!lerTexto(fTemp.tipo, sizeof(fTemp.tipo)))
            return;

        printf("Digite a nova cor: ");
        if (!lerTexto(fTemp.cor, sizeof(fTemp.cor)))
            return;

        printf("Digite a nova marca: ");
        if (!lerTexto(fTemp.marca, sizeof(fTemp.marca)))
            return;
    }
    else if (strcmp(produtos[i].categoria, "Personalizado") == 0)
    {
        printf("Digite o novo nome: ");
        if (!lerTexto(peTemp.nome, sizeof(peTemp.nome)))
            return;

        printf("Digite a nova descricao: ");
        if (!lerTexto(peTemp.descricao, sizeof(peTemp.descricao)))
            return;
    }

    // Alterar preco
    do
    {
        printf("Digite o novo preco: ");
        if (!lerDouble(&novoPreco))
            return;

        if (novoPreco < 0)
        {
            printf("Preco invalido! Digite novamente.\n");
        }

    } while (novoPreco < 0);

    if (strcmp(produtos[i].categoria, "Filamento") == 0)
        filamentos[i] = fTemp;
    else
        personalizados[i] = peTemp;

    produtos[i].preco = novoPreco;

    // Exibir resultado
    printf("\nProduto alterado com sucesso!\n");

    exibirDadosProduto(i);

    printf("Preco: R$ %.2f\n", produtos[i].preco);

    return;
}

void registrarMovimentacao(void)
{
    int codBusca, tipo, i, opcao, quantidadeInteira;
    double quantidade;
    time_t agora;
    struct tm *dataAtual;

    char data_mov[11];

    // Verificar se existem produtos ativos
    if (verificarProdutosAtivos() == 0)
    {
        printf("Nenhum produto ativo cadastrado!\n");
        return;
    }

    // Verificar limite de movimentacoes
    if (totalMovimentacoes >= MAX_PRODUTOS * 10)
    {
        printf("Limite de movimentacoes atingido!\n");
        return;
    }

    printf("\n========== MENU DE MOVIMENTO ==========\n");
    printf("1 - Entrada\n");
    printf("2 - Saida\n");

    do
    {
        printf("Escolha o tipo de movimento: ");
        if (!lerInt(&tipo))
            return;

        if (tipo != 1 && tipo != 2)
            printf("Tipo de movimento invalido!\n");

    } while (tipo != 1 && tipo != 2);

    listarProdutos(-1);
    /// Selecionar produto pelo codigo
    i = selecionarProduto();
    if (i == -1)
        return;
    limparTela();
    codBusca = produtos[i].codigo;
    exibirDadosProduto(i);

    if (strcmp(produtos[i].categoria, "Personalizado") == 0)
    {
        printf("Estoque atual: %.0f unidades\n", produtos[i].quantidade);
        printf("Estoque minimo: %.0f unidades\n", produtos[i].estoqueMinimo);
        printf("Estoque maximo: %.0f unidades\n", produtos[i].estoqueMaximo);
    }
    else
    {
        printf("Estoque atual: %.2f gramas\n", produtos[i].quantidade);
        printf("Estoque minimo: %.2f gramas\n", produtos[i].estoqueMinimo);
        printf("Estoque maximo: %.2f gramas\n", produtos[i].estoqueMaximo);
    }

    // Cadastrar quantidade
    do
    {
        printf("Digite a quantidade: ");

        if (strcmp(produtos[i].categoria, "Personalizado") == 0)
        {
            if (!lerInt(&quantidadeInteira))
                return;

            quantidade = quantidadeInteira;
        }
        else
        {
            if (!lerDouble(&quantidade))
                return;
        }

        if (quantidade <= 0)
            printf("Quantidade deve ser maior que zero!\n");
        else if (tipo == 1 && produtos[i].quantidade + quantidade > produtos[i].estoqueMaximo)
            printf("Estoque maximo ultrapassado!\n");
        else if (tipo == 2 && quantidade > produtos[i].quantidade)
            printf("Estoque insuficiente!\n");

    } while (quantidade <= 0 ||
             (tipo == 1 && produtos[i].quantidade + quantidade > produtos[i].estoqueMaximo) ||
             (tipo == 2 && quantidade > produtos[i].quantidade));

    // Confirmar movimentacao
    printf("\nConfirmar %s? (1 - Sim / 2 - Nao): ",
           tipo == 1 ? "entrada" : "saida");

    do
    {
        if (!lerInt(&opcao))
            return;

        if (opcao != 1 && opcao != 2)
            printf("Opcao invalida! Digite 1 para Sim ou 2 para Nao: ");

    } while (opcao != 1 && opcao != 2);

    if (opcao == 2)
    {
        printf("\nMovimentacao cancelada!\n");
        return;
    }

    // Atualizar estoque
    if (tipo == 1)
    {
        produtos[i].quantidade += quantidade;
        printf("Tipo: Entrada\n");
    }
    else
    {
        produtos[i].quantidade -= quantidade;
        printf("Tipo: Saida\n");
    }

    // Registrar data
    agora = time(NULL);
    dataAtual = localtime(&agora);

    strftime(data_mov, sizeof(data_mov), "%d/%m/%Y", dataAtual);

    // Salvar movimentacao
    movimentacoes[totalMovimentacoes].id_mov = totalMovimentacoes + 1;
    movimentacoes[totalMovimentacoes].codigo_produto = codBusca;
    movimentacoes[totalMovimentacoes].tipo = tipo;
    movimentacoes[totalMovimentacoes].quantidade = quantidade;
    strcpy(movimentacoes[totalMovimentacoes].data, data_mov);

    totalMovimentacoes++;

    // Exibir resultado
    printf("\nMovimentacao registrada com sucesso!\n");

    exibirDadosProduto(i);

    if (strcmp(produtos[i].categoria, "Personalizado") == 0)
    {
        printf("Quantidade movimentada: %.0f unidades\n", quantidade);
        printf("Novo estoque: %.0f unidades\n", produtos[i].quantidade);
    }
    else
    {
        printf("Quantidade movimentada: %.2f gramas\n", quantidade);
        printf("Novo estoque: %.2f gramas\n", produtos[i].quantidade);
    }

    printf("Data: %s\n", data_mov);
}

void listarProdutos(int indice)
{
    int i;

    if (indice < 0)
        printf("\n========== PRODUTOS CADASTRADOS ==========\n");

    if (indice >= 0)
        i = indice;
    else
        i = 0;

    for (; i < (indice >= 0 ? indice + 1 : totalProdutos); i++)
    {
        if (produtos[i].estado_produto == 1)
        {
            printf("\nID: %d | ", produtos[i].codigo);
            printf("Categoria: %s\n", produtos[i].categoria);

            if (strcmp(produtos[i].categoria, "Filamento") == 0)
            {
                printf("Tipo: %s | ", filamentos[i].tipo);
                printf("Marca: %s | ", filamentos[i].marca);
                printf("Cor: %s\n", filamentos[i].cor);
                printf("Preco: R$ %.2f | ", produtos[i].preco);
            }
            else if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            {
                printf("Nome: %s", personalizados[i].nome);
                printf("\nDescricao: %s", personalizados[i].descricao);
                printf("\nPreco: R$ %.2f | ", produtos[i].preco);
            }

            if (strcmp(produtos[i].categoria, "Personalizado") == 0)
                printf("Estoque atual: %.0f unidades\n", produtos[i].quantidade);
            else
                printf("Estoque atual: %.2f gramas\n", produtos[i].quantidade);
            printf("Localizacao: %s\n", produtos[i].localizacao);

            printf("------------------------------------------\n");
        }
    }
}

void exibirDadosProduto(int i)
{
    printf("\nProduto selecionado:\n");
    printf("ID: %d | ", produtos[i].codigo);
    printf("Categoria: %s\n", produtos[i].categoria);

    if (strcmp(produtos[i].categoria, "Filamento") == 0)
    {
        printf("Tipo: %s\n", filamentos[i].tipo);
        printf("Marca: %s\n", filamentos[i].marca);
        printf("Cor: %s\n", filamentos[i].cor);
    }
    else if (strcmp(produtos[i].categoria, "Personalizado") == 0)
    {
        printf("Nome: %s\n", personalizados[i].nome);
        printf("Descricao: %s\n", personalizados[i].descricao);
    }
}

void alterarEstoque(void)
{
    int opcao, i, quantidadeInteira;
    double novoValor;

    // Verificar se existem produtos ativos
    if (verificarProdutosAtivos() == 0)
    {
        printf("Nenhum produto ativo cadastrado!\n");
        return;
    }
    // Selecionar tipo de estoque
    do
    {
        printf("\n===== ALTERAR ESTOQUE =====\n");
        printf("1 - Estoque minimo\n");
        printf("2 - Estoque maximo\n");
        printf("Digite a opcao desejada: ");

        if (!lerInt(&opcao))
            return;

        if (opcao != 1 && opcao != 2)
        {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 1 && opcao != 2);

    // Listar e selecionar produto
    listarProdutos(-1);
    i = selecionarProduto();
    if (i == -1)
        return;
    limparTela();

    // Alterar estoque minimo
    if (opcao == 1)
    {
        exibirDadosProduto(i);

        if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            printf("Estoque minimo atual: %.0f unidades\n", produtos[i].estoqueMinimo);
        else
            printf("Estoque minimo atual: %.2f gramas\n", produtos[i].estoqueMinimo);

        do
        {
            printf("Digite o novo estoque minimo: ");
            if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            {
                if (!lerInt(&quantidadeInteira))
                    return;

                novoValor = quantidadeInteira;
            }
            else
            {
                if (!lerDouble(&novoValor))
                    return;
            }

            if (novoValor < 0 || novoValor > produtos[i].estoqueMaximo)
            {
                printf("Erro: estoque minimo invalido! Tente novamente.\n");
            }

        } while (novoValor < 0 || novoValor > produtos[i].estoqueMaximo);

        produtos[i].estoqueMinimo = novoValor;

        printf("\nEstoque minimo alterado com sucesso!\n");

        exibirDadosProduto(i);

        if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            printf("Novo estoque minimo: %.0f unidades\n", produtos[i].estoqueMinimo);
        else
            printf("Novo estoque minimo: %.2f gramas\n", produtos[i].estoqueMinimo);
    }
    // Alterar estoque maximo
    else if (opcao == 2)
    {
        exibirDadosProduto(i);

        if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            printf("Estoque maximo atual: %.0f unidades\n", produtos[i].estoqueMaximo);
        else
            printf("Estoque maximo atual: %.2f gramas\n", produtos[i].estoqueMaximo);
        do
        {
            printf("Digite o novo estoque maximo: ");
            if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            {
                if (!lerInt(&quantidadeInteira))
                    return;

                novoValor = quantidadeInteira;
            }
            else
            {
                if (!lerDouble(&novoValor))
                    return;
            }

            if (novoValor <= 0 ||
                novoValor < produtos[i].estoqueMinimo ||
                novoValor < produtos[i].quantidade)
            {
                printf("Erro: estoque maximo invalido!\n");
            }

        } while (novoValor <= 0 ||
                 novoValor < produtos[i].estoqueMinimo ||
                 novoValor < produtos[i].quantidade);

        produtos[i].estoqueMaximo = novoValor;

        printf("\nEstoque maximo alterado com sucesso!\n");

        exibirDadosProduto(i);

        if (strcmp(produtos[i].categoria, "Personalizado") == 0)
            printf("Novo estoque maximo: %.0f unidades\n", produtos[i].estoqueMaximo);
        else
            printf("Novo estoque maximo: %.2f gramas\n", produtos[i].estoqueMaximo);
    }

    return;
}

void consultarHistoricoMovimentacoes(void)
{
    int i, j;

    printf("\n===== HISTÓRICO DE MOVIMENTAÇÕES =====\n");

    if (totalMovimentacoes == 0)
    {
        printf("Nenhuma movimentacao registrada no momento.\n");
    }
    else
    {
        for (i = 0; i < totalMovimentacoes; i++)
        {
            printf("\n====================================\n");
            printf("[ID: %d]\n", movimentacoes[i].id_mov);

            j = 0;

            while (j < totalProdutos)
            {
                if (movimentacoes[i].codigo_produto == produtos[j].codigo)
                {
                    exibirDadosProduto(j);

                    if (strcmp(produtos[j].categoria, "Personalizado") == 0)
                        printf("Estoque atual: %.0f unidades\n", produtos[j].quantidade);
                    else
                        printf("Estoque atual: %.2f gramas\n", produtos[j].quantidade);

                    break;
                }
                j++;
            }

            if (movimentacoes[i].tipo == 1)
            {
                printf("Tipo de movimentacao: Entrada\n");
            }
            else
            {
                printf("Tipo de movimentacao: Saida\n");
            }

            if (j < totalProdutos &&
                strcmp(produtos[j].categoria, "Personalizado") == 0)
                printf("Quantidade movimentada: %.0f unidades\n",
                       movimentacoes[i].quantidade);
            else
                printf("Quantidade movimentada: %.2f gramas\n",
                       movimentacoes[i].quantidade);

            printf("Data: %s\n",
                   movimentacoes[i].data);
        }
    }
}

void alterarLocalizacao(void)
{
    int i, j;
    char novaLocalizacao[30];

    // Verificar se existem produtos ativos
    if (verificarProdutosAtivos() == 0)
    {
        printf("Nenhum produto ativo cadastrado!\n");
        return;
    }

    // Listar produtos cadastrados
    listarProdutos(-1);

    // Selecionar produto pelo codigo
    i = selecionarProduto();
    if (i == -1)
        return;
    limparTela();

    // Exibir todos os dados do produto selecionado
    exibirDadosProduto(i);
    printf("\nLocalizacao atual: %s\n", produtos[i].localizacao);
    // Informar nova localizacao
    printf("\nDigite a nova localizacao: ");
    if (!lerTexto(novaLocalizacao, sizeof(novaLocalizacao)))
        return;

    // Converter localizacao para letras maiusculas
    for (j = 0; novaLocalizacao[j] != '\0'; j++)
    {
        novaLocalizacao[j] = toupper((unsigned char)novaLocalizacao[j]);
    }

    // Salvar nova localizacao
    strcpy(produtos[i].localizacao, novaLocalizacao);

    // Exibir resultado
    printf("\nLocalizacao alterada com sucesso!\n");
    exibirDadosProduto(i);
    printf("Localizacao atualizada: %s\n", produtos[i].localizacao);
}

static int compararPorPreco(const void *a, const void *b)
{
    int i = *(int *)a;
    int j = *(int *)b;

    return (produtos[i].preco > produtos[j].preco) -
           (produtos[i].preco < produtos[j].preco);
}

static int compararPorNome(const void *a, const void *b)
{
    int i = *(int *)a;
    int j = *(int *)b;

    char nome1[102], nome2[102];
    if (strcmp(produtos[i].categoria, "Filamento") == 0)
    {
        sprintf(nome1, "%s - %s", filamentos[i].tipo, filamentos[i].cor);
    }
    else if (strcmp(produtos[i].categoria, "Personalizado") == 0)
    {
        strcpy(nome1, personalizados[i].nome);
    }
    if (strcmp(produtos[j].categoria, "Filamento") == 0)
    {
        sprintf(nome2, "%s - %s", filamentos[j].tipo, filamentos[j].cor);
    }
    else if (strcmp(produtos[j].categoria, "Personalizado") == 0)
    {
        strcpy(nome2, personalizados[j].nome);
    }
    return strcmp(nome1, nome2);
}

void ordenarProdutos(void)
{
    int opcao;
    int indices[MAX_PRODUTOS];
    int i, j, aux;
    int encontrados = 0;
    static int categoriaAtual = 0;

    do
    {
        encontrados = 0;
        limparTela();

        printf("\n\n--- CONSULTAR PRODUTOS ---\n\n");
        printf("1 - Listar por categoria\n");
        printf("2 - Ordenar por preco\n");
        printf("3 - Ordenar por nome\n");
        printf("4 - Listar produtos com estoque baixo\n");
        printf("v - Voltar ao menu principal\n");
        printf("Digite a opcao: ");

        if (!lerInt(&opcao))
            return;

        if (verificarProdutosAtivos() == 0)
        {
            printf("\nNenhum produto ativo cadastrado!\n");
            printf("\nPressione Enter para continuar.");
            getchar();
            continue;
        }
        else
        {
            for (i = 0; i < totalProdutos; i++)
            {
                indices[i] = i;
            }

            if (opcao == 1)
            {
                if (categoriaAtual == 0)
                {
                    printf("\n--- LISTA DE FILAMENTOS ---\n");

                    for (i = 0; i < totalProdutos; i++)
                    {
                        if (produtos[i].estado_produto == 1 &&
                            strcmp(produtos[i].categoria, "Filamento") == 0)
                        {
                            listarProdutos(i);
                        }
                    }

                    categoriaAtual = 1;
                }
                else
                {
                    printf("\n--- LISTA DE PERSONALIZADOS ---\n");

                    for (i = 0; i < totalProdutos; i++)
                    {
                        if (produtos[i].estado_produto == 1 &&
                            strcmp(produtos[i].categoria, "Personalizado") == 0)
                        {
                            listarProdutos(i);
                        }
                    }

                    categoriaAtual = 0;
                }
            }
            else if (opcao == 2)
            {
                for (i = 0; i < totalProdutos - 1; i++)
                {
                    for (j = i + 1; j < totalProdutos; j++)
                    {
                        if (compararPorPreco(&indices[i], &indices[j]) > 0)
                        {
                            aux = indices[i];
                            indices[i] = indices[j];
                            indices[j] = aux;
                        }
                    }
                }

                printf("\n--- PRODUTOS ORDENADOS POR PRECO ---\n");

                for (i = 0; i < totalProdutos; i++)
                {
                    if (produtos[indices[i]].estado_produto == 1)
                    {
                        listarProdutos(indices[i]);
                    }
                }
            }
            else if (opcao == 3)
            {
                for (i = 0; i < totalProdutos - 1; i++)
                {
                    for (j = i + 1; j < totalProdutos; j++)
                    {
                        if (compararPorNome(&indices[i], &indices[j]) > 0)
                        {
                            aux = indices[i];
                            indices[i] = indices[j];
                            indices[j] = aux;
                        }
                    }
                }

                printf("\n--- PRODUTOS ORDENADOS POR NOME ---\n");

                for (i = 0; i < totalProdutos; i++)
                {
                    if (produtos[indices[i]].estado_produto == 1)
                    {
                        listarProdutos(indices[i]);
                    }
                }
            }
            else if (opcao == 4)
            {
                printf("\n--- PRODUTOS COM ESTOQUE BAIXO ---\n");

                for (i = 0; i < totalProdutos; i++)
                {
                    if (produtos[i].estado_produto == 1 &&
                        produtos[i].quantidade <= produtos[i].estoqueMinimo)
                    {
                        listarProdutos(i);
                        encontrados++;

                        if (produtos[i].quantidade == 0)
                        {
                            printf("ESTADO CRITICO! REPOSICAO NECESSARIA!\n");
                        }
                    }
                }

                if (encontrados == 0)
                {
                    printf("\nNenhum produto com estoque baixo.\n");
                }
            }
            else
            {
                printf("\nOpcao invalida!\n");
            }

            printf("\nPressione Enter para escolher outra consulta.");
            getchar();
            limparTela();
        }

    } while (1);
}
