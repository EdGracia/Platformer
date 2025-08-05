#ifndef PLAYER_H
#define PLAYER_H

#include "AnimationController.h"
#include "Platform.h"
#include "raylib.h"
#include <vector>

enum class PlayerState { Idle, Walking, Jumping, Falling, Dashing };

class Player {
    public:
        Player(float x, float y);
        void Update(float deltaTime, const std::vector<Platform> &platforms);
        void Draw() const;
        void HandleInput(float deltaTime);
        void UpdateDash(float deltaTime);
        void ApplyGravity(float deltaTime);
        void ResolvePlatformCollisions(const std::vector<Platform> &platforms,
                                       float deltaTime);
        void UpdateState();
        void HandleJump(float deltaTime);
        Rectangle GetCollisionRect() const;
        ~Player();

    private:
        static constexpr int SPRITE_FRAME_WIDTH = 128;
        static constexpr int SPRITE_FRAME_HEIGHT = 128;

        PlayerState state = PlayerState::Idle;

        float desiredScale = 2;
        float width = 128 * desiredScale;
        float height = 128 * desiredScale;
        float hitboxWidth = 18.0f;
        float hitboxHeight = 44.0f;

        float offsetX = 0.0f;
        float offsetY = 0.0f;

        // Animation for idle
        AnimationController animation;
        Texture2D idleTexture;

        int facing = 1; // Direction facing 1 = right -1 = left

        Vector2 position;
        Vector2 velocity;
        float xMove = 0.0f;

        float moveSpeed = 200.0f;
        float gravity = 600.0f;
        float jumpForce = -350.0f;

        float coyoteTime = 0.1f; // Allow 100 ms of grace
        float timeSinceLeftGround = 0.0f;
        float jumpBufferTime = 0.1f; // 100ms of window to store jump input
        float timeSinceJumpPressed = jumpBufferTime + 1.0f;

        bool hasDashed = false;
        float dashTime = 0.0f;
        const float dashDuration = 0.15f;
        const float dashSpeed = 500.0f;
        const float dashCooldown = 0.5f;
        float timeSinceLastDash = dashCooldown + 1.0f;
        Vector2 dashDirection = {0, 0};
};

#endif
