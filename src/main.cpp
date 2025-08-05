#include "Platform.h"
#include "Player.h"
#include "raylib.h"
#include "vector"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pixel Platformer");
    SetTargetFPS(60);

    Player player(100, 100);

    std::vector<Platform> platforms;
    platforms.push_back(Platform(0, 500, 800, 20)); // Ground
    platforms.push_back(Platform(150, 450, 50, 20));
    platforms.push_back(Platform(200, 400, 50, 20));
    platforms.push_back(Platform(300, 300, 50, 20));
    platforms.push_back(Platform(400, 200, 50, 20));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float delta = GetFrameTime();
        player.Update(delta, platforms);
        player.UpdateAnimation(delta);

        // Draw Platforms
        for (const Platform &platform : platforms) {
            platform.Draw();
        }

        // Draw Player
        player.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
