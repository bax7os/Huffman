#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int *lista;
    int tamanho;
    int capacidade;
} minHeap;





//Inicializa o minHeap
minHeap* constroi_minHeap(int capacidade) {
    minHeap *heap = (minHeap*)malloc(sizeof(minHeap));
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    heap->lista = (int*)malloc(capacidade * sizeof(int));
    return heap;
}


void libera_minHeap(minHeap *minHeap) {
    free(minHeap->lista);
    free(minHeap);
}

void troca(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void desce(minHeap *minHeap, int i) {
    int esquerdo = 2 * i + 1;
    int direito = 2 * i + 2;
    int menor = i;

    if (esquerdo < minHeap->tamanho && minHeap->array[left] < minHeap->array[i])
        smallest = left;
    if (right < minHeap->size && minHeap->array[right] < minHeap->array[smallest])
        smallest = right;

    if (smallest != i) {
        swap(&minHeap->array[i], &minHeap->array[smallest]);
        minHeapifyDown(minHeap, smallest);
    }
}

void minHeapifyUp(minHeap *minHeap, int i) {
    int parent = (i - 1) / 2;

    while (i > 0 && minHeap->array[i] < minHeap->array[parent]) {
        swap(&minHeap->array[i], &minHeap->array[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

void insert(minHeap *minHeap, int key) {
    if (minHeap->size < minHeap->capacity) {
        minHeap->array[minHeap->size] = key;
        minHeap->size++;
        minHeapifyUp(minHeap, minHeap->size - 1);
    }
}

int extractMin(minHeap *minHeap) {
    if (minHeap->size == 0)
        return INT_MIN;

    int min = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapifyDown(minHeap, 0);

    return min;
}

int getMin(minHeap *minHeap) {
    if (minHeap->size == 0)
        return INT_MIN;

    return minHeap->array[0];
}

int main() {
    minHeap *minHeap = createminHeap(10);

    insert(minHeap, 3);
    insert(minHeap, 2);
    insert(minHeap, 1);
    insert(minHeap, 15);

    printf("Min minHeap: %d\n", extractMin(minHeap));
    printf("Min minHeap: %d\n", getMin(minHeap));

    destroyminHeap(minHeap);

    return 0;
}
