#pragma once

class Scene {

    public:
        virtual ~Scene() = default;

        // Lifecycle
        virtual void OnEnter() {}
        virtual void OnExit() {}

        // Per-frame
        virtual void HandleInput() {}
        virtual void Update(float deltaTime) = 0;
        virtual void Draw() const = 0;
};
