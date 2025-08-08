#pragma once
#include "Scene.h"
#include <memory>

class SceneManager {
    public:
        // Replace the current scene with a new one.
        // Ownership transfers to the manager.
        void ChangeScene(std::unique_ptr<Scene> next);

        void Update(float dt);
        void Draw() const;

        // Optional: clear everything
        void Clear();

    private:
        std::unique_ptr<Scene> current;
};
