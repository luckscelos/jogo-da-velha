#include <stdio.h>
#include <stdlib.h>

// Aqui estamos criando uma "caixa" que vai guardar o nosso tabuleiro (matriz)
typedef struct {
    char **matriz;
} Jogo;

// Aqui estamos criando uma outra "caixa" para guardar as estatísticas do jogo, como quantas vitórias o X e o 0 tiveram, e quantas rodadas o jogo já teve.
typedef struct {
    int vitoriaX;
    int vitoria0;
    int rodadas;
} Estatisticas;

// Função para criar o tabuleiro e colocar ele todo vazio (com espaços ' ').
void inicializarMatriz(Jogo *jogo) {
    // Aqui criamos o espaço para o tabuleiro (um tabuleiro de 3x3).
    jogo->matriz = (char **)malloc(3 * sizeof(char *));
    for (int l = 0; l < 3; l++) {
        jogo->matriz[l] = (char *)malloc(3 * sizeof(char));
        for (int c = 0; c < 3; c++)
            jogo->matriz[l][c] = ' ';  // Coloca espaço vazio em todas as casas.
    }
}

// Função para liberar o espaço da memória que usamos para o tabuleiro depois que o jogo terminar.
void liberarMatriz(Jogo *jogo) {
    for (int l = 0; l < 3; l++) {
        free(jogo->matriz[l]);  // Libera as linhas do tabuleiro.
    }
    free(jogo->matriz);  // Libera o tabuleiro todo.
}

// Função para mostrar o tabuleiro na tela.
void imprimir(Jogo *jogo) {
    printf("\n\n\t 0   1   2\n\n");
    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            if (c == 0)
                printf("\t");  // Para alinha a primeira coluna.
            printf(" %c ", jogo->matriz[l][c]);  // Mostra o que tem na posição (linha, coluna).
            if (c < 2)
                printf("|");  // Coloca uma linha para separar as colunas.
            if (c == 2)
                printf("   %d", l);  // Aqui mostramos o número da linha.
        }
        printf("\n");
        if (l < 2)
            printf("\t-----------\n");  // Linha separando as linhas do tabuleiro.
    }
}

// Verifica se um jogador ganhou em alguma linha.
int ganhouPorLinha(Jogo *jogo, int l, char c) {
    return (jogo->matriz[l][0] == c && jogo->matriz[l][1] == c && jogo->matriz[l][2] == c);
}

// Verifica se um jogador ganhou em alguma das 3 linhas.
int ganhouPorLinhas(Jogo *jogo, char c) {
    for (int l = 0; l < 3; l++) {
        if (ganhouPorLinha(jogo, l, c))
            return 1;  // Se ganhou em alguma linha, retorna "1" (verdadeiro).
    }
    return 0;  // Se não ganhou, retorna "0".
}

// Verifica se um jogador ganhou em alguma coluna.
int ganhouPorColuna(Jogo *jogo, int c, char j) {
    return (jogo->matriz[0][c] == j && jogo->matriz[1][c] == j && jogo->matriz[2][c] == j);
}

// Verifica se um jogador ganhou em alguma das 3 colunas.
int ganhouPorColunas(Jogo *jogo, char j) {
    for (int c = 0; c < 3; c++) {
        if (ganhouPorColuna(jogo, c, j))
            return 1;  // Se ganhou em alguma coluna, retorna "1".
    }
    return 0;  // Se não ganhou, retorna "0".
}

// Verifica se um jogador ganhou na diagonal principal (de cima a esquerda para baixo a direita).
int ganhouPorDiagPrin(Jogo *jogo, char c) {
    return (jogo->matriz[0][0] == c && jogo->matriz[1][1] == c && jogo->matriz[2][2] == c);
}

// Verifica se um jogador ganhou na diagonal secundária (de cima a direita para baixo a esquerda).
int ganhouPorDiagSec(Jogo *jogo, char c) {
    return (jogo->matriz[0][2] == c && jogo->matriz[1][1] == c && jogo->matriz[2][0] == c);
}

// Verifica se a posição que o jogador escolheu é válida (se está dentro do tabuleiro e se está vazia).
int ehValida(Jogo *jogo, int l, int c) {
    return (l >= 0 && l < 3 && c >= 0 && c < 3 && jogo->matriz[l][c] == ' ');
}

// Função para ler a linha e a coluna que o jogador escolheu, e verificar se é uma posição válida.
void lerCoordenadas(Jogo *jogo, char j) {
    int linha, coluna;

    printf("Digite a linha (0, 1, 2): ");
    scanf("%d", &linha);

    printf("Digite a coluna (0, 1, 2): ");
    scanf("%d", &coluna);

    // Enquanto a posição não for válida, pede novamente as coordenadas.
    while (!ehValida(jogo, linha, coluna)) {
        printf("Coordenadas invalidas! Digite outra linha e outra coluna.\n");
        
        printf("Digite a linha (0, 1, 2): ");
        scanf("%d", &linha);

        printf("Digite a coluna (0, 1, 2): ");
        scanf("%d", &coluna);
    }

    // Marca a posição escolhida com o símbolo do jogador (X ou 0).
    jogo->matriz[linha][coluna] = j;
}

