#include <stdio.h>
#include <stdlib.h>

const char AGUA = '~';
const char TIRO_ERRADO = '*';
const char TIRO_CERTO = 'X';
int rodadas = 0;

typedef struct {
    int id;                
    char tabuleiro[10][10];
    int vidas;
    int acertos;
    int erros;
} Jogador;

//sinalizacao das funcoes criadas
void pausar();
void inicializar_tabuleiro(char tab[10][10]);
void imprimir_tabuleiro(char tab[10][10], int esconder);
int validar(char tab[10][10], int linha, int coluna, int tam, int direcao);
void posicionar_navios(Jogador *j);
void salvar_jogo(Jogador j1, Jogador j2, int vez);
int carregar_jogo(Jogador *j1, Jogador *j2);
void relatorio_final(Jogador j1, Jogador j2);
void batalha(Jogador *j1, Jogador *j2, int vez_inicial);
int navio_afundado(char tab[10][10], char tipo);

int main() {
    Jogador jogador1, jogador2;
    jogador1.id = 1; 
    jogador2.id = 2;
    int opcao;

    do {
        printf("----> Batalha Naval <----\n");
        printf("1.Novo Jogo\n");
        printf("2.Continuar Jogo\n");
        printf("3.Instrucoes\n");
        printf("4.Sair do jogo\n");
        printf("Digite uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: //p opcao novo jogo
                inicializar_tabuleiro(jogador1.tabuleiro);
                inicializar_tabuleiro(jogador2.tabuleiro);
                
                jogador1.vidas = 9; 
                jogador1.acertos = 0; 
                jogador1.erros = 0;
                jogador2.vidas = 9; 
                jogador2.acertos = 0;
                jogador2.erros = 0;

                posicionar_navios(&jogador1);
                system("cls"); //funciona como um control L no terminal
                posicionar_navios(&jogador2);
                
                printf("Navios posicionados com sucesso!\n");
                batalha(&jogador1, &jogador2, 1); //vez inicial do jogador 1 
                break;

            case 2: //p opcao continuar jogo
                printf("Procurando progresso salvo\n");
                int vez = carregar_jogo(&jogador1, &jogador2);
                
                if (vez > 0) {
                    printf("Jogo recuperado! Vez do Jogador %d\n", vez);
                    pausar();
                    batalha(&jogador1, &jogador2, vez);
                } else {
                    printf("Nenhum progresso encontrado.\n");
                    pausar();
                }
                break;

            case 3: 
                system("cls");
                printf("INSTRUÇõES:\n");
                printf("-Tabuleiro 10x10.\n");
                printf("-3 Navios = 9 vidas no total.\n");
                printf("-Digite -1 -1 para salvar e sair.\n");
                pausar();
                break;

            case 4: 
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opcao inválida!\n");
                pausar(); 
        }
    } while (opcao != 4);

    return 0;
}

//funcoes
void pausar() {
    printf("\nAperte enter para continuar");
    getchar();  // esse getchar serve para pegar o enter(\n) que sobrou do scanf anterior
    getchar();  //esse getchar para travar a tela e esperar voce apertar enter
}

void inicializar_tabuleiro(char tab[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            tab[i][j] = AGUA;
        }
    }
}

void imprimir_tabuleiro(char tab[10][10], int esconder) {
    printf("\n  0 1 2 3 4 5 6 7 8 9\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", i); 

        for (int j = 0; j < 10; j++) {
            if (esconder == 1 && tab[i][j] != AGUA && tab[i][j] != TIRO_CERTO && tab[i][j] != TIRO_ERRADO) {
                printf("%c ", AGUA); //p esconder o tabuleiro inimigo, mostra agua no lugar do navio
            } else {
                printf("%c ", tab[i][j]);
            }
        }
        printf("\n");
    }
}

