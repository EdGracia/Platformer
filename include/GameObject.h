#pragma once
#include "Types.h"
#include "raylib.h"

class GameObject {
    public:
        GameObject(float x, float y);
        virtual ~GameObject() = default;

        virtual void Update(
            float deltaTime) = 0; //=0 means every this func must be implemented
                                  // in any class that inherits GAMEOBJECT
        virtual void Draw() const = 0;

        Vector2 GetPosition() const;
        void SetPosition(Vector2 pos);

        Vector2 GetVelocity() const;
        void SetVelocity(Vector2 velo);

        float GetRotation() const;
        void SetRotation(float r);
        // Common transform
        Vector2 position{0, 0};
        Vector2 velocity{0, 0};
        float rotation = 0.0f;

        // New metadata
        Layer layer = Layer::Foreground;
        Tag tag = Tag::None;

        bool active = true; // for soft-deletion

        // Tiny helpers
        // bool IsOnLayer(Layer l) const { return layer == l; }
        bool HasTag(Tag t) const { return tag == t; }
};
