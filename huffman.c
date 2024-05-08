#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256 // ASCII


// -------------------------Min-Heap-----------------------------------
typedef struct
{
    unsigned char letra;
    int frequencia;
} No;

typedef struct
{
    No *array;
    int tamanho;
    int capacidade;
} MinHeap;

MinHeap* criarMinHeap(int capacidade)
{
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->tamanho = 0;
    minHeap->capacidade = capacidade;
    minHeap->array = (No*)malloc(capacidade * sizeof(No));
    return minHeap;
}

void inserirElemento(MinHeap* minHeap, No no)
{
    if (minHeap->tamanho == minHeap->capacidade)
    {
        printf("A min-heap está cheia\n");
        return;
    }

    // Insere o novo nó no final do array
    int indice = minHeap->tamanho;
    minHeap->array[indice] = no;
    minHeap->tamanho++;

    // Garante que a propriedade da min-heap seja mantida
    while (indice != 0 && minHeap->array[indice].frequencia < minHeap->array[(indice - 1) / 2].frequencia)
    {
        // Troca o novo nó com seu pai até que a propriedade da min-heap seja restaurada
        No temp = minHeap->array[indice];
        minHeap->array[indice] = minHeap->array[(indice - 1) / 2];
        minHeap->array[(indice - 1) / 2] = temp;
        indice = (indice - 1) / 2;
    }
}
void imprimirMinHeap(MinHeap* minHeap)
{
    printf("Elementos da min-heap:\n");
    for (int i = 0; i < minHeap->tamanho; i++)
    {
        printf("Caracter: %c, Frequencia: %d\n", minHeap->array[i].letra, minHeap->array[i].frequencia);
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
    //Lista lista;

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


    // -------------------------Min-Heap----------------------------------
    MinHeap *minHeap;

    minHeap = criarMinHeap(MAX);

    // Insere os elementos na min-heap
    for (int i = 0; i < MAX; i++)
    {
        if (tabela_frequencia[i] > 0)
        {
            No novo;
            novo.letra = i;
            novo.frequencia = tabela_frequencia[i];
            inserirElemento(minHeap, novo);
        }
    }
    
    imprimirMinHeap(minHeap);
   
    return 0;
}