void posicionar_navios(Jogador *j) {
    int tamanhos[3] = {4, 3, 2}; 
    char *nomes[3] = {"Navio-tanque", "Submarino", "Bote"};
    char simbolos_navios[3] = {'T', 'S', 'B'};
    int linha, coluna, direcao, valida; 

    for (int i = 0; i < 3; i++) {
        do { 
            system("cls");
            printf("Vez do jogador %d colocar seus navios:\n", j->id); 
            imprimir_tabuleiro(j->tabuleiro, 0); 
            
            printf("\nNavio: %s - Tamanho: %d\n", nomes[i], tamanhos[i]);
            printf("Escolha linha (0-9) e coluna (0-9): ");
            scanf("%d %d", &linha, &coluna);
            printf("Escolha a direcao (1-Horizontal ou 2-Vertical): ");
            scanf("%d", &direcao);

             if (direcao != 1 && direcao != 2) {
                printf("Direcao invalida! Digite 1 ou 2\n");
                valida = 0;
                pausar();
                } else {
                    valida = validar(j->tabuleiro, linha, coluna, tamanhos[i], direcao);
                    if (valida == 0) {
                    printf("\nPosicao Invalida! Tente novamente!\n"); 
                    pausar(); 
                    }
                }
        } while (valida == 0);

        // grava na matriz
        for (int k = 0; k < tamanhos[i]; k++) {
            if (direcao == 1){
                j->tabuleiro[linha][coluna + k] = simbolos_navios[i];
            }
            else{
                j->tabuleiro[linha + k][coluna] = simbolos_navios[i];
            }
        }
    }
}

int validar(char tab[10][10], int linha, int coluna, int tam, int direcao) {
    if (direcao == 1 && coluna + tam > 10) return 0; 
    if (direcao == 2 && linha + tam > 10) return 0; 

    for (int i = 0; i < tam; i++) {
        if (direcao == 1) {
            if (tab[linha][coluna + i] != AGUA) return 0;
        } else {
            if (tab[linha + i][coluna] != AGUA) return 0;
        }
    }
    return 1; //pos valida
}

void batalha(Jogador *j1, Jogador *j2, int vez_inicial) {
    int vez = vez_inicial;
    int linha, coluna, jogando = 1;

    while (jogando == 1) {
        system("cls");

        if (vez == 1) {
            printf("-Vez do Jogador 1:\n");
            printf("Vidas do inimigo: %d\n", j2->vidas);

            printf("-Tabuleiro Inimigo-\n");
            imprimir_tabuleiro(j2->tabuleiro, 1);

            printf("\n-Seu Tabuleiro-\n");
            imprimir_tabuleiro(j1->tabuleiro, 0);
        } else {
            printf("-Vez do Jogador 2:\n");
            printf("Vidas do inimigo: %d\n", j1->vidas);

            printf("-Tabuleiro Inimigo-\n");
            imprimir_tabuleiro(j1->tabuleiro, 1);

            printf("\n-Seu Tabuleiro-\n");
            imprimir_tabuleiro(j2->tabuleiro, 0);
        }

        printf("\nDigite a linha e coluna que deseja atacar (ou -1 -1 para salvar e voltar ao menu): ");
        scanf("%d %d", &linha, &coluna);

        //ao digitar o codigo, salva o jogo
        if (linha == -1 && coluna == -1) {
            salvar_jogo(*j1, *j2, vez);
            return;
        }

        if (linha < 0 || linha > 9 || coluna < 0 || coluna > 9) {
            printf("Coordenada inválida!\n");
            pausar();
            continue;
        }
        char tipo_atingido;
        if (vez == 1) {
            tipo_atingido = j2->tabuleiro[linha][coluna];
        } else {
            tipo_atingido = j1->tabuleiro[linha][coluna];
        }

        if (tipo_atingido == TIRO_CERTO || tipo_atingido == TIRO_ERRADO) {
            printf("Coordenada já atirada!\n");
            pausar();
            continue;
        }

        if (tipo_atingido != AGUA) {
            printf("\nTiro certo!\n");
            if (vez == 1) {
                j2->tabuleiro[linha][coluna] = TIRO_CERTO;
                j1->acertos++;
                j2->vidas--;
                if (navio_afundado(j2->tabuleiro, tipo_atingido)) {
                    printf("NAVIO AFUNDADO!\n");
                }
            } else {
                j1->tabuleiro[linha][coluna] = TIRO_CERTO;
                j2->acertos++;
                j1->vidas--;
                if (navio_afundado(j1->tabuleiro, tipo_atingido)) {
                    printf("NAVIO AFUNDADO!\n");
                }
            }
            
        } else {
            printf("\nTiro errado!\n");
            if (vez == 1) {
                j2->tabuleiro[linha][coluna] = TIRO_ERRADO;
                j1->erros++;
            } else {
                j1->tabuleiro[linha][coluna] = TIRO_ERRADO;
                j2->erros++;
            }
        }        
        rodadas++;
        pausar();
        //verificar vitoria
        if (j1->vidas <= 0) {
            system("cls");
            printf("VITORIA DO JOGADOR 2!\n");
            relatorio_final(*j1, *j2);
            jogando = 0;
            pausar();
        } 
        else if (j2->vidas <= 0) {
            system("cls");
            printf("VITORIA DO JOGADOR 1!\n");
            relatorio_final(*j1, *j2);
            jogando = 0;
        } 
        else { //troca de vez
            if (vez == 1)
                vez = 2;
            else
                vez = 1;
        }
    }
}

