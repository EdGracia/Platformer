#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"

class GameObject {
    protected:
        Vector2 position;
        Vector2 velocity;
        float rotation = 0.0f;

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
};

#endif
