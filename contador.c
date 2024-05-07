#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000 // Tamanho máximo da palavra

int main() {
    char palavra[MAX_LENGTH];
    int contagem[26] = {0}; // Array para armazenar a contagem de cada letra do alfabeto

    // Lê a palavra
    printf("Digite uma palavra: ");
    scanf("%s", palavra);

    // Conta quantas vezes cada letra aparece na palavra
    for (int i = 0; i < strlen(palavra); i++) {
        if (palavra[i] >= 'A' && palavra[i] <= 'Z') {
            int index = palavra[i] - 'A';
            contagem[index]++;
        } else if (palavra[i] >= 'a' && palavra[i] <= 'z') {
            int index = palavra[i] - 'a';
            contagem[index]++;
        }
    }

    // Imprime a contagem de cada letra
    printf("Contagem de letras na palavra:\n");
    for (int i = 0; i < 26; i++) {
        if (contagem[i] > 0) {
            printf("%c: %d\n", 'A' + i, contagem[i]);
        }
    }

    return 0;
}
