#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "jogo.h"

#define TOTAL_CARTAS 39

char modo = 'N';

int main() {
    srand(time(NULL));
    PilhaCarta *listaCartas = lerArq("cartas.txt");
    int opcao;
    int aux = 1;
    while (aux == 1) {
        Jogo *jogo = inicializa_estruturas(listaCartas);
        printf("Escolha o modo de jogo:\n");
        printf("1 - Manual\n");
        printf("2 - Automático\n");
        printf("3 - Encerrar\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                modo = 'M';
                partidas(jogo);
                break;
            case 2:
                modo = 'A';
                partidas(jogo);
                break;
            case 3:
                printf("Jogo Encerrado!");
                aux = 0;
                break;
        }
    }
}

Jogo *inicializa_estruturas(PilhaCarta *listCartas) {
    Jogador *jogadorA1 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorA1->nome, "A1");
    jogadorA1->cartas = NULL;


    Jogador *jogadorB1 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorB1->nome, "B1");
    jogadorB1->cartas = NULL;


    Jogador *jogadorA2 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorA2->nome, "A2");
    jogadorA2->cartas = NULL;


    Jogador *jogadorB2 = (Jogador *) malloc(sizeof(Jogador));
    strcpy(jogadorB2->nome, "B2");
    jogadorB2->cartas = NULL;

    jogadorA1->ant = jogadorB2;
    jogadorA1->prox = jogadorB1;

    jogadorB1->ant = jogadorA1;
    jogadorB1->prox = jogadorA2;

    jogadorA2->ant = jogadorB1;
    jogadorA2->prox = jogadorB2;

    jogadorB2->ant = jogadorA2;
    jogadorB2->prox = jogadorA1;

    Mesa *mesa = (Mesa *) malloc(sizeof(Mesa));
    mesa->jogadores = jogadorA1;
    mesa->rodada = 0;
    mesa->pontosA = 0;
    mesa->pontosB = 0;

    Jogo *jogo = (Jogo *) malloc(sizeof(Jogo));
    jogo->mesa = mesa;
    jogo->pontosA = 0;
    jogo->pontosB = 0;
    jogo->turno = 0;
    jogo->vencedor = 'N';
    jogo->lcartas = listCartas;
    jogo->carta_virada = NULL;

    return jogo;
}

void slp(unsigned int seconds) {
    if (modo == 'A') {
        sleep(seconds);
    }
}

