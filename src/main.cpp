#include "Platform.h"
#include "Player.h"
#include "raylib.h"
#include "vector"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pixel Platformer");
    SetTargetFPS(60);

    Player player(100, 200);
    Platform::LoadTileset();

    std::vector<Platform> platforms;
    platforms.push_back(Platform(0, 500, 800, 95)); // Ground
    platforms.push_back(Platform(150, 450, 50, 20));
    platforms.push_back(Platform(200, 400, 50, 20));
    platforms.push_back(Platform(300, 300, 50, 20));
    platforms.push_back(Platform(400, 200, 100, 32));
    platforms.push_back(Platform(100, 150, 100, 70));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        float delta = GetFrameTime();
        player.Update(delta, platforms);

        // Draw Platforms
        for (const Platform &platform : platforms) {
            platform.Draw();
        }

        // Draw Player
        player.Draw();
        EndDrawing();
    }

    Platform::UnloadTileset();

    CloseWindow();
    return 0;
}
