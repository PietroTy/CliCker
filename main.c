#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>

#define MAX_UPGRADES 6

// Estrutura para representar upgrades
typedef struct Upgrade {
    int cost;
    int increment;
    bool isPurchased;
} Upgrade;

// Função para resetar o jogo
void ResetGame(int *score, int *clickValue, Upgrade *upgrades, int numUpgrades) {
    *score = 0;
    *clickValue = 1;
    for (int i = 0; i < numUpgrades; i++) {
        upgrades[i].isPurchased = false;
    }
}

// Função para salvar o progresso em um arquivo CSV
void SaveGame(int score, int clickValue, Upgrade *upgrades, int numUpgrades) {
    FILE *file = fopen("save.csv", "w");
    if (file == NULL) {
        printf("Erro ao salvar o jogo.\n");
        return;
    }
    fprintf(file, "Score,ClickValue\n%d,%d\n", score, clickValue);
    fprintf(file, "UpgradesPurchased\n");
    for (int i = 0; i < numUpgrades; i++) {
        fprintf(file, "%d\n", upgrades[i].isPurchased ? 1 : 0);
    }
    fclose(file);
    printf("Progresso salvo.\n");
}

// Função para carregar o progresso do arquivo CSV
void LoadGame(int *score, int *clickValue, Upgrade *upgrades, int numUpgrades) {
    FILE *file = fopen("save.csv", "r");

    if (file == NULL) {
        printf("Nenhum dado salvo encontrado, começando novo jogo.\n");
        return;
    }

    fscanf(file, "Score,ClickValue\n%d,%d\n", score, clickValue);
    fscanf(file, "UpgradesPurchased\n");

    for (int i = 0; i < numUpgrades; i++) {
        int purchased;
        fscanf(file, "%d\n", &purchased);
        upgrades[i].isPurchased = purchased == 1;
    }
    
    fclose(file);
    printf("Progresso carregado.\n");
}

int main(void) {
    int score = 0;
    int clickValue = 1;
    const int numUpgrades = MAX_UPGRADES;
    Upgrade upgrades[MAX_UPGRADES];

    // Definindo os upgrades
    upgrades[0] = (Upgrade){10, 1, false};
    upgrades[1] = (Upgrade){50, 5, false};
    upgrades[2] = (Upgrade){100, 10, false};
    upgrades[3] = (Upgrade){250, 20, false};  // Custo 250, incremento 20
    upgrades[4] = (Upgrade){500, 50, false};  // Custo 500, incremento 50
    upgrades[5] = (Upgrade){1000, 100, false}; // Custo 1000, incremento 100

    // Tamanho maior da janela para mais upgrades e botões
    InitWindow(800, 600, "Clicker Game");
    SetTargetFPS(60);

    // Carregar progresso do jogo, se existir
    LoadGame(&score, &clickValue, upgrades, numUpgrades);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // Detectar clique
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Verificar se o clique está no botão de "clique"
            if (CheckCollisionPointRec(mousePos, (Rectangle){350, 200, 100, 100})) {
                score += clickValue;
            }

            // Verificar cliques nos upgrades
            for (int i = 0; i < numUpgrades; i++) {
                Rectangle upgradeRect = (Rectangle){50, 350 + (i * 40), 200, 30};
                if (CheckCollisionPointRec(mousePos, upgradeRect) && !upgrades[i].isPurchased && score >= upgrades[i].cost) {
                    score -= upgrades[i].cost;
                    clickValue += upgrades[i].increment;
                    upgrades[i].isPurchased = true;
                }
            }

            // Verificar clique no botão de salvar
            if (CheckCollisionPointRec(mousePos, (Rectangle){650, 500, 100, 40})) {
                SaveGame(score, clickValue, upgrades, numUpgrades);
            }
        }

        // Desenhar elementos
        BeginDrawing();
        ClearBackground(BLACK);

        // Desenhar botão de clique
        DrawRectangle(350, 200, 100, 100, BEIGE);
        DrawText("CLIQUE", 370, 240, 20, DARKGRAY);

        // Mostrar pontuação
        DrawText(TextFormat("Score: %d", score), 10, 10, 30, WHITE);

        // Mostrar upgrades
        for (int i = 0; i < numUpgrades; i++) {
            Rectangle upgradeRect = (Rectangle){50, 350 + (i * 40), 200, 30};
            DrawRectangleRec(upgradeRect, upgrades[i].isPurchased ? DARKGREEN : DARKGRAY);
            DrawText(TextFormat("Upgrade %d: Custo %d", i + 1, upgrades[i].cost), 60, 355 + (i * 40), 20, upgrades[i].isPurchased ? GREEN : WHITE);
        }

        // Desenhar botão de salvar
        DrawRectangle(650, 500, 100, 40, DARKBLUE);
        DrawText("Salvar", 675, 510, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
