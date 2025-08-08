#pragma once
#include "CameraController.h"
#include "Platform.h"
#include "Player.h"
#include "Scene.h"
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

        Player player;
        std::vector<Platform> platforms;
        CameraController camera;
        // any scene-local flags/vars go here
};