void partidas(Jogo *jogo) {

    char trucoMaoOnze = 'N';
    while (verifica_vencedor(jogo) == 'N') {
        jogo->turno++;
        printf("-----Turno %d-----\n", jogo->turno);
        printf("Time A: %d pontos\n", jogo->pontosA);
        printf("Time B: %d pontos\n", jogo->pontosB);
        slp(1);
        PilhaCarta *baralho = distribuir_cartas(jogo);
        jogo->carta_virada = pop(baralho);

        int ponto = 1;
        int aposta = 1;

        char maoOnze = 'N';
        if (!(jogo->pontosA == 11 && jogo->pontosB == 11)) {
            slp(1);
            if (jogo->pontosA == 11) {
                maoOnze = mao_de_onze(jogo, 'A');
            } else if (jogo->pontosB == 11) {
                maoOnze = mao_de_onze(jogo, 'B');
            }

            if (maoOnze == '1') {
                slp(1);
                printf("Mão de Onze aceita!\n");
                ponto = 3;
            } else if (maoOnze == '0') {
                slp(1);
                printf("Mão de Onze rejeitada!\n");
                if (jogo->pontosA == 11) {
                    printf("O time B ganhou %d ponto!\n", 1);
                    jogo->pontosB = jogo->pontosB + 1;
                } else {
                    printf("O time A ganhou %d ponto!\n", 1);
                    jogo->pontosA = jogo->pontosA + 1;
                }
                recolher_cartas(jogo);
                continue;
            }
        }

        for (jogo->mesa->rodada = 0; jogo->mesa->rodada < 3; jogo->mesa->rodada++) {
            slp(1);
            printf("\n----Rodada %d----\n", jogo->mesa->rodada + 1);
            printf("Carta virada: %c de %s\n", jogo->carta_virada->valor, jogo->carta_virada->naipe);

            Jogador *jogador = jogo->jogador_que_distribuiu->prox;
            Carta *maiorCarta = NULL;
            char timeMaiorCarta = 'N';
            int empate = 0;
            for (int j = 0; j < 4; j++) {
                slp(1);
                printf("--Jogador %s--\n", jogador->nome);
                slp(1);
                printf("Cartas:\n");
                olharCartas(jogador->cartas);

                slp(1);
                ponto = aumentarAposta(ponto, maoOnze);
                if (maoOnze == '1' && ponto == 0) {
                    trucoMaoOnze = jogador->nome[0];
                    break;
                }

                if (ponto != aposta) {
                    slp(1);
                    aposta = aceitarAposta(ponto, jogador);
                    if (aposta != ponto) {
                        slp(1);
                        char time = jogador->nome[0];
                        printf("Fim da rodada! O time %c ganhou %d pontos!\n", time, aposta);
                        if (time == 'A') {
                            jogo->pontosA = jogo->pontosA + aposta;
                        } else {
                            jogo->pontosB = jogo->pontosB + aposta;
                        }
                        break;
                    }
                }
                slp(1);
                printf("Escolha:\n");

                PilhaCarta *cartas = jogador->cartas;
                Carta *carta = escolherCarta(cartas);

                if (maiorCarta != NULL && strcmp(carta->naipe, maiorCarta->naipe) == 0 &&
                    carta->valor == maiorCarta->valor) {
                    empate = 1;
                } else {
                    slp(1);
                    char maior = verifica_carta(carta, maiorCarta, jogo->carta_virada, jogo->lcartas);
                    if (maior == '1') {
                        timeMaiorCarta = jogador->nome[0];
                        maiorCarta = carta;
                    }
                    empate = 0;
                }
                slp(1);
                mostrarCarta(jogo->mesa->rodada, carta, jogador);
                jogador = jogador->prox;
            }

            if (trucoMaoOnze != 'N') {
                break;
            }

            if (aposta != ponto) {
                break;
            }
            slp(1);
            if (jogo->mesa->resultado[0] == 'E' && jogo->mesa->rodada == 1) {
                if (timeMaiorCarta == 'A') {
                    jogo->pontosA = jogo->pontosA + ponto;
                    printf("Fim do turno! O time A ganhou %d pontos!\n", ponto);
                    break;
                } else {
                    jogo->pontosB = jogo->pontosB + ponto;
                    printf("Fim da turno! O time B ganhou %d pontos!\n", ponto);
                    break;
                }
            }

            if (empate == 1 && jogo->mesa->rodada == 1) {
                if (jogo->mesa->resultado[0] == 'A') {
                    jogo->pontosA = jogo->pontosA + ponto;
                    printf("Fim do turno! O time A ganhou %d pontos!\n", ponto);
                    break;
                } else {
                    jogo->pontosB = jogo->pontosB + ponto;
                    printf("Fim da turno! O time B ganhou %d pontos!\n", ponto);
                    break;
                }
            }

            if (jogo->mesa->resultado[0] == 'E' && jogo->mesa->resultado[1] == 'E' && empate == 1 &&
                jogo->mesa->rodada == 2) {
                if (timeMaiorCarta == 'A') {
                    jogo->pontosA = jogo->pontosA + ponto;
                    printf("Fim do turno! O time A ganhou %d pontos!\n", ponto);
                    break;
                } else {
                    jogo->pontosB = jogo->pontosB + ponto;
                    printf("Fim do turno! O time B ganhou %d pontos!\n", ponto);
                    break;
                }
            }

            if (jogo->mesa->resultado[0] == 'E' && jogo->mesa->resultado[1] == 'E' && jogo->mesa->resultado[2] == 'E') {
                printf("Fim da turno! Empate!\n");
                break;
            }
            slp(1);
            if (timeMaiorCarta == 'A') {
                jogo->mesa->resultado[jogo->mesa->rodada] = 'A';
                printf("Fim da rodada! Vence time A!\n");
                jogo->mesa->pontosA++;
            } else if (timeMaiorCarta == 'B') {
                jogo->mesa->resultado[jogo->mesa->rodada] = 'B';
                printf("Fim da rodada! Vence time B!\n");
                jogo->mesa->pontosB++;
            }

            if (empate == 1) {
                jogo->mesa->resultado[jogo->mesa->rodada] = 'E';
                printf("Fim da rodada! Empate!\n");
            }

            if (jogo->mesa->pontosA >= 2) {
                jogo->pontosA = jogo->pontosA + ponto;
                printf("Fim do turno! O time A ganhou %d pontos!\n", ponto);
                break;
            } else if (jogo->mesa->pontosB >= 2) {
                jogo->pontosB = jogo->pontosB + ponto;
                printf("Fim da turno! O time B ganhou %d pontos!\n", ponto);
                break;
            }
        }
        if (trucoMaoOnze == 'S') {
            break;
        }
        ponto = 1;
        aposta = 1;
        recolher_cartas(jogo);
        strcpy(jogo->mesa->resultado, "");
        jogo->mesa->pontosB = 0;
        jogo->mesa->pontosA = 0;
        slp(1);
    }

    printf("\nFim de jogo!\n");
    if (trucoMaoOnze == 'A') {
        printf("O vencedor foi o time B!\n");
    } else if (trucoMaoOnze == 'B') {
        printf("O vencedor foi o time A!\n");
    } else {
        printf("Time A: %d pontos\n", jogo->pontosA);
        printf("Time B: %d pontos\n", jogo->pontosB);
        printf("O vencedor foi o time %c!\n", verifica_vencedor(jogo));
    }
    printf("\nDeseja jogar novamente?\n");
}

