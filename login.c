#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"
#include "utils.h"

int autenticar(void)
{
    int opcao;
    long long hash;
    char usuario[50];
    char senha[50];
    int i;

    limparTela();

    do
    {
        printf("\n\n===== TELA DE LOGIN =====\n\n");
        printf("1 - Login\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        scanf("%d", &opcao);
        getchar();

        if (opcao == 0)
        {
            printf("\nSistema encerrado.\n");
            return 0;
        }
        else if (opcao == 1)
        {
            printf("\nUsuario: ");
            fgets(usuario, sizeof(usuario), stdin);
            usuario[strcspn(usuario, "\n")] = '\0';

            printf("Senha: ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = '\0';

            hash = 0;

            for (i = 0; senha[i] != '\0'; i++)
            {
                hash = (hash * 31) + (unsigned char)senha[i];
            }

            if (strcmp(usuario, "admin@sistema.com") == 0 && hash == 1509442)
            {
                printf("\nAcesso Concedido!\n");
                return 1;
            }
            else
            {
                limparTela();
                printf("\nUsuario ou Senha Incorretos!\n\n");
            }
        }
        else
        {
            printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
