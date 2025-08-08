#pragma once
#include "Scene.h"

class SceneManager;

class TitleScene : public Scene {
    public:
        TitleScene(int w, int h, SceneManager *router);

        void OnEnter() override {}
        void OnExit() override {}

        void HandleInput() override;
        void Update(float dt) override;
        void Draw() const override;

    private:
        int screenW, screenH;
        SceneManager *router; // we’ll include its header in the .cpp
};