void mostrarCarta(int rodada, Carta *carta, Jogador *jogador) {
    if (rodada != 0) {
        int aux = 0;
        printf("Deseja esconder carta?\n");
        printf("1 - Sim\n");
        printf("2 - Não\n");
        scan("%d", &aux, 2);
        if (aux == 1) {
            printf("Carta escondida\n");
        } else {
            printf("Carta jogada: %c de %s\n", carta->valor, carta->naipe);
        }
    } else {
        printf("Carta jogada: %c de %s\n", carta->valor, carta->naipe);
    }
}

void scan(const char *__restrict format, int *aux, int r) {
    srand(time(NULL));
    if (modo == 'M') {
        scanf(format, aux);
    } else {
        int a;
        a = rand() % r + 1;
        *aux = a;
    }
}

char mao_de_onze(Jogo *jogo, char time) {
    Jogador *auxJogador = jogo->mesa->jogadores;
    PilhaCarta *aux = NULL;

    printf("\nMão de Onze para o time %c\n", time);
    int qtd = 0;
    while (1) {
        if (auxJogador->nome[0] == time) {
            if (qtd == 0) {
                printf("Suas cartas:\n");
            } else {
                printf("Cartas do parceiro:\n");
            }
            aux = auxJogador->cartas;
            while (aux->prox != NULL) {
                printf("%c de %s\n", aux->prox->valor->valor, aux->prox->valor->naipe);
                aux = aux->prox;
            }
            qtd++;
            if (qtd == 2) {
                break;
            }
        }
        auxJogador = auxJogador->prox;
    }

    printf("Aceitar partida?\n");
    printf("1 - Sim\n");
    printf("2 - Não\n");
    int opcao = 0;
    scan("%d", &opcao, 2);
    if (opcao == 1) {
        return '1';
    } else {
        return '0';
    }
}

char verifica_carta(Carta *carta1, Carta *carta2, Carta *vira, PilhaCarta *ordemCartas) {
    if (carta1 == NULL) {
        return '2';
    } else if (carta2 == NULL) {
        return '1';
    }

    PilhaCarta *aux = ordemCartas;
    Carta *manilha = NULL;

    int ordemCarta = 10;
    int ordemCarta1;
    int ordemCarta2;

    if (vira->valor == '3') {
        manilha = (Carta *) malloc(sizeof(Carta));
        manilha->valor = '4';
    } else if (vira->valor == '7') {
        manilha = (Carta *) malloc(sizeof(Carta));
        manilha->valor = 'Q';
    } else {
        char valor = '0';
        while (aux->prox != NULL) {
            if (aux->prox->valor->valor != valor) {
                valor = aux->prox->valor->valor;
                if (aux->prox->valor->valor == vira->valor) {
                    manilha = aux->prox->prox->prox->prox->prox->valor;
                    break;
                }
            }
            aux = aux->prox;
        }
    }

    if (carta1->valor == manilha->valor && carta2->valor != manilha->valor) {
        return '1';
    } else if (carta2->valor == manilha->valor && carta1->valor != manilha->valor) {
        return '2';
    } else if (carta1->valor == manilha->valor && carta2->valor == manilha->valor) {
        ordemCarta1 = verifica_naipe(carta1);
        ordemCarta2 = verifica_naipe(carta2);
    } else {
        char valor = '0';
        aux = ordemCartas;
        while (aux->prox != NULL) {
            if (aux->prox->valor->valor != valor) {
                valor = aux->prox->valor->valor;

                if (aux->prox->valor->valor == carta1->valor) {
                    ordemCarta1 = ordemCarta;
                }

                if (aux->prox->valor->valor == carta2->valor) {
                    ordemCarta2 = ordemCarta;
                }
                ordemCarta--;
            }
            aux = aux->prox;
        }
    }

    if (ordemCarta1 > ordemCarta2) {
        return '2';
    } else if (ordemCarta1 < ordemCarta2) {
        return '1';
    } else {
        return '0';
    }
}

