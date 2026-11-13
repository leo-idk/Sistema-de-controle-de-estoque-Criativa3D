#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define MAX_PRODUTOS 100

typedef struct
{
    int codigo;
    char categoria[20];
    char unidade[10];
    double preco;
    double quantidade;
    double estoqueMinimo;
    double estoqueMaximo;
    char localizacao[30];
    int estado_produto;
} Produto;

typedef struct
{
    int codigo_produto; // PK e FK
    char tipo[50];
    char cor[50];
    char marca[50];
} Filamento;

typedef struct
{
    int codigo_produto; // PK e FK
    char nome[50];
    char descricao[100];
} Personalizado;

typedef struct
{
    int id_mov; // PK
    int codigo_produto;
    int tipo;
    double quantidade;
    char data[20];
} Movimentacao;

// Variaveis globais compartilhadas entre os modulos
extern Produto produtos[MAX_PRODUTOS];
extern Filamento filamentos[MAX_PRODUTOS];
extern Personalizado personalizados[MAX_PRODUTOS];
extern Movimentacao movimentacoes[MAX_PRODUTOS * 10];

extern int totalMovimentacoes;
extern int totalProdutos;
extern int proximoCodigo;
extern int cancelouOperacao;

#endif