#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[50];
    char corExercito[20];
    int tropas;
} Territorio;

//------------------------- MISSÕES ----------------------------//

// Sorteia missão e copia para variável do jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
    printf("\n🎯 Missão atribuída: %s\n", destino);
}

// Verifica se a missão foi cumprida (lógica inicial simplificada)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    
    // Missão 1 — Ter 3 territórios com a mesma cor
    if (strstr(missao, "controlar 3 territorios") != NULL) {
        int cont = 0;
        for(int i=0; i<tamanho; i++)
            if(strcmp(mapa[i].corExercito, "Azul") == 0) // Exemplo de cor
                cont++;

        if(cont >= 3) return 1; // Missão cumprida
    }

    // Missão 2 — Conquistar qualquer território deixando com 0 tropas
    if (strstr(missao, "zerar tropas") != NULL) {
        for(int i=0; i<tamanho; i++)
            if(mapa[i].tropas == 0) return 1;
    }

    // Missão 3 — Ter o dobro de tropas em algum território
    if (strstr(missao, "dobro de tropas") != NULL) {
        for(int i=0; i<tamanho; i++)
            if(mapa[i].tropas >= 20) return 1; // meta provisória
    }

    return 0; // Ainda não venceu
}

//------------------------- ATAQUE ----------------------------//

void atacar(Territorio *atacante, Territorio *defensor) {
    if(atacante->tropas <= 1) {
        printf("\n❌ Tropas insuficientes para atacar!\n");
        return;
    }

    int dadoA = (rand()%6)+1;
    int dadoD = (rand()%6)+1;

    printf("\n🎲 Dado atacante: %d | 🎲 Dado defensor: %d\n", dadoA,dadoD);

    if(dadoA > dadoD) {
        defensor->tropas--;
        printf("🔥 Atacante venceu o duelo!\n");

        if(defensor->tropas <= 0){
            printf("🏳 Território %s foi conquistado!\n", defensor->nome);
            defensor->tropas = atacante->tropas/2;
            strcpy(defensor->corExercito, atacante->corExercito);
        }
    } else {
        atacante->tropas--;
        printf("🛡 Defesa resistiu!\n");
    }
}

//------------------------- MAIN GAME ----------------------------//

int main(){
    srand(time(NULL));

    int qtd;
    printf("\nQuantos territórios deseja cadastrar? ");
    scanf("%d",&qtd);

    Territorio *mapa = malloc(qtd*sizeof(Territorio));

    for(int i=0;i<qtd;i++){
        printf("\n--- Território %d ---\n",i+1);
        printf("Nome: "); scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exercito: "); scanf(" %[^\n]", mapa[i].corExercito);
        printf("Tropas: "); scanf("%d",&mapa[i].tropas);
    }

    // Banco de missões
    char* missoes[] = {
        "controlar 3 territorios de mesma cor",
        "zerar tropas de um territorio inimigo",
        "ter dobro de tropas em qualquer territorio",
        "conquistar um territorio adjacente",
        "manter lideranca de tropas por 3 rodadas"
    };

    int totalMissoes = 5;

    // Cada jogador recebe missão usando malloc
    char* missaoJogador = malloc(200*sizeof(char));
    atribuirMissao(missaoJogador, missoes, totalMissoes);

    // LOOP DO JOGO
    while(1){
        int A, D;

        printf("\n--------- MAPA -----------\n");
        for(int i=0;i<qtd;i++)
            printf("%d - %s | Cor:%s | Tropas:%d\n",
                i,mapa[i].nome,mapa[i].corExercito,mapa[i].tropas);

        printf("\nAtaque (-1 p/ sair)\nAtacante: ");
        scanf("%d",&A);
        if(A==-1) break;

        printf("Defensor: ");
        scanf("%d",&D);

        if(A<0||A>=qtd||D<0||D>=qtd||A==D){
            printf("❌ Seleção inválida!\n"); continue;
        }

        atacar(&mapa[A], &mapa[D]);

        // Verifica vitória por missão
        if(verificarMissao(missaoJogador, mapa, qtd)){
            printf("\n🏆 MISSÃO CUMPRIDA! VOCÊ VENCEU!\n");
            printf("Missão concluída: %s\n",missaoJogador);
            break;
        }
    }

    free(mapa);
    free(missaoJogador);
    return 0;
}
