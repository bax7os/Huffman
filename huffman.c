#include <stdio.h>
#include <stdlib.h>


#define MAX 256 // ASCII

int main(int argc, char *argv[]) {
    FILE *arquivo;
    char linha[MAX];
    char nome_arquivo[100]; // Tamanho máximo do nome do arquivo

    // Verifique se o nome do arquivo foi fornecido como argumento de linha de comando
    if (argc != 2) {
        printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }

    // Copie o nome do arquivo fornecido pelo usuário
    strcpy(nome_arquivo, argv[1]);

    // Abra o arquivo em modo de leitura
    arquivo = fopen(nome_arquivo, "r");

    // Verifique se o arquivo foi aberto com sucesso
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Ler uma linha do arquivo e armazená-la em linha
    fgets(linha, MAX, arquivo);


    // Feche o arquivo
    fclose(arquivo);

    return 0;
}
