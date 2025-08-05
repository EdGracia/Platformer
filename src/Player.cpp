#include "Player.h"
#include "iostream"
#include "raylib.h"

Player::Player(float x, float y) {
    position = {x, y};
    velocity = {0, 0};

    // texture = LoadTexture("assets/player.png");
    idleTexture = LoadTexture("assets/player_animations/Idle.png");

    // for pixel art sharpness
    SetTextureFilter(idleTexture, TEXTURE_FILTER_POINT);

    idleFrameWidth = idleTexture.width / idleFrameCount;
    idleFrameHeight = idleTexture.height;

    float spriteWidth = idleFrameWidth * desiredScale;
    float spriteHeight = idleFrameHeight * desiredScale;

    offsetX = (spriteWidth - hitboxWidth * desiredScale) / 2.0f;
    offsetY = 46 * desiredScale;
}

void Player::Update(float deltaTime, const std::vector<Platform> &platforms) {
    // Horizontal movement input
    if (IsKeyDown(KEY_A))
        xMove = -moveSpeed;
    else if (IsKeyDown(KEY_D))
        xMove = moveSpeed;
    else
        xMove = 0.0f;

    if (xMove > 0)
        facing = 1;
    else if (xMove < 0)
        facing = -1;

    // Setting up dash variables
    timeSinceLastDash += deltaTime;

    if (IsKeyPressed(KEY_LEFT_SHIFT) && !isDashing &&
        timeSinceLastDash >= dashCooldown && !hasDashed) {
        isDashing = true;
        dashTime = 0.0f;
        timeSinceLastDash = 0.0f;
        hasDashed = true;
    }
    if (isDashing) {
        dashTime += deltaTime;
        // if not moving default dash right
        if (facing > 0)
            xMove = dashSpeed;
        else if (facing < 0)
            xMove = -dashSpeed;

        if (dashTime >= dashDuration) {
            isDashing = false;
        }
        velocity.x = xMove;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        timeSinceJumpPressed = 0.0f;
    } else {
        timeSinceJumpPressed += deltaTime;
    }
    // Gravity
    velocity.y += gravity * deltaTime;

    velocity.x = xMove;

    // predict future position
    Rectangle playerRect = GetCollisionRect();
    Rectangle futureRect = {playerRect.x, playerRect.y + velocity.y * deltaTime,
                            playerRect.width, playerRect.height};

    // Reset ground state
    isOnGround = false;

    // Check for platform collisions
    for (const Platform &platform : platforms) {
        Rectangle platformBounds = platform.GetBounds();
        bool fallingThroughPlatform =
            velocity.y > 0 &&
            playerRect.y + playerRect.height <=
                platformBounds.y && // player is above platform
            futureRect.y + futureRect.height >=
                platformBounds.y && // player will pass through platform top
            playerRect.x + playerRect.width > platformBounds.x &&
            playerRect.x < platformBounds.x + platformBounds.width;

        if (fallingThroughPlatform) {
            // Snap player to platform
            position.y = platformBounds.y - hitboxHeight * desiredScale;
            velocity.y = 0;
            isOnGround = true;
            break;
        }
    }

    if (isOnGround) {
        timeSinceLeftGround = 0.0f;
        hasDashed = false;
    } else
        timeSinceLeftGround += deltaTime;
    // Jump
    if (timeSinceLeftGround <= coyoteTime &&
        timeSinceJumpPressed <= jumpBufferTime) {
        velocity.y = jumpForce;
        isOnGround = false;
        timeSinceLeftGround = coyoteTime + 1.0f;
    }

    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Player::UpdateAnimation(float deltaTime) {
    idleFrameTimer += deltaTime;

    if (idleFrameTimer >= idleFrameTime) {
        idleFrameTimer = 0.0f;
        idleCurrentFrame = (idleCurrentFrame + 1) % idleFrameCount;
    }
}

Rectangle Player::GetCollisionRect() const {

    return Rectangle{position.x, position.y, hitboxWidth * desiredScale,
                     hitboxHeight * desiredScale};
}

void Player::Draw() const {
    // Source: draw the full texture
    Rectangle source = {(float)idleCurrentFrame * 128, 0.0f,
                        (float)128 * facing, 128};

    // Destination: where and how big to draw on screen
    Rectangle dest = {position.x - offsetX, position.y - offsetY,
                      128 * desiredScale * facing, 128 * desiredScale};

    // Origin: anchor point (top-left corner)
    Vector2 origin = {0.0f, 0.0f};

    // Rotation = 0, Color = WHITE (no tint)
    DrawTexturePro(idleTexture, source, dest, origin, 0.0f, WHITE);

    // TO SHOW HITBOX:
    DrawRectangleRec(GetCollisionRect(), Fade(RED, 0.4f));
    DrawRectangleLines(dest.x, dest.y, abs(dest.width), dest.height, BLUE);
}

Player::~Player() { UnloadTexture(idleTexture); }
