#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

// ASCII

#define MAX 256

// NÓ DA ARVORE - NÓ HEAP

typedef struct No_arvore No_Heap;
struct No_arvore
{
    int freq;            // frequencia - prioridade do heap
    unsigned char letra; // letra
    No_Heap *esquerda,   // filho esquerdo arvore Huffman
        *direita;        // filho direito arvore Huffman
};

// heap
typedef struct heap
{
    int heap_size;
    No_Heap *C[MAX];
} heap;

// criando uma lista

void cria_heap(heap *h)
{
    h->heap_size = 0;
}

// nó pai

int pai(int i)
{
    return (i - 1) / 2;
}

// filho esquerdo

int filho_esquerdo(int i)
{
    return i * 2 + 1;
}

// filho direito

int filho_direito(int i)
{
    return i * 2 + 2;
}

// função de manutenção heap
void mantem_min_heap(heap *h, int i)
{
    int l, r, menor;
    No_Heap *t;

    l = filho_esquerdo(i);
    r = filho_direito(i);

    // acha o menor

    if (l < h->heap_size && h->C[l]->freq < h->C[i]->freq)
        menor = l;
    else
        menor = i;
    if (r < h->heap_size && h->C[r]->freq < h->C[menor]->freq)
        menor = r;

    // troca

    if (menor != i)
    {
        t = h->C[i];
        h->C[i] = h->C[menor];
        h->C[menor] = t;
        mantem_min_heap(h, menor);
    }
}

// insere o elemento de menor prioridade
void insercao_lista(heap *h, No_Heap *r)
{
    int i;

    h->heap_size++;
    i = h->heap_size - 1;

    while ((i > 0) && (h->C[pai(i)]->freq > r->freq))
    {
        h->C[i] = h->C[pai(i)];
        i = pai(i);
    }
    h->C[i] = r;
}

// extrai min
No_Heap *extrai_min(heap *h)
{
    No_Heap *r;

    if (h->heap_size == 0)
    {
        printf("heap cheia\n");
        exit(1);
    }

    r = h->C[0];
    h->C[0] = h->C[h->heap_size - 1];
    h->heap_size--;

    mantem_min_heap(h, 0);
    return r;
}

// le o arquivo
unsigned int pega_frequencias(FILE *f, unsigned int v[])
{
    int r, n;

    for (n = 0;; n++)
    {
        r = fgetc(f);
        if (feof(f))
            break;
        v[r]++;
    }
    return n;
}
No_Heap *novo_no(unsigned char letra)
{
    No_Heap *no = malloc(sizeof(No_Heap));
    no->letra = letra;
    no->esquerda = NULL;
    no->direita = NULL;
    return no;
}
// Arvore de Huffman

No_Heap *monta_arvore(unsigned int freq[])
{
    int i, n;
    No_Heap *x, *y, *z;
    heap h;

    cria_heap(&h);

    for (i = 0; i < MAX; i++)
    {
        /* its a leaf of the Huffman tree */
        if (freq[i] != 0)
        {
            x = malloc(sizeof(heap));
            x->esquerda = NULL;
            x->direita = NULL;
            x->freq = freq[i];
            x->letra = (char)i;
            insercao_lista(&h, x);
        }
    }

    n = h.heap_size - 1;

    for (i = 0; i < n; i++)
    {

        z = malloc(sizeof(heap));
        x = extrai_min(&h);
        y = extrai_min(&h);
        z->esquerda = x;
        z->direita = y;
        z->freq = x->freq + y->freq;
        insercao_lista(&h, z);
    }

    return extrai_min(&h);
}

void percorre_arvore(No_Heap *r, int nivel, char temp[], char *cod[])
{
    if ((r->esquerda == NULL) && (r->direita == NULL))
    {

        temp[nivel] = 0;
        cod[r->letra] = strdup(temp);
    }
    else
    {

        temp[nivel] = '0';

        percorre_arvore(r->esquerda, nivel + 1, temp, cod);

        temp[nivel] = '1';
        percorre_arvore(r->direita, nivel + 1, temp, cod);
    }
}

void percorre_arvore_pre_ordem(No_Heap *r, int *preordem, int *indice, FILE *f)
{
    if (r == NULL)
        return;

    // Se o nó é uma folha, adiciona '1' à string
    if (r->esquerda == NULL && r->direita == NULL)
    {
        preordem[(*indice)++] = 1;
        fputc(r->letra, f);
    }
    else
    {
        preordem[(*indice)++] = 0; // Se não, adiciona '0'
    }
    percorre_arvore_pre_ordem(r->esquerda, preordem, indice, f);
    percorre_arvore_pre_ordem(r->direita, preordem, indice, f);
}

