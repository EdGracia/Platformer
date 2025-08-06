#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"

class GameObject {
    protected:
        Vector2 position;

    public:
        GameObject(float x, float y);
        virtual ~GameObject() = default;

        virtual void Update(
            float deltaTime) = 0; //=0 means every this func must be implemented
                                  // in any class that inherits GAMEOBJECT
        virtual void Draw() const = 0;

        Vector2 GetPosition() const;
        void SetPosition(Vector2 pos);
};

#endif
