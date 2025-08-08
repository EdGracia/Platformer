#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "raylib.h"

TitleScene::TitleScene(int w, int h, SceneManager *r)
    : screenW(w),
      screenH(h),
      router(r) {}

void TitleScene::HandleInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        router->ChangeScene(std::make_unique<GameScene>(screenW, screenH));
    }
}

void TitleScene::Update(float /*dt*/) {
    // nothing for now
}

void TitleScene::Draw() const {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Press ENTER to Start", 200, 260, 30, RAYWHITE);
    EndDrawing();
}
