#include "core/game_manager.h"
#include "raylib.h"

int main() {
    const int windowWidth = 1280;
    const int windowHeight = 720;

    InitWindow(windowWidth, windowHeight, "Canvas Of Dreams");
    SetTargetFPS(60);

    GameManager gameManager;
    gameManager.initialize();

    while (!WindowShouldClose()) {
        gameManager.update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        gameManager.render();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