int navio_afundado(char tab[10][10], char tipo) {
    int i, j;
    for(i = 0; i < 10; i++) {
        for(j = 0; j < 10; j++) {
            if(tab[i][j] == tipo) {
                return 0; // Ainda foi encontrada uma celula de navio, nao afundou
            }
        }
    }
    return 1; // Não foi encontrada, afundou
}

void salvar_jogo(Jogador j1, Jogador j2, int vez) {
    FILE *arquivo = fopen("progresso_salvo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    fprintf(arquivo, "%d\n", vez);

    fprintf(arquivo, "%d %d %d %d\n", j1.id, j1.acertos, j1.erros, j1.vidas);
    int i, j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++) {
            fprintf(arquivo, "%c ", j1.tabuleiro[i][j]);
        }
    }
    
    fprintf(arquivo, "%d %d %d %d\n", j2.id, j2.acertos, j2.erros, j2.vidas);
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            fprintf(arquivo, "%c ", j2.tabuleiro[i][j]);
        }
    }
    fclose(arquivo);
    printf("Salvo com sucesso!\n");
}

int carregar_jogo(Jogador *j1, Jogador *j2) {
    FILE *arquivo = fopen("progresso_salvo.txt", "r");
    if (arquivo == NULL) return 0;

    int vez;
    fscanf(arquivo, "%d", &vez);

    fscanf(arquivo, "%d %d %d %d", &j1->id, &j1->acertos, &j1->erros, &j1->vidas);
    int i, j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            fscanf(arquivo, " %c", &j1->tabuleiro[i][j]);
        }
    }
        
    fscanf(arquivo, "%d %d %d %d", &j2->id, &j2->acertos, &j2->erros, &j2->vidas);
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            fscanf(arquivo, " %c", &j2->tabuleiro[i][j]);
        }
    }
    fclose(arquivo);
    return vez; 
}

void relatorio_final(Jogador j1, Jogador j2) {
    FILE *arquivo = fopen("historico.txt", "a");
    if (arquivo == NULL) {
        return;
    }
    fprintf(arquivo, "Fim da Partida\n");
    fprintf(arquivo, "Rodadas: %d\n", rodadas);
    fprintf(arquivo,"Jogador %d - Vidas: %d - Acertos: %d - Erros: %d\n", j1.id, j1.vidas, j1.acertos, j1.erros);
    fprintf(arquivo,"Jogador %d - Vidas: %d - Acertos: %d - Erros: %d\n", j2.id, j2.vidas, j2.acertos, j2.erros);
    if (j1.vidas > j2.vidas)
        fprintf(arquivo, "Vencedor: Jogador %d\n\n", j1.id);
    else
        fprintf(arquivo, "Vencedor: Jogador %d\n\n", j2.id);
    fclose(arquivo);
}