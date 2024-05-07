#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256 // ASCII

//inicializando tabela de frequencia com 0 
void inicializa_tabela_frequencia(unsigned int tabela[]){
    for (int i = 0; i < MAX; i++)
        tabela[i] = 0;
}

void preenche_tabela_frequencia(unsigned char texto[], unsigned int tabela[]){
    int compare = 0;
    int i = 0;
    while (texto[i] != '\0')
    {
        tabela[texto[i]]++;
        i++;
    }
}

void imprime_tabela_frequancia(unsigned int tabela[]){
    for (int i = 0; i < MAX; i++)
    {
        printf("\t%d = %d = %c\n", i, tabela[i], i);
    }
    

}



int main(int argc, char *argv[]) {
    
    unsigned char linha[MAX];
    unsigned int tabela_frequencia[MAX];


    // -------------------------Leitura-do-Arquivo-----------------------------------

    FILE *arquivo;
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


    // -------------------------Tabela-de-frequencia-----------------------------------
    inicializa_tabela_frequencia(tabela_frequencia);
    preenche_tabela_frequencia(linha, tabela_frequencia);
    //imprime_tabela_frequancia(tabela_frequencia);
    return 0;
}
