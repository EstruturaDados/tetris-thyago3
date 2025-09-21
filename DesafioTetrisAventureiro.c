#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente, tras, qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

int contadorId = 0;
char tipos[] = {'I', 'O', 'T', 'L'};

Peca gerarPeca() {
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorId++;
    return nova;
}

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void push(Pilha *p, Peca pc) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = pc;
}

Peca pop(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    return p->itens[p->topo--];
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=== Estado Atual ===\n");
    printf("Fila de pecas:\t");
    int i, idx;
    for (i = 0, idx = f->frente; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
    printf("Pilha de reserva (Topo -> Base): ");
    for (i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n====================\n");
}

int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada = desenfileirar(&fila);
            if (jogada.id != -1) {
                printf("Voce jogou a peca [%c %d]\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca());
            }
        } else if (opcao == 2) {
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca reservada = desenfileirar(&fila);
                push(&pilha, reservada);
                printf("Peca [%c %d] reservada\n", reservada.nome, reservada.id);
                enfileirar(&fila, gerarPeca());
            } else {
                printf("Nao foi possivel reservar\n");
            }
        } else if (opcao == 3) {
            Peca usada = pop(&pilha);
            if (usada.id != -1) {
                printf("Voce usou a peca reservada [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("Nenhuma peca reservada\n");
            }
        }

    } while (opcao != 0);

    printf("Jogo encerrado!\n");
    return 0;
}
