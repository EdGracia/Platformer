#include "GameScene.h"
#include "raylib.h"

GameScene::GameScene(int w, int h)
    : screenW(w),
      screenH(h),
      player(10, 200),
      camera((float)w, (float)h) {}

void GameScene::OnEnter() {
    // Load shared tileset (if not already) & any scene assets
    Platform::LoadTileset();

    // Build your level
    platforms.clear();
    platforms.push_back(Platform(-100, 500, 1600, 95)); // Ground
    platforms.push_back(Platform(100, 450, 64, 32));
    platforms.push_back(Platform(200, 410, 64, 32));
    platforms.push_back(Platform(300, 350, 64, 32));
    platforms.push_back(Platform(400, 300, 96, 32));
    platforms.push_back(Platform(100, 150, 96, 96));
    platforms.push_back(Platform(600, 250, 32, 32));

    // If Player needs platform access internally:
    player.SetPlatforms(platforms);
}

void GameScene::OnExit() {
    // Unload scene-local stuff if needed
    Platform::UnloadTileset(); // if tileset is globally shared, you may move
                               // this elsewhere
}

void GameScene::HandleInput() {
    // You can keep Player input in Player::Update if you prefer.
    // This is here if you eventually want scene-level input (pause menu, etc.)
}

void GameScene::Update(float dt) {
    // Update player
    player.Update(dt);

    // Update camera target/feel
    camera.SetTarget(player.GetCenter());
    camera.UpdateLookAhead(player.GetVelocity().x);
    camera.Update(dt);

    // Platforms inherit GameObject now—if you ever have moving platforms:
    for (auto &p : platforms)
        p.Update(dt);
}

void GameScene::Draw() const {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    camera.Apply(); // BeginMode2D inside

    // World
    for (const auto &p : platforms)
        p.Draw();
    player.Draw();

    EndMode2D();

    // UI (not affected by camera)
    DrawText("Pixel Platformer", 10, 10, 20, WHITE);

    EndDrawing();
}
