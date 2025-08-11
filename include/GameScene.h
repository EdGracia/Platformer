#pragma once
#include "CameraController.h"
#include "Platform.h"
#include "Player.h"
#include "Scene.h"
#include "TilesetResolver.h"
#include <vector>

class GameScene : public Scene {
    public:
        GameScene(int screenW, int screenH);

        void OnEnter() override;
        void OnExit() override;

        void HandleInput() override;
        void Update(float dt) override;
        void Draw() const override;

    private:
        int screenW, screenH;

        // Game objects
        Player player;
        std::vector<Platform> fgPlatforms;
        std::vector<Platform> bgPlatforms;
        CameraController camera;
        // any scene-local flags/vars go here
        bool debugPhysics = false;

        TilesetResolver tilesets;
        Texture2D fgTileset{};
        Texture2D bgTileset{}; // if you keep a handle here
};
