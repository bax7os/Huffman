#include <stdio.h>
#include <stdint.h>

typedef struct Bits {
    FILE *file;   // Ponteiro para o arquivo sendo lido/escrito
    uint8_t b[8]; // Buffer com bits de um byte
    uint8_t n;    // Quantidade de posições ocupadas no vetor acima
} Bits;

// Construtor padrão com o arquivo que será lido ou escrito
void Bits_init(Bits *bits, FILE *file) {
    bits->file = file;
    bits->n = 0;
}

void Bits_descarrega(Bits *bits) { // Força a escrita do byte no buffer (completa com 0s, caso necessário)
    if (bits->n == 0)
        return;
    for (int i = bits->n; i < 8; i++)
        bits->b[i] = 0;
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
        byte |= bits->b[i] << (7 - i);
    fwrite(&byte, sizeof(byte), 1, bits->file);
    bits->n = 0;
}
// Funções do modo escrita
void Bits_adiciona_bit(Bits *bits, uint8_t bit) { // Adiciona um bit 0 ou 1 ao buffer (escreve byte no arquivo se encher)
    bits->b[bits->n++] = bit != 0; // para usar 0/1 mesmo se bit > 1
    if (bits->n == 8)
        Bits_descarrega(bits);
}


uint8_t Bits_obtem_bit(Bits *bits) {
    if (bits->n == 0) {
        uint8_t byte;
        if (fread(&byte, sizeof(byte), 1, bits->file) != 1)
            return 2; // Não há mais bits para ler
        for (int i = 0; i < 8; i++)
            bits->b[i] = (byte >> (7 - i)) & 1;
        bits->n = 8;
    }
    return bits->b[--bits->n];
}
