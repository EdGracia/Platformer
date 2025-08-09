#include "GameScene.h"
#include "raylib.h"

GameScene::GameScene(int w, int h)
    : screenW(w),
      screenH(h),
      player(10, 200),
      camera((float)w, (float)h) {}

void GameScene::OnEnter() {

    Platform::LoadTileset();
    camera.SetZoom(.1);

    // If you keep a local tileset handle (optional):

    bgTileset = LoadTexture("assets/Backgrounds/1B_Background.png");
    SetTextureFilter(bgTileset, TEXTURE_FILTER_POINT);

    // Level geometry…
    platforms.clear();

    player.SetPlatforms(platforms);

    // Build background layers (example setup)
    bgLayers.clear();
    bgLayers.emplace_back(
        /* tileset   */ bgTileset /* or Platform::tileset if public */,
        /* tileSize  */ 32,
        /* gridCols  */ 5,
        /* gridRows  */ 5,
        /* origin    */ Vector2{400.0f, 1000.0f},
        /* factor    */ 0.01f);

    bgLayers.emplace_back(bgTileset, 32, 2, 2, Vector2{300.0f, 280.0f}, 0.01f);

    // Build your level
    platforms.clear();
    platforms.push_back(Platform(-500, 500, 1600, 95)); // Ground
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
    Platform::UnloadTileset();
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
    ClearBackground(Color{0xd8, 0xe4, 0xf4, 0xff});

    camera.Apply(); // BeginMode2D inside

    Vector2 camTarget = camera.GetTarget();
    // Draw background layers first (far → near)
    for (const auto &layer : bgLayers)
        layer.Draw(camTarget);
    // Game Objects
    for (const auto &p : platforms)
        p.Draw();
    player.Draw();

    EndMode2D();

    // UI (not affected by camera)
    DrawText("Pixel Platformer", 10, 10, 20, WHITE);

    EndDrawing();
}
