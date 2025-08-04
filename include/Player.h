#ifndef PLAYER_H
#define PLAYER_H

#include "Platform.h"
#include "raylib.h"
#include <vector>

class Player {
    public:
        Player(float x, float y);
        void Update(float deltaTime, const std::vector<Platform> &platforms);
        void Draw() const;
        Rectangle GetCollisionRect() const;
        ~Player();

    private:
        Texture2D texture;
        float width = 64;
        float height = 64;

        Vector2 position;
        Vector2 velocity;

        float moveSpeed = 200.0f;
        float gravity = 600.0f;
        float jumpForce = 400.0f;

        bool isOnGround = false;
        float coyoteTime = 0.1f; // Allow 100 ms of grace
        float timeSinceLeftGround = 0.0f;
        float jumpBufferTime = 0.1f; // 100ms of window to store jump input
        float timeSinceJumpPressed = jumpBufferTime + 1.0f;
};

#endif
