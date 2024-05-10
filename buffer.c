#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Byte com buffer para leitura ou escrita bit-a-bit em arquivo. No
   modo escrita, quando o buffer completa 8 bits, escreve 1 byte no
   arquivo. No modo leitura, quando o buffer fica vazio, lê 1 byte do
   arquivo e preenche o buffer. Utilize cada objeto desta classe apenas
   para ler ou apenas para escrever, nunca os dois (seu comportamento
   seria indefinido) */

typedef struct {
    FILE *file;   // Ponteiro para o arquivo sendo lido/escrito
    uint8_t b[8]; // Buffer com bits de um byte
    uint8_t n;    // Quantidade de posições ocupadas no vetor acima
} Bits;

Bits* createBits(FILE *file) {
    Bits *bits = malloc(sizeof(Bits));
    bits->file = file;
    bits->n = 0;
    return bits;
}

uint8_t ocupados(Bits *bits) {
    return bits->n;
}

uint8_t livres(Bits *bits) {
    return 8 - bits->n;
}

// Funções do modo escrita
void adiciona_bit(Bits *bits, uint8_t bit) {
    bits->b[bits->n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

    if (bits->n == 8)
        descarrega(bits);
}

void descarrega(Bits *bits) {
    // Completa o byte com 0s para preencher os 8 bits, caso n < 8
    while (bits->n < 8) {
        bits->b[bits->n++] = 0;
    }
    // Escreve o byte no arquivo
    fwrite(bits->b, sizeof(uint8_t), 1, bits->file);
    // Reinicia o contador de bits
    bits->n = 0;
}

// Funções do modo leitura
uint8_t obtem_bit(Bits *bits) {
    // Caso n == 0, lê 1 byte do arquivo e coloca no buffer
    if (bits->n == 0) {
        if (fread(bits->b, sizeof(uint8_t), 1, bits->file) == 0) {
            return 2; // Indica que não há mais bytes para serem lidos do arquivo
        }
        bits->n = 8;
    }
    // Devolve um bit (0 ou 1) e decrementa n
    return bits->b[--bits->n];
}

int main() {
    FILE *original, *copia;
    original = fopen("original.txt", "rb");
    copia = fopen("copia.txt", "wb");

    Bits *in = createBits(original);
    Bits *out = createBits(copia);
    uint8_t bit;
  
    while ((bit = obtem_bit(in)) != 2) {
        putchar('0' + bit);
        adiciona_bit(out, bit);
    }
    putchar('\n');
    descarrega(out);

    fclose(original);
    fclose(copia);
    free(in);
    free(out);
  
    return 0;
}
