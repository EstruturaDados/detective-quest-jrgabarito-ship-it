#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura que representa uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

//Função: criarSala
//Objetivo: Cria dinamicamente uma nova sala na memória, define seu nome e inicializa os ponteiros de caminhos como NULL.
 
Sala* criarSala(const char* nomeSala) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nomeSala);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função: explorarSalas
// Objetivo: Gerencia a navegação interativa do jogador pela mansão.
// O jogo continua até encontrar uma sala sem saídas (nó-folha) ou o usuário decidir sair.

void explorarSalas(Sala* raiz) {
    if (raiz == NULL) {
        printf("A mansao está vazia!\n");
        return;
    }

    Sala* salaAtual = raiz;
    char escolha;

    printf("\n=== BEM-VINDO AO DETECTIVE QUEST ===\n");
    printf("Sua investigacao começa agora. Encontre as pistas!\n\n");

    // O loop continua até que a sala atual seja um nó-folha (sem esquerda E sem direita)
    while (salaAtual->esquerda != NULL || salaAtual->direita != NULL) {
        printf("----------------------------------------\n");
        printf("Voce esta na sala: %s\n", salaAtual->nome);
        printf("Caminhos disponiveis:\n");
        
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Ir para a Esquerda (%s)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("  [d] Ir para a Direita (%s)\n", salaAtual->direita->nome);
        }
        printf("  [s] Sair do jogo\n");
        
        printf("Escolha sua direcao: ");
        scanf(" %c", &escolha); // O espaço antes de %c limpa o buffer do teclado

        if (escolha == 's' || escolha == 'S') {
            printf("\nVoce abandonou a investigacao. O culpado escapou!\n");
            return;
        } 
        else if ((escolha == 'e' || escolha == 'E') && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } 
        else if ((escolha == 'd' || escolha == 'D') && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } 
        else {
            printf("\n[Aviso] Escolha invalida ou caminho bloqueado! Tente novamente.\n");
        }
    }

    // Chegou a um nó-folha
    printf("----------------------------------------\n");
    printf("Voce chegou ao fim da linha: %s\n", salaAtual->nome);
    printf("Investigacao concluida! Voce explorou a mansao com sucesso.\n");
}

// Função: main
// Objetivo: Monta a estrutura da mansão interligando os nós e inicia o jogo.
 
int main() {
    // 1. Criação dos cômodos (Nós da árvore)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* escritorio = criarSala("Escritorio");
    Sala* jardim = criarSala("Jardim Externo");
    Sala* porao = criarSala("Porao Escuro");

    // 2. Montagem do mapa (Interligando a árvore binária)
    // Nível 1 (Raiz)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    // Nível 2 (Filhos do Hall)
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;

    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // 3. Início da exploração
    explorarSalas(hall);

    // 4. Liberação de memória (Boa prática)
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(escritorio);
    free(jardim);
    free(porao);

    return 0;
}

