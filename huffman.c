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
    No *lista;
    int tamanho;
    int capacidade;
} minHeap;

// Inicializa o minHeap
minHeap *constroi_minHeap(int capacidade)
{
    minHeap *heap = (minHeap *)malloc(sizeof(minHeap));
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    heap->lista = (No *)malloc(capacidade * sizeof(No));
    return heap;
}

// Libera memoria
void libera_minHeap(minHeap *minHeap)
{
    free(minHeap->lista);
    free(minHeap);
}

// Troca a com b
void troca(No *a, No *b)
{
    No temp = *a;
    *a = *b;
    *b = temp;
}
void imprimirMinHeap(minHeap *minHeap)
{
    printf("Elementos da min-heap:\n");
    for (int i = 0; i < minHeap->tamanho; i++)
    {
        printf("Caracter: %c, Frequencia: %d\n", minHeap->lista[i].letra, minHeap->lista[i].frequencia);
    }
}
// Função desce
void desce(minHeap *minHeap, int i)
{
    int esquerdo = 2 * i + 1;
    int direito = 2 * i + 2;
    int menor = i;

    if (esquerdo < minHeap->tamanho && minHeap->lista[esquerdo].frequencia < minHeap->lista[i].frequencia)
        menor = esquerdo;
    if (direito < minHeap->tamanho && minHeap->lista[direito].frequencia < minHeap->lista[menor].frequencia)
        menor = direito;

    if (menor != i)
    {
        troca(&minHeap->lista[i], &minHeap->lista[menor]);
        desce(minHeap, menor);
    }
}

// sobe
void sobe(minHeap *minHeap, int i)
{

    int pai = (i - 1) / 2;

    while (i > 0 && minHeap->lista[i].frequencia < minHeap->lista[pai].frequencia)
    {
        troca(&minHeap->lista[i], &minHeap->lista[pai]);
        i = pai;
        pai = (i - 1) / 2;
    }
}

void insere(minHeap *minHeap, No *novo)
{
    if (minHeap->tamanho < minHeap->capacidade)
    {
        No *novo_no = (No *)malloc(sizeof(No));
        novo_no->letra = novo->letra;
        novo_no->frequencia = novo->frequencia;
        minHeap->lista[minHeap->tamanho] = *novo_no;
        minHeap->tamanho++;
        sobe(minHeap, minHeap->tamanho-1);
        free(novo_no); // Liberar memória alocada
    }
}

int extrai_Min(minHeap *minHeap)
{

    int min = minHeap->lista[0].frequencia;
    minHeap->lista[0] = minHeap->lista[minHeap->tamanho - 1];
    minHeap->tamanho--;
    desce(minHeap, 0);
    printf("dentro da extrai \n");
    imprimirMinHeap(minHeap);
    return min;
}

int retorna_min(minHeap *minHeap)
{
    return minHeap->lista[0].frequencia;
}

// -------------------------Árvore-----------------------------------

minHeap *montar_arvore(minHeap *heap)
{
    minHeap *primeiro, *segundo, *novo;
    while (heap->tamanho > 1)
    {

        minHeap *primeiro = (minHeap *)malloc(sizeof(minHeap));
        minHeap *segundo = (minHeap *)malloc(sizeof(minHeap));
        primeiro->lista = (No *)malloc(sizeof(No));
        primeiro->lista->letra = heap->lista[0].letra;
        primeiro->lista->frequencia = heap->lista[0].frequencia;
        primeiro->tamanho = 1;

        // printf("Depois de remover o primeiro \n");
        // imprimirMinHeap(heap);

        segundo->lista = (No *)malloc(sizeof(No));
        segundo->lista->letra = heap->lista[1].letra;
        segundo->lista->frequencia = heap->lista[1].frequencia;
        segundo->tamanho = 1;


        novo = (minHeap *)malloc(sizeof(minHeap));
        novo->lista = (No *)malloc(sizeof(No));
        if (novo)
        {
            novo->lista->letra = '+';
            novo->lista->frequencia = primeiro->lista->frequencia + segundo->lista->frequencia;
            insere(heap, novo->lista);
            
        }
        else
        {
            printf("\n\t ERRO ao alocar memória");
            break;
        }
       
        extrai_Min(heap);
        extrai_Min(heap);
        free(primeiro->lista);
        free(primeiro);
        free(segundo->lista);
        free(segundo);
        free(novo->lista);
        free(novo);
    }

    return heap;
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
    // Lista lista;

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
    minHeap *minHeap;

    minHeap = constroi_minHeap(MAX);

    // Insere os elementos na min-heap
    for (int i = 0; i < MAX; i++)
    {
        if (tabela_frequencia[i] > 0)
        {
            No novo;
            novo.letra = i;
            novo.frequencia = tabela_frequencia[i];
            insere(minHeap, &novo);
        }
    }
    // -------------------------Arvore----------------------------------
    montar_arvore(minHeap);
    printf("dentro da Main \n");
    imprimirMinHeap(minHeap);

    return 0;
}
