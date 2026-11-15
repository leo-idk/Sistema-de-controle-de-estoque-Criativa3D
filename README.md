# Sistema-de-controle-de-estoque-Criativa3D

Projeto acadêmico desenvolvido em linguagem C para o controle de estoque de produtos 3D.

## Compilação e execução

**Linux:**

```bash
gcc -std=c11 -Wall -Wextra *.c -o sistema_estoque
./sistema_estoque
```

**Windows:**

```bash
gcc -std=c11 -Wall -Wextra *.c -o sistema_estoque.exe
.\sistema_estoque.exe
```

**Code::Blocks:**

Abra o arquivo do projeto (`.cbp`) no Code::Blocks e utilize a opção **Build and Run (F9)**.

## Autenticação

A autenticação utiliza uma função de hash simplificada, que percorre os caracteres da senha, multiplica o resultado acumulado por 31 e soma o código de cada caractere.

A senha de demonstração é `1234`, cujo resultado é `1509442`. Esse valor é utilizado na condição de autenticação.

Caso o resultado calculado seja diferente, adicione temporariamente `printf("\nHash calculado: %lld\n", hash);` após o laço de cálculo em `login.c`. Execute o programa, verifique o valor apresentado e substitua `1509442` na condição de autenticação pelo resultado obtido.

**Observação:** a função utilizada possui finalidade didática e não oferece segurança criptográfica para o armazenamento de senhas.