// Conta quantas posições vazias ainda têm no tabuleiro.
int quantVazias(Jogo *jogo) {
    int quantidade = 0;

    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            if (jogo->matriz[l][c] == ' ')  // Se a posição estiver vazia, aumenta a quantidade.
                quantidade++;
        }
    }
    return quantidade;  // Retorna a quantidade de espaços vazios.
}

// Função para salvar as estatísticas do jogo (quantas vitórias e rodadas) em um arquivo.
void salvarEstatisticas(Estatisticas *estatisticas) {
    FILE *arquivo = fopen("estatisticas.bin", "wb");  // Abre o arquivo para escrever.
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas\n");
        return;
    }
    fwrite(estatisticas, sizeof(Estatisticas), 1, arquivo);  // Grava as estatísticas no arquivo.
    fclose(arquivo);  // Fecha o arquivo.
}

// Função para carregar as estatísticas salvas do arquivo.
void carregarEstatisticas(Estatisticas *estatisticas) {
    FILE *arquivo = fopen("estatisticas.bin", "rb");  // Abre o arquivo para ler.
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas\n");
        return;
    }
    fread(estatisticas, sizeof(Estatisticas), 1, arquivo);  // Lê as estatísticas do arquivo.
    fclose(arquivo);  // Fecha o arquivo.
}

// Função que mostra o menu principal onde o jogador pode escolher o que fazer.
void exibirMenuPrincipal() {
    printf("=== Menu Principal ===\n");
    printf("1. Iniciar Jogo\n");
    printf("2. Estatisticas\n");
    printf("3. Sair\n");
}

// Função para mostrar as estatísticas do jogo (quantas vitórias e rodadas).
void exibirEstatisticas(Estatisticas *estatisticas) {
    printf("\n=== Estatisticas ===\n");
    printf("Vitorias Jogador 1 (X): %d\n", estatisticas->vitoriaX);
    printf("Vitorias Jogador 2 (0): %d\n", estatisticas->vitoria0);
    printf("Rodadas Jogadas: %d\n", estatisticas->rodadas);
}

// Função que verifica se algum jogador ganhou o jogo.
int verificarVitoria(Jogo *jogo, char jogador) {
    return ganhouPorLinhas(jogo, jogador) || 
           ganhouPorColunas(jogo, jogador) || 
           ganhouPorDiagPrin(jogo, jogador) || 
           ganhouPorDiagSec(jogo, jogador);
}

// Função principal onde o jogo acontece, e os jogadores jogam um contra o outro.
void jogar() {
    Jogo jogo;
    Estatisticas estatisticas = {0, 0, 0};  // Inicializa as estatísticas.
    char jogador1 = 'X', jogador2 = '0';
    int jogador = 1;  // Começa com o jogador 1.
    int faseAtual = 1;  // Fase do jogo.

    carregarEstatisticas(&estatisticas);  // Carrega as estatísticas do arquivo.

    do {
        inicializarMatriz(&jogo);  // Cria o tabuleiro vazio.

        int venceu = 0;
        do {
            imprimir(&jogo);  // Mostra o tabuleiro para os jogadores.
            if (jogador == 1) {
                lerCoordenadas(&jogo, jogador1);  // Jogador 1 faz uma jogada.
                venceu = verificarVitoria(&jogo, jogador1);  // Verifica se o jogador 1 ganhou.
                if (venceu) {
                    estatisticas.vitoriaX++;  // Se ganhou, conta como vitória do X.
                    printf("\nParabens Jogador 1. Voce venceu!!!\n");
                    break;
                }
                jogador = 2;  // Passa a vez para o jogador 2.
            } else {
                lerCoordenadas(&jogo, jogador2);  // Jogador 2 faz uma jogada.
                venceu = verificarVitoria(&jogo, jogador2);  // Verifica se o jogador 2 ganhou.
                if (venceu) {
                    estatisticas.vitoria0++;  // Se ganhou, conta como vitória do O.
                    printf("\nParabens Jogador 2. Voce venceu!!!\n");
                    break;
                }
                jogador = 1;  // Passa a vez para o jogador 1.
            }
        } while (quantVazias(&jogo) > 0);  // Continua enquanto houver casas vazias.

        if (!venceu) {
            printf("\nEmpate! Ninguem venceu.\n");
        }

        estatisticas.rodadas++;  // Aumenta o número de rodadas jogadas.
        salvarEstatisticas(&estatisticas);  // Salva as estatísticas.
        liberarMatriz(&jogo);  // Libera o tabuleiro após o fim do jogo.
        faseAtual++;
    } while (faseAtual <= 5);  // Joga até 5 rodadas.
}

// Função principal onde o programa começa e o jogador escolhe o que fazer.
int main() {
    int opcao;

    do {
        exibirMenuPrincipal();  // Mostra o menu para o jogador escolher.
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);  // O jogador escolhe uma opção.

        switch (opcao) {
            case 1:
                jogar();  // Começa o jogo.
                break;
            case 2: {
                Estatisticas estatisticas;
                carregarEstatisticas(&estatisticas);  // Carrega as estatísticas.
                exibirEstatisticas(&estatisticas);  // Mostra as estatísticas.
                break;
            }
            case 3:
                printf("\nSaindo do jogo. Ate mais!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 3);  // O programa continua até o jogador escolher sair.

    return 0;  // Finaliza o programa.
}
