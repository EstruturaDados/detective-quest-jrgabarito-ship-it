#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 11 // Tamanho primo para ajudar na distribuição da Hash

// --- ESTRUTURAS DE DADOS ---

// Nó da Árvore Binária de Busca (BST) de Pistas Coletadas
typedef struct PistaNode {
    char textoPista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Nó da Árvore Binária de Cômodos da Mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó para a Lista Ligada da Tabela Hash (Tratamento de Colisões por Encadeamento)
typedef struct HashNode {
    char chavePista[100];
    char valorSuspeito[50];
    struct HashNode* proximo;
} HashNode;

// Estrutura da Tabela Hash
typedef struct {
    HashNode* tabela[TAMANHO_HASH];
} TabelaHash;


// --- FUNÇÕES DA TABELA HASH ---

/* * Função: funcaoHash
 * Objetivo: Transforma a string da pista em um índice numérico para a tabela.
 * Utiliza o algoritmo clássico polinomial de espalhamento (DJB2 adaptado).
 */
int funcaoHash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % TAMANHO_HASH;
}

/* * Função: inserirNaHash
 * Objetivo: Insere a associação [Pista -> Suspeito] dentro da tabela hash.
 */
void inserirNaHash(TabelaHash* ht, const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);
    
    HashNode* novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        printf("Erro de alocacao na Hash!\n");
        exit(1);
    }
    strcpy(novoNo->chavePista, pista);
    strcpy(novoNo->valorSuspeito, suspeito);
    
    // Insere no início da lista encadeada daquela posição (colisão)
    novoNo->proximo = ht->tabela[indice];
    ht->tabela[indice] = novoNo;
}

/* * Função: encontrarSuspeito
 * Objetivo: Consulta a tabela hash para descobrir qual suspeito está atrelado à pista.
 */
const char* encontrarSuspeito(TabelaHash* ht, const char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = ht->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->chavePista, pista) == 0) {
            return atual->valorSuspeito;
        }
        atual = atual->proximo;
    }
    return "Desconhecido";
}


// --- FUNÇÕES DAS ÁRVORES ---

/* * Função: criarSala
 * Objetivo: Cria dinamicamente um cômodo da mansão com uma pista estática.
 */
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

/* * Função: inserirPista
 * Objetivo: Insere a pista coletada na árvore BST de forma alfabética.
 */
PistaNode* inserirPista(PistaNode* raizPistas, const char* novaPista) {
    if (raizPistas == NULL) {
        PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novoNo == NULL) {
            printf("Erro de alocacao para o no da pista!\n");
            exit(1);
        }
        strcpy(novoNo->textoPista, novaPista);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }

    if (strcmp(novaPista, raizPistas->textoPista) < 0) {
        raizPistas->esquerda = inserirPista(raizPistas->esquerda, novaPista);
    } else if (strcmp(novaPista, raizPistas->textoPista) > 0) {
        raizPistas->direita = inserirPista(raizPistas->direita, novaPista);
    }
    return raizPistas;
}

/* * Função: contarPistasDoSuspeito
 * Objetivo: Navega recursivamente pela BST de pistas coletadas, consulta a Hash,
 * e conta quantas dessas pistas acusam o suspeito alvo.
 */
int contarPistasDoSuspeito(PistaNode* raizPistas, TabelaHash* ht, const char* suspeitoAlvo) {
    if (raizPistas == NULL) return 0;

    int contagem = 0;
    // Verifica quem essa pista aponta na tabela hash
    const char* suspeitoDaPista = encontrarSuspeito(ht, raizPistas->textoPista);
    
    if (strcmp(suspeitoDaPista, suspeitoAlvo) == 0) {
        contagem = 1;
    }

    // Soma o resultado das subárvores esquerda e direita
    return contagem + 
           contarPistasDoSuspeito(raizPistas->esquerda, ht, suspeitoAlvo) + 
           contarPistasDoSuspeito(raizPistas->direita, ht, suspeitoAlvo);
}

/* * Função: exibirPistasEAlvos
 * Objetivo: Lista todas as pistas da BST em ordem alfabética mostrando quem elas incriminam.
 */
void exibirPistasEAlvos(PistaNode* raizPistas, TabelaHash* ht) {
    if (raizPistas != NULL) {
        exibirPistasEAlvos(raizPistas->esquerda, ht);
        printf("- \"%s\" (Aponta para: %s)\n", raizPistas->textoPista, encontrarSuspeito(ht, raizPistas->textoPista));
        exibirPistasEAlvos(raizPistas->direita, ht);
    }
}


// --- FASE DE JULGAMENTO E EXPLORAÇÃO ---

/* * Função: verificarSuspeitoFinal
 * Objetivo: Conduz a fase do veredito. Pede a acusação do jogador e valida,
 * cruzando os dados da BST com a Hash, se há provas suficientes (>= 2 pistas).
 */
