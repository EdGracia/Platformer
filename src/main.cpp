#include "SceneManager.h"
#include "TitleScene.h"
#include "raylib.h"

int main() {
    // Window Setup
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Pixel Platformer");
    SetTargetFPS(60);

    SceneManager scenes;

    // Start on TitleScene. We pass a pointer to the manager (“router”) in.
    scenes.ChangeScene(
        std::make_unique<TitleScene>(screenWidth, screenHeight, &scenes));

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        scenes.Update(dt);
        scenes.Draw();
    }
    CloseWindow();
}
