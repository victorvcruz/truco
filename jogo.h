#ifndef TRUCO_JOGO_H
#define TRUCO_JOGO_H

/*
fluxo: jogo truco paulista

1 - ler o arquivo cartas.txt e salva na lista de cartas
2 - inicializar as estrutura do jogo.
   2.1 zerar os pontos
   2.2 atribuir nulo nas struturas
   2.3

3 - faz um cópias das cartas da lista de cartas para embaralhar

4 - se for o primeira partida, a distribuição será escolhida aleatória e, o outro time começa jogando, a carta virada será a próxima carta da distribuição.
   4.1 - a partir da próxima partida, a distribuição será pelo próximo a direita.
   4.2 atualizar a lista de manilhas

5 - A primeira jogada da partida, será o jogador da direita do que destribuiu
   5.1 - As proxima roda da partida, é retomada por quem ganhou a mão.
   5.2 Se houver empate na primeira rodada, vira-se a maior carta.
      5.2.1 Se empatar a segunda, o jogo irá para a terceira rodada
   5.3 Se empatar a segunda ou a terceira, quem fez a primeira, leva a rodada.

   partida ou rodada - é uma jogada do turno
   turno - é a melhor de 3 partida


   Naipes: ouros (♦ ), espadas (♠), copas (♥) e paus (♣)

*/
typedef struct carta{
    char naipe[15];
    char valor;
}Carta;

typedef struct PilhaCarta {
    Carta *valor;
    struct PilhaCarta *prox;
} PilhaCarta;

typedef struct jogador{
    char nome[2];
    struct PilhaCarta *cartas;
    struct jogador *ant;
    struct jogador * prox;
}Jogador;

typedef struct mesa{
    int pontosA;
    int pontosB;
    struct jogador *jogadores;
    int rodada;
    char resultado;
}Mesa;

typedef struct jogo{
    int pontosA;
    int pontosB;
    int turno;
    struct jogador *jogador_que_distribuiu;
    struct mesa *mesa;
    struct PilhaCarta *lcartas;
    Carta *carta_virada;
    char vencedor; /* A - venceu ; B - venceu ; N - não tem vencedor ainda */

}Jogo;

/*
  funcao: lerArq
  descricao: ler o arquivo de cartas
  entrada: nome do arquivo
  retorno: uma lista de cartas

*/
PilhaCarta *lerArq(char *nome);

/*
  funcao: inicializa_estruturas
  descricao: inicializa as estruturas do jogo
  entrada:
  retorno: a estrutura jogo

*/
Jogo *inicializa_estruturas(PilhaCarta *listCartas);



/*
  funcao: turnos_manual
  descricao: realiza os turnos de um jogo manual
  entrada: jogo
  retorno: resultados: A - Vencedor ; B - Vencedor

*/
char turnos_manual(Jogo *jogo);


/*
  funcao: turnos_automatico
  descricao: realiza os turnos de um jogo automático
  entrada: jogo
  retorno: resultados: A - Vencedor ; B - Vencedor

*/
char turnos_automatico(Jogo *jogo);


/*
  funcao: partidas_manual
  descricao: realiza as partidas_manual de um jogo manual
  entrada: jogo
  retorno: resultados:

*/
void partidas_manual(Jogo *jogo);


/*
  funcao: partidas_automática
  descricao: realiza as partidas automática de um jogo automático
  entrada: jogo
  retorno: resultados:

*/
void partidas_automatica(Jogo *jogo);



/*
  funcao: verifica_vencedor
  descricao: realiza a contagem de pontos  de um jogo
  entrada: jogo
  retorno: resultados: A - Vencedor ; B - Vencedor ; N - sem vencedor

*/
char verifica_vencedor(Jogo *jogo);

void push(PilhaCarta * topo, Carta *valor);

PilhaCarta * embaralhar(PilhaCarta *topo);

Carta *pop(PilhaCarta *topo);

PilhaCarta *  distribuir_cartas(Jogo *jogo);

Carta * escolherCarta(PilhaCarta *cartas);

Carta * remover_carta(PilhaCarta *cartas, int posicao);

PilhaCarta * adicionar_carta(PilhaCarta *cartas, Carta *valor);

char verifica_carta(Carta *carta1, Carta * carta2, Carta * vira, PilhaCarta *ordemCartas) ;

int verifica_naipe(Carta * carta);
#endif //TRUCO_JOGO_H