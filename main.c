#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char **matriz;
} Jogo;

typedef struct {
    int vitoriaX;
    int vitoria0;
    int rodadas;
} Estatisticas;

void inicializarMatriz(Jogo *jogo) {
    jogo->matriz = (char **)malloc(3 * sizeof(char *));
    for (int l = 0; l < 3; l++) {
        jogo->matriz[l] = (char *)malloc(3 * sizeof(char));
        for (int c = 0; c < 3; c++)
            jogo->matriz[l][c] = ' ';
    }
}

void liberarMatriz(Jogo *jogo) {
    for (int l = 0; l < 3; l++) {
        free(jogo->matriz[l]);
    }
    free(jogo->matriz);
}

void imprimir(Jogo *jogo) {
    printf("\n\n\t 0   1   2\n\n");
    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            if (c == 0)
                printf("\t");
            printf(" %c ", jogo->matriz[l][c]);
            if (c < 2)
                printf("|");
            if (c == 2)
                printf("   %d", l);
        }
        printf("\n");
        if (l < 2)
            printf("\t-----------\n");
    }
}

int ganhouPorLinha(Jogo *jogo, int l, char c) {
    return (jogo->matriz[l][0] == c && jogo->matriz[l][1] == c && jogo->matriz[l][2] == c);
}

int ganhouPorLinhas(Jogo *jogo, char c) {
    int ganhou = 0;
    for (int l = 0; l < 3; l++) {
        ganhou += ganhouPorLinha(jogo, l, c);
    }
    return ganhou;
}

int ganhouPorColuna(Jogo *jogo, int c, char j) {
    return (jogo->matriz[0][c] == j && jogo->matriz[1][c] == j && jogo->matriz[2][c] == j);
}

int ganhouPorColunas(Jogo *jogo, char j) {
    int ganhou = 0;
    for (int c = 0; c < 3; c++) {
        ganhou += ganhouPorColuna(jogo, c, j);
    }
    return ganhou;
}

int ganhouPorDiagPrin(Jogo *jogo, char c) {
    return (jogo->matriz[0][0] == c && jogo->matriz[1][1] == c && jogo->matriz[2][2] == c);
}

int ganhouPorDiagSec(Jogo *jogo, char c) {
    return (jogo->matriz[0][2] == c && jogo->matriz[1][1] == c && jogo->matriz[2][0] == c);
}

int ehValida(Jogo *jogo, int l, int c) {
    return (l >= 0 && l < 3 && c >= 0 && c < 3 && jogo->matriz[l][c] == ' ');
}

void lerCoordenadas(Jogo *jogo, char j) {
    int linha, coluna;

    printf("Digite linha e coluna: ");
    scanf("%d%d", &linha, &coluna);

    while (ehValida(jogo, linha, coluna) == 0) {
        printf("Coordenadas invalidas! Digite outra linha e coluna: ");
        scanf("%d%d", &linha, &coluna);
    }
    jogo->matriz[linha][coluna] = j;
}

int quantVazias(Jogo *jogo) {
    int quantidade = 0;

    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++)
            if (jogo->matriz[l][c] == ' ')
                quantidade++;
    }
    return quantidade;
}

void carregarFase(Jogo *jogo, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }
    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            fscanf(arquivo, " %c", &jogo->matriz[l][c]);
        }
    }
    fclose(arquivo);
}

void salvarEstatisticas(Estatisticas *estatisticas) {
    FILE *arquivo = fopen("estatisticas.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas\n");
        return;
    }
    fwrite(estatisticas, sizeof(Estatisticas), 1, arquivo);
    fclose(arquivo);
}

void carregarEstatisticas(Estatisticas *estatisticas) {
    FILE *arquivo = fopen("estatisticas.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas\n");
        return;
    }
    fread(estatisticas, sizeof(Estatisticas), 1, arquivo);
    fclose(arquivo);
}

void exibirMenuPrincipal() {
    printf("=== Menu Principal ===\n");
    printf("1. Iniciar Jogo\n");
    printf("2. Estatísticas\n");
    printf("3. Sair\n");
}

void exibirEstatisticas(Estatisticas *estatisticas) {
    printf("\n=== Estatísticas ===\n");
    printf("Vitórias Jogador 1 (X): %d\n", estatisticas->vitoriaX);
    printf("Vitórias Jogador 2 (0): %d\n", estatisticas->vitoria0);
    printf("Rodadas Jogadas: %d\n", estatisticas->rodadas);
}

void jogar() {
    Jogo jogo;
    Estatisticas estatisticas = {0, 0, 0};
    char jogador1 = 'X', jogador2 = '0';
    int jogador = 1;
    int faseAtual = 1;

    carregarEstatisticas(&estatisticas);

    do {
        inicializarMatriz(&jogo);
        char nomeArquivo[20];
        sprintf(nomeArquivo, "fase%d.txt", faseAtual);
        carregarFase(&jogo, nomeArquivo); // Carrega a fase do arquivo

        do {
            imprimir(&jogo);
            if (jogador == 1) {
                lerCoordenadas(&jogo, jogador1);
                jogador++;
                estatisticas.vitoriaX += ganhouPorLinhas(&jogo, jogador1);
                estatisticas.vitoriaX += ganhouPorColunas(&jogo, jogador1);
                estatisticas.vitoriaX += ganhouPorDiagPrin(&jogo, jogador1);
                estatisticas.vitoriaX += ganhouPorDiagSec(&jogo, jogador1);
            } else {
                lerCoordenadas(&jogo, jogador2);
                jogador = 1;
                estatisticas.vitoria0 += ganhouPorLinhas(&jogo, jogador2);
                estatisticas.vitoria0 += ganhouPorColunas(&jogo, jogador2);
                estatisticas.vitoria0 += ganhouPorDiagPrin(&jogo, jogador2);
                estatisticas.vitoria0 += ganhouPorDiagSec(&jogo, jogador2);
            }
        } while (estatisticas.vitoriaX == 0 && estatisticas.vitoria0 == 0 && quantVazias(&jogo) > 0);

        imprimir(&jogo);

        if (estatisticas.vitoria0 == 1)
            printf("\nParabens Jogador 2. Voce venceu!!!\n");
        else if (estatisticas.vitoriaX == 1)
            printf("\nParabens Jogador 1. Voce venceu!!!\n");
        else
            printf("\nQue pena. Perderam!!!\n");

        estatisticas.rodadas++;
        salvarEstatisticas(&estatisticas);
        liberarMatriz(&jogo);
        faseAtual++;
    } while (faseAtual <= 5); // Limita a 5 fases
}

int main() {
    int opcao;

    do {
        exibirMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar();
                break;
            case 2: {
                Estatisticas estatisticas;
                carregarEstatisticas(&estatisticas);
                exibirEstatisticas(&estatisticas);
                break;
            }
            case 3:
                printf("\nSaindo do jogo. Até mais!\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;
}