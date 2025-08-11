#include "Renderer.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "raylib.h"

int main() {
    // Renderer/Window Setup
    const int VIRTUAL_W = 1280;
    const int VIRTUAL_H = 740;

    Renderer renderer(VIRTUAL_W, VIRTUAL_H);
    renderer.Initialize(VIRTUAL_W, VIRTUAL_H, "Pixel Platformer");
    SetTargetFPS(60);

    SceneManager scenes;
    scenes.ChangeScene(
        std::make_unique<TitleScene>(VIRTUAL_W, VIRTUAL_H, &scenes));
    while (!WindowShouldClose()) {
        if (IsWindowResized())
            renderer.OnResize();
        float dt = GetFrameTime();
        scenes.Update(dt);
        renderer.BeginWorld();
        // Your scenes should draw *world* (gameplay) here
        scenes.Draw();
        // it should scale
        renderer.EndWorld();

        // Present by drawing directly to window.
        renderer.Present();
    }
    renderer.Shutdown();
    return 0;
}
