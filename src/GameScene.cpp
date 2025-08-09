#include "GameScene.h"
#include "raylib.h"

GameScene::GameScene(int w, int h)
    : screenW(w),
      screenH(h),
      player(10, 200),
      camera((float)w, (float)h) {}

void GameScene::OnEnter() {

    // If you keep a local tileset handle (optional):

    bgTileset = LoadTexture("assets/Platforms/1B_Background.png");
    SetTextureFilter(bgTileset, TEXTURE_FILTER_POINT);

    fgTileset = LoadTexture("assets/Platforms/1B_Foreground.png");
    SetTextureFilter(bgTileset, TEXTURE_FILTER_POINT);

    // Level geometry…
    bgPlatforms.clear();
    bgPlatforms.push_back(Platform(100, 100, 64, 64, bgTileset));

    player.SetPlatforms(platforms);

    // Build your level
    platforms.clear();
    platforms.push_back(Platform(-500, 500, 1600, 95, fgTileset)); // Ground
    platforms.push_back(Platform(100, 450, 64, 32, fgTileset));
    platforms.push_back(Platform(200, 410, 64, 32, fgTileset));
    platforms.push_back(Platform(300, 350, 64, 32, fgTileset));
    platforms.push_back(Platform(400, 300, 96, 32, fgTileset));
    platforms.push_back(Platform(100, 150, 96, 96, fgTileset));
    platforms.push_back(Platform(600, 250, 32, 32, fgTileset));

    // If Player needs platform access internally:
    player.SetPlatforms(platforms);
}

void GameScene::OnExit() {
    UnloadTexture(fgTileset);
    UnloadTexture(bgTileset);
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

    // Grab the main, already-smoothed camera as our base
    Camera2D base = camera.GetRaw();
    Vector2 mainTarget = base.target;

    // === FAR BACKGROUND (moves least) ===
    Camera2D farCam = base;
    farCam.target = {mainTarget.x * 0.30f,
                     mainTarget.y * 0.10f}; // small vertical factor
    BeginMode2D(farCam);
    for (const auto &p : bgPlatforms)
        p.Draw(); // no changes inside Platform
    EndMode2D();

    camera.Apply(); // BeginMode2D inside
    // Game Objects
    for (const auto &p : platforms)
        p.Draw();
    player.Draw();

    EndMode2D();

    // UI (not affected by camera)
    DrawText("Pixel Platformer", 10, 10, 20, WHITE);

    EndDrawing();
}
