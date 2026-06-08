#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct para os nós da Árvore Binária de Busca (BST) de Pistas
typedef struct PistaNode {
    char textoPista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Struct para os nós da Árvore Binária que representa o mapa da Mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; // String com a pista da sala (pode ser vazia "" se não houver)
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função: criarSala
// Objetivo: Cria dinamicamente uma nova sala na memória, define seu nome,
//associa uma pista a ela e inicializa os ponteiros de caminhos como NULL.
 
Sala* criarSala(const char* nomeSala, const char* pistaSala) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocacao de memoria para a sala!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nomeSala);
    strcpy(novaSala->pista, pistaSala);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função: inserirPista
// Objetivo: Insere uma pista de forma ordenada (alfabeticamente) na árvore BST.
// Utiliza a função strcmp() para comparar as strings e decidir o lado.
 
PistaNode* inserirPista(PistaNode* raizPistas, const char* novaPista) {
    // Se a árvore (ou subárvore) estiver vazia, cria um novo nó
    if (raizPistas == NULL) {
        PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novoNo == NULL) {
            printf("Erro de alocação de memória para a pista!\n");
            exit(1);
        }
        strcpy(novoNo->textoPista, novaPista);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }

    // Compara a nova pista com a pista do nó atual
    // strcmp retorna < 0 se a primeira for alfabeticamente menor
    if (strcmp(novaPista, raizPistas->textoPista) < 0) {
        raizPistas->esquerda = inserirPista(raizPistas->esquerda, novaPista);
    } else if (strcmp(novaPista, raizPistas->textoPista) > 0) {
        raizPistas->direita = inserirPista(raizPistas->direita, novaPista);
    }
    // Se forem iguais (strcmp == 0), não faz nada (evita pistas duplicadas)

    return raizPistas;
}

// Função: exibirPistas
// Objetivo: Imprime todas as pistas da árvore BST em ordem alfabética.
// Utiliza o percurso em-ordem (Esquerda, Raiz, Direita).
 
void exibirPistas(PistaNode* raizPistas) {
    if (raizPistas != NULL) {
        exibirPistas(raizPistas->esquerda);         // Visita a subárvore esquerda
        printf("- %s\n", raizPistas->textoPista);     // Visita a raiz (imprime)
        exibirPistas(raizPistas->direita);          // Visita a subárvore direita
    }
}

// Função: explorarSalasComPistas
// Objetivo: Controla a navegação do jogador pelas salas. A cada sala visitada,
// se houver uma pista, ela é coletada e inserida automaticamente na BST.
// O jogo continua até o jogador escolher explicitamente sair ('s').
 
void explorarSalasComPistas(Sala* raizMansao) {
    if (raizMansao == NULL) {
        printf("A mansão está vazia!\n");
        return;
    }

    Sala* salaAtual = raizMansao;
    PistaNode* inventarioPistas = NULL; // Raiz da árvore BST de pistas coletadas
    char escolha;

    printf("\n=== DETECTIVE QUEST: EXPANSAO COLETA DE PISTAS ===\n");
    printf("Explore a mansao, colete os indicios e desvende o misterio.\n");

    while (1) {
        printf("\n--------------------------------------------------\n");
        printf("Voce esta na sala: %s\n", salaAtual->nome);

        // Sistema de coleta automática de pistas
        if (strlen(salaAtual->pista) > 0) {
            printf("[!] PISTA ENCONTRADA: %s\n", salaAtual->pista);
            // Armazena na BST de pistas
            inventarioPistas = inserirPista(inventarioPistas, salaAtual->pista);
            // Limpa a pista da sala para não coletar repetidamente caso o jogador volte nela
            strcpy(salaAtual->pista, ""); 
        } else {
            printf("[i] Nenhuma pista nova aqui.\n");
        }

        // Exibe opções de caminhos
        printf("\nCaminhos disponiveis:\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Ir para a Esquerda (%s)\n", salaAtual->esquerda->nome);
        } else {
            printf("  [e] Caminho da Esquerda: Bloqueado (Parede)\n");
        }

        if (salaAtual->direita != NULL) {
            printf("  [d] Ir para a Direita (%s)\n", salaAtual->direita->nome);
        } else {
            printf("  [d] Caminho da Direita: Bloqueado (Parede)\n");
        }
        printf("  [s] Encerrar investigacao e listar pistas\n");

        printf("Escolha sua acao: ");
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            printf("\nInvestigacao encerrada pelo detetive.\n");
            break; // Sai do loop de exploração
        } 
        else if ((escolha == 'e' || escolha == 'E') && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } 
        else if ((escolha == 'd' || escolha == 'D') && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } 
        else {
            printf("\n[Aviso] Comando invalido ou direcao sem saida! Tente novamente.\n");
        }
    }

    // Fim do jogo: Exibe o relatório de pistas organizadas
    printf("\n==================================================\n");
    printf("RELATORIO FINAL: INDICIOS COLETADOS (ORDEM ALFABETICA)\n");
    printf("==================================================\n");
    if (inventarioPistas == NULL) {
        printf("Voce nao coletou nenhuma pista durante a investigacao.\n");
    } else {
        exibirPistas(inventarioPistas);
    }
    printf("==================================================\n");

    // NOTA: Em um cenário real, também liberaríamos a memória da árvore de pistas aqui.
}

// Função: main
//Objetivo: Define a estrutura fixa da mansão, insere as pistas iniciais nos cômodos e inicia o fluxo.
 
int main() {
    // 1. Criação das salas com suas respectivas pistas (Requisito Funcional)
    // Passamos o Nome da Sala e a Pista textual associada
    Sala* hall = criarSala("Hall de Entrada", "Pegadas de lama apontando para o corredor.");
    Sala* salaEstar = criarSala("Sala de Estar", "Um copo quebrado com vestigios de veneno.");
    Sala* cozinha = criarSala("Cozinha", "Faca de chefe ausente do suporte.");
    Sala* biblioteca = criarSala("Biblioteca", "Diario secreto trancado na gaveta.");
    Sala* escritorio = criarSala("Escritorio", "Bilhete de chantagem rasgado na lixeira.");
    Sala* jardim = criarSala("Jardim Externo", ""); // Sala sem pista (opcional)
    Sala* porao = criarSala("Porao Escuro", "Chave dourada escondida sob o tapete.");

    // 2. Montagem do mapa fixo da Mansão (Árvore Binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;

    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // 3. Inicia o jogo e o sistema de coleta
    explorarSalasComPistas(hall);

    // 4. Liberação de memória dos nós da mansão
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(escritorio);
    free(jardim);
    free(porao);

    return 0;
}