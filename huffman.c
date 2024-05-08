#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256 // ASCII

// -------------------------Lista-encadeada-e-AVBB-----------------------------------

typedef struct no
{
    unsigned char letra;
    int frequencia;
    struct no *esq, *dir, *prox;
} No;

typedef struct
{
    No *inicio;
    int tam;
} Lista;

void cria_lista(Lista *lista)
{

    lista->inicio = NULL;
    lista->tam = 0;
}
void ordenar_lista(Lista *lista, No *no)
{
    No *aux;
    if (lista->inicio == NULL)
    {
        lista->inicio = no;
        // lista->tam++;
    }
    else if (lista->inicio->frequencia > no->frequencia)
    {
        no->prox = lista->inicio;
        lista->inicio = no;
        // lista->tam++;
    }
    else
    {
        aux = lista->inicio;
        while (aux->prox && aux->prox->frequencia <= no->frequencia)
            aux = aux->prox;

        no->prox = aux->prox;
        aux->prox = no;
        // lista->tam++;
    }
    lista->tam++;
}

void inserir_elementos(unsigned int tabela[], Lista *lista)


{
    int i;
    No *novo;
    for (i = 0; i < MAX; i++)
    {
        if (tabela[i] > 0)
        {
            novo = malloc(sizeof(No));
            if (novo)
            {
               novo->letra = i;
               novo->frequencia = tabela[i];
               novo->dir = NULL;
               novo->esq = NULL;
               novo->prox = NULL;

               ordenar_lista(lista, novo);
            }
            else
            {
                printf("\tERRO AO ALOCAR MEMÓRIA EM inserir_elementos");
                break;
            }
        }
    }
}

void imprime_lista_encadeada(Lista *lista){
    No *aux;
    aux = lista->inicio;
    while (aux)
    {
        printf("\tCaracter: %c Frequencia: %d\n", aux->letra, aux->frequencia);
        aux = aux->prox;
    }
    

}

// -------------------------Tabela-de-frequencia-----------------------------------

void inicializa_tabela_frequencia(unsigned int tabela[])
{
    for (int i = 0; i < MAX; i++)
        tabela[i] = 0;
}

void preenche_tabela_frequencia(unsigned char texto[], unsigned int tabela[])
{
    int compare = 0;
    int i = 0;
    while (texto[i] != '\0')
    {
        tabela[texto[i]]++;
        i++;
    }
}

void imprime_tabela_frequancia(unsigned int tabela[])
{
    for (int i = 0; i < MAX; i++)
    {
        printf("\t%d = %d = %c\n", i, tabela[i], i);
    }
}

int main(int argc, char *argv[])
{

    unsigned char linha[MAX];
    unsigned int tabela_frequencia[MAX];
    Lista lista;


    // -------------------------Leitura-do-Arquivo-----------------------------------

    FILE *arquivo;
    char nome_arquivo[100]; // Tamanho máximo do nome do arquivo

    // Verifique se o nome do arquivo foi fornecido como argumento de linha de comando
    if (argc != 2)
    {
        printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }

    // Copie o nome do arquivo fornecido pelo usuário
    strcpy(nome_arquivo, argv[1]);

    // Abra o arquivo em modo de leitura
    arquivo = fopen(nome_arquivo, "r");

    // Verifique se o arquivo foi aberto com sucesso
    if (arquivo == NULL)
    {
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
    // imprime_tabela_frequancia(tabela_frequencia);

    // -------------------------Lista-encadeada-e-AVBB-----------------------------------
    cria_lista(&lista);
    inserir_elementos(tabela_frequencia, &lista);
    imprime_lista_encadeada(&lista);

    return 0;
}
