#include "CameraController.h"
#include "Platform.h"
#include "Player.h"
#include "raylib.h"
#include "vector"

int main() {
    // Window Setup
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Pixel Platformer");
    SetTargetFPS(60);

    // Player Setup
    Player player(100, 200);
    Platform::LoadTileset();

    // Platform Setup
    std::vector<Platform> platforms;
    platforms.push_back(Platform(-100, 500, 1600, 95)); // Ground
    platforms.push_back(Platform(100, 450, 64, 32));
    platforms.push_back(Platform(200, 410, 64, 32));
    platforms.push_back(Platform(300, 350, 64, 32));
    platforms.push_back(Platform(400, 300, 96, 32));
    platforms.push_back(Platform(100, 150, 96, 96));
    platforms.push_back(Platform(600, 250, 32, 32));

    // Camera Setup
    CameraController camera((float)GetScreenWidth(), (float)GetScreenHeight());

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        float deltaTime = GetFrameTime();

        // cameraHandling
        camera.SetTarget(player.GetCenter());
        camera.UpdateLookAhead(player.GetVelocity().x);
        camera.Update(deltaTime);
        camera.Apply();

        // Player Handling
        player.Update(deltaTime, platforms);
        player.Draw();

        // Draw Platforms
        for (const Platform &platform : platforms) {
            platform.Draw();
        }

        EndMode2D();

        EndDrawing();
    }

    Platform::UnloadTileset();

    CloseWindow();
    return 0;
}
