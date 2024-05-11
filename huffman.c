#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

#define MAX 256 // ASCII


typedef struct No {
    unsigned char letra;
    int frequencia;
    struct No* esquerdo;
    struct No* direito;
} No;

typedef struct TabelaHash {
    char *codigo_huffman;
} TabelaHash;

// -------------------------Min-Heap-----------------------------------
No* cria_no(unsigned char letra, int frequencia) {
    No* newno = (No*)malloc(sizeof(No));
    newno->letra = letra;
    newno->frequencia = frequencia;
    newno->direito = NULL;
    newno->esquerdo = NULL;
    return newno;
}

void insere(No** minHeap, int *tamanho, No* newno) {
    int i = ++(*tamanho);
    while(i > 1 && newno->frequencia < minHeap[i/2]->frequencia) {
        minHeap[i] = minHeap[i/2];
        i = i/2;
    }
    minHeap[i] = newno;
}

No* extrai_Min(No** minheap, int *tamanho) {
    No* menor_no = minheap[1];
    No* ultimo_no = minheap[(*tamanho)--];
    int i = 1;
    int filho = 2*i; 
    while(i*2 <= *tamanho) {
        filho = 2*i;
        if(filho != *tamanho && minheap[filho+1]->frequencia < minheap[filho]->frequencia) {
            filho++;
        }
        if(ultimo_no->frequencia > minheap[filho]->frequencia) {
            minheap[i] = minheap[filho];
        } else {
            break;
        }
        i = filho;
    }
    minheap[i] = ultimo_no;
    return menor_no;
}

// -------------------------Árvore-----------------------------------
No* montar_arvore(unsigned char letra[], int frequencia[], int n) {
    No* minheap[n+1];
    int tamanho = 0;
    for (int i = 0; i < n; i++) {
        No* newno = cria_no(letra[i], frequencia[i]);
        insere(minheap, &tamanho, newno);
    }
    while (tamanho > 1) {
        No* filho_esquerdo = extrai_Min(minheap, &tamanho);
        No* filho_direito = extrai_Min(minheap, &tamanho);
        No* no_interno = cria_no('\0', filho_esquerdo->frequencia + filho_direito->frequencia);
        no_interno->esquerdo = filho_esquerdo;
        no_interno->direito = filho_direito;
        insere(minheap, &tamanho, no_interno); // Inserir o nó interno de volta ao heap mínimo
    }
    return extrai_Min(minheap, &tamanho);
}

// -------------------------Tabela-hash-----------------------------------
void preenche_tabela_huffman(No *raiz, TabelaHash tabela[]) {
    static char codigo[MAX]; // Armazena temporariamente o código de Huffman
    static int indice = 0;   // Índice atual do código de Huffman
    if (raiz->esquerdo != NULL) {
        codigo[indice++] = '0';
        preenche_tabela_huffman(raiz->esquerdo, tabela);
        indice--;
    }
    if (raiz->direito != NULL) {
        codigo[indice++] = '1';
        preenche_tabela_huffman(raiz->direito, tabela);
        indice--;
    }
    if (raiz->esquerdo == NULL && raiz->direito == NULL) {
        // Cria uma entrada na tabela hash para a letra com seu código Huffman
        tabela[raiz->letra].codigo_huffman = (char*)malloc((indice + 1) * sizeof(char));
        strncpy(tabela[raiz->letra].codigo_huffman, codigo, indice);
        tabela[raiz->letra].codigo_huffman[indice] = '\0'; // Adiciona o caractere nulo no final
    }
}


void imprime_tabela_huffman(TabelaHash tabela[]) {
    printf("========= Tabela de Huffman =========\n");
    for (int i = 0; i < MAX; i++) {
        if (tabela[i].codigo_huffman != NULL) {
            printf(" %d : %c : %s\n", i, i, tabela[i].codigo_huffman);
        }
    }
}

// -------------------------Codificar-----------------------------------
void codificar_e_escrever(Bits *bits_saida, TabelaHash *tabela, unsigned char *texto) {
    int i = 0;
    while (texto[i] != '\0') {
        char *codigo = tabela[texto[i]].codigo_huffman;
        int j = 0;
        while (codigo[j] != '\0') {
            Bits_adiciona_bit(bits_saida, codigo[j] - '0'); // Converte o caractere '0' ou '1' em um número inteiro
            j++;
        }
        i++;
    }
    Bits_descarrega(bits_saida); // Escreve qualquer bit restante no buffer
}


// -------------------------Tabela-de-frequencia-----------------------------------

void inicializa_tabela_frequencia(unsigned int tabela[])
{
    for (int i = 0; i < MAX; i++)
        tabela[i] = 0;
}

void preenche_tabela_frequencia(unsigned char texto[], unsigned int tabela[])
{
    
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



int main(int argc, char *argv[]) {
    unsigned char linha[MAX];
    unsigned int tabela_frequencia[MAX];

    if (argc != 3) {
        printf("Uso: %s <nome_do_arquivo_entrada> <nome_do_arquivo_saida>\n", argv[0]);
        return 1;
    }

    FILE *arquivo_entrada = fopen(argv[1], "r");
    if (arquivo_entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    fgets(linha, MAX, arquivo_entrada);
    fclose(arquivo_entrada);

    inicializa_tabela_frequencia(tabela_frequencia);
    preenche_tabela_frequencia(linha, tabela_frequencia);

    unsigned char letra[MAX];
    int frequencia[MAX];
    int contador = 0;

    for (int i = 0; i < MAX; i++) {
        if (tabela_frequencia[i] > 0) {
            letra[contador] = i;
            frequencia[contador] = tabela_frequencia[i];
            contador++;
        }
    }

    No *raiz = montar_arvore(letra, frequencia, contador);
    TabelaHash tabela_huffman[MAX];
    for (int i = 0; i < MAX; i++) {
        tabela_huffman[i].codigo_huffman = NULL;
    }
    preenche_tabela_huffman(raiz, tabela_huffman);

    FILE *arquivo_saida = fopen(argv[2], "wb"); // Abre o arquivo de saída em modo binário
    if (arquivo_saida == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return 1;
    }

    Bits bits_saida;
    Bits_init(&bits_saida, arquivo_saida);

    codificar_e_escrever(&bits_saida, tabela_huffman, linha);

    fclose(arquivo_saida);

    printf("Texto codificado e escrito no arquivo de saída.\n");

    return 0;
}