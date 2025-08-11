#ifndef PLAYER_H
#define PLAYER_H

#include "AnimationController.h"
#include "GameObject.h"
#include "Platform.h"
#include "raylib.h"
#include <vector>

enum class PlayerState {
    None,
    Idle,
    Walking,
    Jumping,
    Falling,
    Landing,
    Dashing
};

class Player : public GameObject {
    public:
        Player(float x, float y);
        ~Player();

        // Getters
        Vector2 GetPosition() const;
        float GetWidth() const;
        float GetHeight() const;
        Vector2 GetCenter() const;
        Vector2 GetVelocity() const;

        // Setters
        void SetPosition(float x, float y);
        void SetPlatforms(std::vector<Platform> &platforms_);

        void Update(float deltaTime, const std::vector<Platform> &platforms);
        void Update(float deltaTime) override;
        void Draw() const override;
        void HandleInput(float deltaTime);
        void UpdateDash(float deltaTime);
        // void ApplyGravity(float deltaTime);

        // void
        // ResolveHorizontalCollisions(const std::vector<Platform> &platforms);
        // void ResolveVerticalCollisions(const std::vector<Platform>
        // &platforms, float deltaTime);
        void UpdateState();
        void HandleJump(float deltaTime);
        Rectangle GetCollisionRect() const;

    private:
        static constexpr int SPRITE_FRAME_WIDTH = 128;
        static constexpr int SPRITE_FRAME_HEIGHT = 128;

        PlayerState state = PlayerState::Idle;
        PlayerState lastState = PlayerState::Idle;

        float desiredScale = 1.5;
        float width = 128 * desiredScale;
        float height = 128 * desiredScale;
        float hitboxWidth = 18.0f;
        float hitboxHeight = 44.0f;

        float offsetX = 0.0f;
        float offsetY = 0.0f;

        // Animation for idle
        AnimationController animation;
        Texture2D currentTexture;
        Texture2D idleTexture;
        Texture2D walkTexture;
        Texture2D jumpTexture;
        Texture2D landTexture;

        int facing = 1; // Direction facing 1 = right -1 = left

        // Factor of horizontal movement
        float xMove = 0.0f;

        float moveSpeed = 200.0f;
        float gravity = 850.0f;
        float jumpForce = -400.0f;

        // for jump
        float coyoteTime = 0.1f; // Allow 100 ms of grace
        float timeSinceLeftGround = 0.0f;
        float jumpBufferTime = 0.1f; // 100ms of window to store jump input
        float timeSinceJumpPressed = jumpBufferTime + 1.0f;
        // for dash
        bool hasDashed = false;
        float dashTime = 0.0f;
        const float dashDuration = 0.2f;
        const float dashSpeed = 550.0f;
        const float dashCooldown = 0.5f;
        float timeSinceLastDash = dashCooldown + 1.0f;
        Vector2 dashDirection = {0, 0};

        std::vector<Platform> platforms;
};

#endif