void verificarSuspeitoFinal(PistaNode* raizPistas, TabelaHash* ht) {
    char acusacao[50];
    
    printf("\n==================================================\n");
    printf("         FASE FINAL: O JULGAMENTO\n");
    printf("==================================================\n");
    printf("Com base nas suas investigacoes, aqui estao suas pistas:\n");
    
    if (raizPistas == NULL) {
        printf("[!] Voce nao coletou nenhuma pista! Nao ha como acusar ninguem.\n");
        return;
    }
    
    exibirPistasEAlvos(raizPistas, ht);
    
    printf("\nDigite exatamente o nome do seu Suspeito (Opcoes: Mordomo, Cozinheira, Jardineiro):\n");
    printf("Sua acusacao: ");
    scanf(" %[^\n]", acusacao); // Permite ler nomes com espaços se necessário

    // Processa a contagem de provas usando a árvore e a hash
    int ProvasEncontradas = contarPistasDoSuspeito(raizPistas, ht, acusacao);

    printf("\nAnalisando o caso...\n");
    printf("Provas validas coletadas contra %s: %d\n", acusacao, ProvasEncontradas);

    if (ProvasEncontradas >= 2) {
        printf("\n[SUCESSO] Justica feita! Voce tinha provas robustas suficentes.\n");
        printf("O culpado %s foi preso em flagrante!\n", acusacao);
    } else {
        printf("\n[FRACASSO] O caso desmoronou! O advogado de %s conseguiu a absolvicao.\n", acusacao);
        printf("Motivo: Menos de 2 pistas concretas ligavam o suspeito ao crime.\n");
    }
    printf("==================================================\n");
}

/* * Função: explorarSalas
 * Objetivo: Navega pela árvore binária da mansão e alimenta a BST de pistas automáticas.
 */
PistaNode* explorarSalas(Sala* raizMansao, TabelaHash* ht) {
    Sala* salaAtual = raizMansao;
    PistaNode* inventarioPistas = NULL;
    char escolha;

    printf("\n=== DETECTIVE QUEST - CAPITULO FINAL (NIVEL MESTRE) ===\n");
    printf("Colete indicios cruciais para montar seu caso no tribunal.\n");

    while (1) {
        printf("\n--------------------------------------------------\n");
        printf("Voce esta na sala: %s\n", salaAtual->nome);

        // Coleta automática se houver pista na sala estática
        if (strlen(salaAtual->pista) > 0) {
            printf("[!] PISTA ENCONTRADA: %s\n", salaAtual->pista);
            inventarioPistas = inserirPista(inventarioPistas, salaAtual->pista);
            strcpy(salaAtual->pista, ""); // Evita re-coleta
        } else {
            printf("[i] Sem pistas novas por aqui.\n");
        }

        printf("\nAcoes disponiveis:\n");
        if (salaAtual->esquerda != NULL) printf("  [e] Ir para Esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL) printf("  [d] Ir para Direita (%s)\n", salaAtual->direita->nome);
        printf("  [s] Encerrar busca e ir para o Julgamento\n");

        printf("Escolha sua acao: ");
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            break;
        } 
        else if ((escolha == 'e' || escolha == 'E') && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } 
        else if ((escolha == 'd' || escolha == 'D') && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } 
        else {
            printf("\n[Aviso] Caminho invalido!\n");
        }
    }
    return inventarioPistas;
}


// --- MAIN ---

int main() {
    // 1. Inicializando a Tabela Hash
    TabelaHash ht;
    for (int i = 0; i < TAMANHO_HASH; i++) {
        ht.tabela[i] = NULL;
    }

    // 2. Mapeando Pistas estáticas aos Suspeitos na Hash
    inserirNaHash(&ht, "Bilhete de chantagem rasgado", "Mordomo");
    inserirNaHash(&ht, "Veneno comprado em nome do funcionario", "Mordomo");
    
    inserirNaHash(&ht, "Faca de chefe ausente da bancada", "Cozinheira");
    inserirNaHash(&ht, "Vestigios de toxinas no avental", "Cozinheira");
    
    inserirNaHash(&ht, "Luvas de jardinagem com marcas de sangue", "Jardineiro");
    inserirNaHash(&ht, "Pegadas de botas de lama raras no tapete", "Jardineiro");

    // 3. Criação das Salas (Mapa Fixo)
    Sala* hall = criarSala("Hall de Entrada", "");
    Sala* salaEstar = criarSala("Sala de Estar", "Bilhete de chantagem rasgado");
    Sala* cozinha = criarSala("Cozinha", "Faca de chefe sumida da bancada");
    Sala* biblioteca = criarSala("Biblioteca", "Veneno comprado em nome do funcionario");
    Sala* escritorio = criarSala("Escritorio", "Vestigios de toxinas no avental");
    Sala* jardim = criarSala("Jardim Externo", "Luvas de jardinagem com marcas de sangue");
    Sala* porao = criarSala("Porao Escuro", "Pegadas de botas de lama raras no tapete");

    // Conectando os caminhos da Mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;

    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // 4. Fluxo Principal do Jogo
    PistaNode* pistasColetadas = explorarSalas(hall, &ht);
    
    verificarSuspeitoFinal(pistasColetadas, &ht);

    // 5. Limpeza de Memória (Mansão)
    free(hall); free(salaEstar); free(cozinha);
    free(biblioteca); free(escritorio); free(jardim); free(porao);

    // NOTA: Em código de produção, aqui também rodaríamos a limpeza dos nós alocados na BST e na Tabela Hash.
    return 0;
}