#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"
#include "utils.h"

unsigned int calcular_hash_simples(const char *str)
{
    unsigned int hash = 0;

    while (*str)
    {
        hash = (hash * 31) + (*str);
        str++;
    }

    return hash;
}

int autenticar(void)
{
    char email[50];
    char senha[50];

    unsigned int HASH_SENHA_ESPERADO = 1509442;

    limparTela();

    while (1)
    {
        printf("\n\n=== TELA DE LOGIN ===\n\n");
        printf("Email: ");

        if (fgets(email, sizeof(email), stdin) == NULL)
            return 0;

        email[strcspn(email, "\n")] = '\0';

        if (strcmp(email, "V") == 0 ||
            strcmp(email, "v") == 0)
            return 0;

        printf("Senha: ");

        if (fgets(senha, sizeof(senha), stdin) == NULL)
            return 0;

        senha[strcspn(senha, "\n")] = '\0';

        if (strcmp(email, "admin@sistema.com") == 0 &&
            calcular_hash_simples(senha) == HASH_SENHA_ESPERADO)
        {
            printf("\nAcesso Concedido!\n");
            return 1;
        }
        else
        {
            limparTela();
            printf("\nEmail ou senha incorretos!\n\n");
        }
    }
}