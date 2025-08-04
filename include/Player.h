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
        float desiredScale = 2;
        Texture2D texture;
        float width = 32 * desiredScale;
        float height = 32 * desiredScale;

        int facing = 1; // Direction facing 1 = right -1 = left

        Vector2 position;
        Vector2 velocity;
        float xMove = 0.0f;

        float moveSpeed = 200.0f;
        float gravity = 600.0f;
        float jumpForce = 400.0f;

        bool isOnGround = false;
        float coyoteTime = 0.1f; // Allow 100 ms of grace
        float timeSinceLeftGround = 0.0f;
        float jumpBufferTime = 0.1f; // 100ms of window to store jump input
        float timeSinceJumpPressed = jumpBufferTime + 1.0f;

        bool isDashing = false;
        float dashTime = 0.0f;
        const float dashDuration = 0.15f;
        const float dashSpeed = 500.0f;
        const float dashCooldown = 0.5f;
        float timeSinceLastDash = dashCooldown + 1.0f;
        Vector2 dashDirection = {0, 0};
};

#endif