int verifica_naipe(Carta *carta) {
    int ordemCarta;
    if (strcmp(carta->naipe, "Paus") == 0) {
        ordemCarta = 1;
    } else if (strcmp(carta->naipe, "Copas") == 0) {
        ordemCarta = 2;
    } else if (strcmp(carta->naipe, "Espadas") == 0) {
        ordemCarta = 3;
    } else if (strcmp(carta->naipe, "Ouros") == 0) {
        ordemCarta = 4;
    }
    return ordemCarta;
}

char verifica_vencedor(Jogo *jogo) {
    if (jogo->pontosA >= 12) {
        return 'A';
    } else if (jogo->pontosB >= 12) {
        return 'B';
    } else {
        return 'N';
    }
}

int aumentarAposta(int aposta, char maoOnze) {
    int opcao = 1;
    if (maoOnze == '1') {
        printf("1 - Pedir truco\n");
        printf("2 - Fazer nada\n");
        scan("%d", &opcao, 2);
        if (opcao == 1) {
            aposta = 0;
        }
    }
    switch (aposta) {
        case 1:
            printf("1 - Pedir truco\n");
            printf("2 - Fazer nada\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                aposta = 3;
            }
            break;
        case 3:
            printf("1 - Pedir seis\n");
            printf("2 - Fazer nada\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                aposta = 6;
            }
            break;
        case 6:
            printf("1 - Pedir nove\n");
            printf("2 - Fazer nada\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                aposta = 9;
            }
            break;
        case 9:
            printf("1 - Pedir doze\n");
            printf("2 - Fazer nada\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                aposta = 12;
            }
            break;
    }
    return aposta;
}

int aceitarAposta(int aposta, Jogador *jogador) {
    int opcao = 1;
    Jogador *jogadorAtual = jogador;
    printf("Jogador %s deseja aumentar a aposta?\n", jogadorAtual->prox->nome);
    slp(1);
    printf("Suas cartas:\n");
    olharCartas(jogador->prox->cartas);
    switch (aposta) {
        case 3:
            printf("1 - Aceitar truco\n");
            printf("2 - Rejeitar truco\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                printf("Aposta aumentada para 3 pontos\n");
                aposta = 3;
            } else {
                aposta = 1;
            }
            break;
        case 6:
            printf("1 - Aceitar seis?\n");
            printf("2 - Rejeitar seis\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                printf("Aposta aumentada para 6 pontos\n");
                aposta = 6;
            } else {
                aposta = 3;
            }
            break;
        case 9:
            printf("1 - Aceitar nove?\n");
            printf("2 - Rejeitar nove\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                printf("Aposta aumentada para 9 pontos\n");
                aposta = 9;
            } else {
                aposta = 6;
            }
            break;
        case 12:
            printf("1 - Aceitar doze?\n");
            printf("2 - Rejeitar doze\n");
            scan("%d", &opcao, 2);
            if (opcao == 1) {
                printf("Aposta aumentada para 12 pontos\n");
                aposta = 12;
            } else {
                aposta = 9;
            }
            break;
    }
    return aposta;
}

void olharCartas(PilhaCarta *cartas) {
    PilhaCarta *aux = cartas;
    while (aux->prox != NULL) {
        printf("%c de %s\n", aux->prox->valor->valor, aux->prox->valor->naipe);
        aux = aux->prox;
    }
}

Carta *escolherCarta(PilhaCarta *cartas) {
    PilhaCarta *aux = cartas;
    int tamanho = 0;
    while (aux->prox != NULL) {
        tamanho++;
        printf("%d - %c de %s\n", tamanho, aux->prox->valor->valor, aux->prox->valor->naipe);
        aux = aux->prox;
    }

    int escolha = 10;
    while (1) {
        printf("Escolha uma carta: ");
        scan("%d", &escolha, tamanho);
        if (escolha > tamanho) {
            printf("Essa opção não existe\n");
            continue;
        }
        break;
    }
    Carta *carta = remover_carta(cartas, escolha);
    return carta;
}

void recolher_cartas(Jogo *jogo) {
    Jogador *jogador = jogo->mesa->jogadores;

    for (int i = 0; i < 4; i++) {
        PilhaCarta *cartas = jogador->cartas;
        while (cartas != NULL) {
            PilhaCarta *temp = cartas;
            cartas = cartas->prox;
            free(temp);
        }
        jogador->cartas = cartas;
        jogador = jogador->prox;
    }

}