int n_bits, byte_atual, n_bytes, bits_faltantes;

void bit_out(FILE *f, char b, int *bit_pos, unsigned char *byte)
{
    *byte <<= 1;

    if (b == '1')
        *byte |= 1;

    (*bit_pos)--;

    if (*bit_pos < 0)
    {
        fputc(*byte, f);
        n_bytes++;
        *bit_pos = 7;
        *byte = 0;
    }
}
void write_bits(FILE *out, int *preordem, int indice, int *bit_pos, unsigned char *byte) {
    *byte = 0;
    *bit_pos = 7; // Começa a escrever o bit mais significativo do byte

    // Escreve os bits do percurso pré-ordem
    for (int i = 0; i < indice; i++) {
        // Chama a função bit_out para cada bit
        bit_out(out, preordem[i] == 1 ? '1' : '0', bit_pos, byte);
    }
}


void codificar(FILE *in, FILE *out, char *cod[], int *bit_pos, unsigned char *byte)
{
    unsigned char letra;
    char *s;

    // Começa a partir do bit que faltou
    *bit_pos = bits_faltantes - 1;

    for (;;)
    {
        letra = fgetc(in);
        if (feof(in))
            break;

        for (s = cod[letra]; *s; s++)
            bit_out(out, *s, bit_pos, byte);
    }

    // Se houver bits restantes no último byte, preencha-os com 0s e escreva-o
    while (*bit_pos != 7)
        bit_out(out, '0', bit_pos, byte);
}

void libera_arvore(No_Heap *raiz) {
    if (raiz == NULL)
        return;

    libera_arvore(raiz->esquerda);
    libera_arvore(raiz->direita);
    free(raiz);
}
void imprime_arvore(No_Heap *raiz, int nivel)
{
    if (raiz == NULL)
        return;

    imprime_arvore(raiz->direita, nivel + 1);

    for (int i = 0; i < nivel; i++)
        printf("   ");

    if (raiz->letra)
        printf("[%c]\n", raiz->letra);
    else
        printf("[freq=%d]\n", raiz->freq);

    imprime_arvore(raiz->esquerda, nivel + 1);
}

int main(int argc, char *argv[])
{
    FILE *f, *g;
    No_Heap *r, *raiz;
    unsigned int n, tabela_freq[MAX];
    int indice = 0, preordem[MAX];
    int bit_pos = 7;
    unsigned char byte;
    uint16_t K = 0;
    uint32_t T = 0;
    char *cod[MAX], codigo[MAX], nome_arquivo[100];

    if (argc != 4)
    {
        fprintf(stderr, "Use: %s <c/d> <inputfile> <outputfile>\n", argv[0]);
        exit(1);
    }

    memset(tabela_freq, 0, sizeof(tabela_freq));

    f = fopen(argv[2], "r");
    if (!f)
    {
        perror(argv[2]);
        exit(1);
    }

    if (argv[1][0] == 'c'){
        n = pega_frequencias(f, tabela_freq);
        fclose(f);

        // Atualiza K com o número de caracteres únicos
        for (int i = 0; i < MAX; i++)
        {
            if (tabela_freq[i] != 0)
            {
                K++;
            }
        }

        // Atualiza T com o tamanho do texto original
        T = n;
        r = monta_arvore(tabela_freq);
        percorre_arvore(r, 0, codigo, cod);

        g = fopen(argv[3], "w");
        if (!g)
        {
            perror(argv[3]);
            exit(1);
        }

        fwrite(&K, sizeof(K), 1, g);
        fwrite(&T, sizeof(T), 1, g);

        // Escreve os bits do percurso pré-ordem no arquivo
        percorre_arvore_pre_ordem(r, preordem, &indice, g);
       
        write_bits(g, preordem, indice, &bit_pos, &byte);

        for (int j = 0; j < indice; j++)
        {
            printf("%d", preordem[j]);
        }
        
        // Escreve o texto compactado no arquivo
        f = fopen(argv[2], "r");
        if (!f)
        {
            perror(argv[2]);
            exit(1);
        }

    // Continue a escrita no arquivo onde write_bits parou
    codificar(f, g, cod, &bit_pos, &byte);

        fclose(f);
        fclose(g);

        printf("%s is %0.2f%% of %s\n",
               argv[3], (float)n_bytes / (float)n, argv[2]);
    libera_arvore(r);

    }
    else if (argv[1][0] == 'd') {
 
    return 0;
}
}