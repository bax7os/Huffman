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

No_Heap *extrai_min(heap *h)
{
    No_Heap *r;
    if (h->heap_size == 0)
    {
        fprintf(stderr, "Erro: tentando extrair elemento de heap vazio.\n");
        return NULL;
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

typedef struct Bits
{
    FILE *file;   // Ponteiro para o arquivo sendo lido/escrito
    uint8_t b[8]; // Buffer com bits de um byte
    uint8_t n;    // Quantidade de posições ocupadas no vetor acima
} Bits;

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
void write_bits(FILE *out, int *preordem, int indice, int *bit_pos, unsigned char *byte)
{
    *byte = 0;
    *bit_pos = 7; // Começa a escrever o bit mais significativo do byte

    // Escreve os bits do percurso pré-ordem
    for (int i = 0; i < indice; i++)
    {
        // Chama a função bit_out para cada bit
        bit_out(out, preordem[i] == 1 ? '1' : '0', bit_pos, byte);
    }
}

uint8_t Bits_obtem_bit(Bits *bits)
{
    if (bits->n == 0)
    {
        uint8_t byte;
        if (fread(&byte, sizeof(byte), 1, bits->file) != 1)
            return 2; // Não há mais bits para ler
        for (int i = 0; i < 8; i++){
            bits->b[i] = (byte >> (7 - i)) & 1;
            printf("%d", bits->b[i]);
        }
        bits->n = 8;
    }
    return bits->b[--bits->n];
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

void libera_arvore(No_Heap *raiz)
{
    if (raiz == NULL)
        return;

    libera_arvore(raiz->esquerda);
    libera_arvore(raiz->direita);
    free(raiz);
}
// Função auxiliar para impressão com indentação
// Função auxiliar para impressão com indentação
void imprime_preordem(No_Heap *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%c ", root->letra);
    imprime_preordem(root->esquerda);
    imprime_preordem(root->direita);
}

int letra = 0;
No_Heap *reconstroi_huffman(int *preordem, char *alfabeto, int *index, int n)
{
    if (*index >= n)
    {
        return NULL;
    }

    No_Heap *node = (No_Heap *)malloc(sizeof(No_Heap));
    if (preordem[*index] == 1)
    { // Folha
        node->letra = alfabeto[letra];
        printf("%c\n", node->letra);
        node->esquerda = NULL;
        node->direita = NULL;
        letra++;
    }
    else
    { // Nó interno
        node->letra = '+';
        (*index)++;
        node->esquerda = reconstroi_huffman(preordem, alfabeto, index, n);
        (*index)++;
        node->direita = reconstroi_huffman(preordem, alfabeto, index, n);
    }

    return node;
}
// Função para descompactar o texto
void descompactar_texto(FILE *f, FILE *g, No_Heap *raiz, uint32_t tamanho_texto, int *restante[], int h)
{
    No_Heap *atual = raiz;
    uint8_t byte;
    int bit;
    uint32_t contagem = 0;

    // Ler o arquivo compactado bit a bit
    while (contagem < tamanho_texto)
    {

        if (h != 0)
        {
            for (int i = 0; i < h; i++)
            {
                printf("1");
                    // Navega na árvore de Huffman
                if (*restante[i] == 1)
                    atual = atual->direita;
                else
                    atual = atual->esquerda;

                // Se é uma folha, escreve o caractere no arquivo de saída
                if (!atual->esquerda && !atual->direita)
                {
                    fputc(atual->letra, g);
                    atual = raiz;
                    contagem++;
                }
            }
            h = 0;
        }else
        {
           printf("2");
        byte = fgetc(f);
        for (bit = 7; bit >= 0 && contagem < tamanho_texto; bit--)
        {
            int bit_valor = (byte >> bit) & 1;
            printf("%d", bit_valor); // Imprime o bit atual

            // Navega na árvore de Huffman
            if (bit_valor)
                atual = atual->direita;
            else
                atual = atual->esquerda;

            // Se é uma folha, escreve o caractere no arquivo de saída
            if (!atual->esquerda && !atual->direita)
            {
                fputc(atual->letra, g);
                atual = raiz;
                contagem++;
            }
        }
        }
        
        


        printf("\n");
    }
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

    if (argv[1][0] == 'c')
    {
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
    else if (argv[1][0] == 'd')
    {
        Bits *b = malloc(sizeof(Bits));
        uint16_t letras[MAX];
        uint8_t valor;
        char alfabeto[MAX];
        // Lê K e T do cabeçalho

        fread(&K, sizeof(K), 1, f);
        fread(&T, sizeof(T), 1, f);

        if (b == NULL)
        {
            fprintf(stderr, "Falha ao alocar memória para Bits\n");
            return 1;
        }
        b->file = f;
        b->n = 0; // Inicializa o contador de bits

        // Posiciona o ponteiro do arquivo para começar a ler as letras do alfabeto
        fseek(f, sizeof(K) + sizeof(T), SEEK_SET);
     printf("%d %d\n", K, T);

        for (int i = 0; i < K; i++)
        {
            // Lê cada letra do alfabeto
            fread(&valor, sizeof(valor), 1, f);
            printf("%c\n", valor);
            alfabeto[i] = valor;
        }
        // Lê o primeiro byte após as letras do alfabeto
        // Bits_obtem_bit(b);
       // long pos = ftell(f);
        //printf("Posição atual do ponteiro do arquivo: %ld\n", pos);

        Bits_obtem_bit(b);
     

        int n_folhas = 0, j = 0, i = 0;
        int *restante = malloc(sizeof(int) * sizeof(b->b)); // N é o tamanho de b->b[]

        while (n_folhas < K)
        {
            if (b->b[j] == 1)
            {
                n_folhas++;
            }

            preordem[j] = b->b[j];
            j++;
        }

        // Guardar os valores restantes em outro vetor
        while (j < sizeof(b->b))
        {
            restante[i] = b->b[j];
            i++;
            j++;
        }

        for (int i = 0; i < j; i++)
        {
            printf("%d", preordem[i]);
        }

        int index = 0;
        n = sizeof(preordem) / sizeof(preordem[0]);

        raiz = reconstroi_huffman(preordem, alfabeto, &index, n);
        imprime_preordem(raiz);

        g = fopen(argv[3], "wb");
        if (!g)
        {
            perror(argv[3]);
            exit(1);
        }
        //pos = ftell(f);
        //printf("Posição atual do ponteiro do arquivo: %ld\n", pos);

        descompactar_texto(f, g, raiz, T, &restante, i);

        fclose(g);
        libera_arvore(raiz);
    }

    fclose(f);
    return 0;
}