PilhaCarta *distribuir_cartas(Jogo *jogo) {
    if (jogo->turno == 1) {
        int sorteioDistribui = rand() % 4;
        Jogador *atual = jogo->mesa->jogadores;
        for (int k = 0; k < sorteioDistribui; k++) {
            atual = atual->prox;
        }
        jogo->jogador_que_distribuiu = atual;
    } else {
        jogo->jogador_que_distribuiu = jogo->jogador_que_distribuiu->prox;
    }

    PilhaCarta *baralho = embaralhar(jogo->lcartas);

    Jogador *jogadores = jogo->jogador_que_distribuiu;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            jogadores->cartas = adicionar_carta(jogadores->cartas, pop(baralho));
        }
        jogadores = jogadores->prox;
    }
    return baralho;
}

// adicionar na pilha
void push(PilhaCarta *topo, Carta *valor) {
    PilhaCarta *atual = topo;

    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    atual->prox->valor = valor;
    atual->prox->prox = NULL;
}

// remover da pilha
Carta *pop(PilhaCarta *topo) {
    Carta *valor;
    if (topo->prox == NULL) {
        valor = topo->valor;
        free(topo);
        return valor;
    }

    PilhaCarta *atual = topo;
    while (atual->prox->prox != NULL) {
        atual = atual->prox;
    }

    valor = atual->prox->valor;
    free(atual->prox);
    atual->prox = NULL;
    return valor;
}

// adicionar na lista
PilhaCarta *adicionar_carta(PilhaCarta *cartas, Carta *valor) {
    PilhaCarta *atual = cartas;
    if (atual == NULL) {
        atual = (PilhaCarta *) malloc(sizeof(PilhaCarta));
        PilhaCarta *carta = (PilhaCarta *) malloc(sizeof(PilhaCarta));
        carta->valor = valor;
        carta->prox = NULL;
        atual->prox = carta;
        return atual;
    }

    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    atual->prox->valor = valor;
    atual->prox->prox = NULL;
    return cartas;
}

Carta *remover_carta(PilhaCarta *cartas, int posicao) {
    PilhaCarta *atual = cartas;
    PilhaCarta *aux = NULL;
    Carta *valor = NULL;
    switch (posicao) {
        case 1:
            valor = atual->prox->valor;
            aux = atual->prox->prox;
            free(atual->prox);
            atual->prox = aux;
            break;
        case 2:
            valor = atual->prox->prox->valor;
            aux = atual->prox->prox->prox;
            free(atual->prox->prox);
            atual->prox->prox = aux;
            break;
        case 3:
            valor = atual->prox->prox->prox->valor;
            free(atual->prox->prox->prox);
            atual->prox->prox->prox = NULL;
            break;
    }
    return valor;
}

PilhaCarta *embaralhar(PilhaCarta *topo) {
    PilhaCarta *baralho = (PilhaCarta *) malloc(sizeof(PilhaCarta));;
    baralho->valor = NULL;
    baralho->prox = NULL;

    PilhaCarta *cartas = topo;

    while (cartas->prox != NULL) {
        Carta *novaCarta = (Carta *) malloc(sizeof(Carta));
        novaCarta->valor = cartas->prox->valor->valor;
        strcpy(novaCarta->naipe, cartas->prox->valor->naipe);
        push(baralho, novaCarta);
        cartas = cartas->prox;
    }

    for (int indexCarta = 0; indexCarta < TOTAL_CARTAS - 1; indexCarta++) {
        int indexCartaTrocada = rand() % TOTAL_CARTAS;

        PilhaCarta *original = baralho->prox;
        for (int k = 0; k < indexCarta; k++) {
            original = original->prox;
        }

        PilhaCarta *trocada = baralho->prox;
        for (int k = 0; k < indexCartaTrocada; k++) {
            trocada = trocada->prox;
        }

        Carta *aux = original->valor;
        original->valor = trocada->valor;
        trocada->valor = aux;
    }
    return baralho;
}

PilhaCarta *lerArq(char *nome) {
    PilhaCarta *topo = NULL;
    topo = (PilhaCarta *) malloc(sizeof(PilhaCarta));
    if (topo == NULL) {
        exit(1);
    }

    FILE *file = fopen("cartas.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[20];
    while (fgets(linha, 20, file) != NULL) {
        Carta *novaCarta = (Carta *) malloc(sizeof(Carta));
        sscanf(linha, " %c %[^\n]", &novaCarta->valor, novaCarta->naipe);
        push(topo, novaCarta);
    }

    fclose(file);
    return topo;
}