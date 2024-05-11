int main(int argc, char *argv[]) {
    unsigned char linha[MAX];
    unsigned int tabela_frequencia[MAX];

    if (argc != 2) {
        printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }

    FILE *arquivo;
    arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fgets(linha, MAX, arquivo);
    fclose(arquivo);

    inicializa_tabela_frequencia(tabela_frequencia);
    preenche_tabela_frequencia(linha, tabela_frequencia);

    unsigned char letra[MAX];
    int frequencia[MAX];
    int contador = 0;

    for (int i = 0; i < MAX; i++) {
        if (tabela_frequencia[i] > 0){
            letra[contador] = i;
            frequencia[contador] = tabela_frequencia[i];            
            contador++;
        }
    }

    No* raiz = montar_arvore(letra, frequencia, contador);
    TabelaHash tabela_huffman[MAX];
    for (int i = 0; i < MAX; i++) {
    tabela_huffman[i].codigo_huffman = NULL;
    }
    char *codificado;
    preenche_tabela_huffman(raiz, tabela_huffman);
    imprime_tabela_huffman(tabela_huffman);
    codificado = codificar(tabela_huffman, linha);
    printf("\n\tTexto codificado: %s\n", codificado);
    return 0;
}
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