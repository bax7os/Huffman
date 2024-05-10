#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256 // ASCII

// -------------------------Min-Heap-----------------------------------
typedef struct No
{
    unsigned char letra;
    int frequencia;
    struct No*esquerdo;
    struct No*direito;

} No;

No* cria_no(unsigned char letra, int frequencia){
    No* newno = (No*)malloc(sizeof(No));
    newno->letra = letra;
    newno->frequencia = frequencia;
    newno->direito = NULL;
    newno->esquerdo = NULL;
    return newno;
}

void insere(No** minHeap, int *tamanho, No* newno)
{
    int i = ++(*tamanho);
    while(i>1 && newno->frequencia<minHeap[i/2]->frequencia){
        minHeap[i]=minHeap[i/2];
        i=i/2;
    }
    minHeap[i]=newno;
}

No* extrai_Min(No** minheap, int *tamanho){
    No* menor_no = minheap[1];
    No* ultimo_no = minheap[(*tamanho)--];
    int i = 1;
    int filho = 2*i; 
    while(i*2 <= *tamanho){
        filho = 2*i;
        if(filho != *tamanho && minheap[filho+1]->frequencia < minheap[filho]->frequencia){
            filho++;
        }
        if(ultimo_no->frequencia > minheap[filho]->frequencia){
            minheap[i] = minheap[filho];
        }else{
            break;
        }
        i = filho;
    }
    minheap[i] = ultimo_no;
    return menor_no;
}



// -------------------------Árvore-----------------------------------
No* montar_arvore(unsigned char letra[], int frequencia[], int n){
    No* minheap[n+1];
    int tamanho = 0;
    for (int i = 0; i < n; i++){
        No* newno = cria_no(letra[i], frequencia[i]);
        insere(minheap, &tamanho, newno);
    }
    while (tamanho>1){
        No* filho_esquerdo = extrai_Min(minheap, &tamanho);
        No* filho_direito = extrai_Min(minheap, &tamanho);
        No* no_interno = cria_no('\0', filho_esquerdo->frequencia + filho_direito->frequencia);
        no_interno->esquerdo = filho_esquerdo;
        no_interno->direito = filho_direito;
        insere(minheap, &tamanho, no_interno); // Inserir o nó interno de volta ao heap mínimo
    }
    return extrai_Min(minheap, &tamanho);
}

void imprime_arvore(No* raiz) {
    if (raiz != NULL) {
        printf("%c:%d\n", raiz->letra, raiz->frequencia);
        imprime_arvore(raiz->esquerdo);
        imprime_arvore(raiz->direito);
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



    // Insere os elementos na min-heap
    unsigned char letra[MAX];
    int frequencia[MAX];
    int contador = 0;

    for (int i = 0; i < MAX; i++)
    {
        if (tabela_frequencia[i] > 0){
        letra[contador] = i;
        frequencia[contador] = tabela_frequencia[i];            
        contador++;
        }
    }

    No* raiz = montar_arvore(letra, frequencia, contador);
    imprime_arvore(raiz);
    return 0;
}
