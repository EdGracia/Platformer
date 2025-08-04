#include "Player.h"
#include "iostream"
#include "raylib.h"

Player::Player(float x, float y) {
    position = {x, y};
    velocity = {0, 0};

    texture = LoadTexture("assets/player.png");
    // for pixel art sharpness
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}

void Player::Update(float deltaTime, const std::vector<Platform> &platforms) {
    // Horizontal movement
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

    timeSinceLastDash += deltaTime;
    if (IsKeyPressed(KEY_LEFT_SHIFT) && !isDashing &&
        timeSinceLastDash >= dashCooldown) {
        isDashing = true;
        dashTime = 0.0f;
        timeSinceLastDash = 0.0f;
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
    } else {
        if (IsKeyPressed(KEY_SPACE)) {
            timeSinceJumpPressed = 0.0f;
        } else {
            timeSinceJumpPressed += deltaTime;
        }
        // Gravity
        velocity.y += gravity * deltaTime;

        velocity.x = xMove;
    }

    // If jump is pressed, reset the timer
    // predict future position
    Rectangle playerRect = GetCollisionRect();
    float futureY = position.y + velocity.y * deltaTime;
    Rectangle futureRect = {playerRect.x, futureY, playerRect.width,
                            playerRect.height};

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
            position.y = platformBounds.y - playerRect.height;
            velocity.y = 0;
            isOnGround = true;
            break;
        }
    }

    if (isOnGround)
        timeSinceLeftGround = 0.0f;
    else
        timeSinceLeftGround += deltaTime;
    // Jump
    if (timeSinceLeftGround <= coyoteTime &&
        timeSinceJumpPressed <= jumpBufferTime) {
        velocity.y = -400;
        isOnGround = false;
        timeSinceLeftGround = coyoteTime + 1.0f;
    }

    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

Rectangle Player::GetCollisionRect() const {
    float hitboxWidth = 10 * desiredScale;
    float hitboxHeight = height;

    float offsetX = (width - hitboxWidth) / 2.0f;

    return Rectangle{position.x + offsetX, // center the narrow hitbox
                     position.y, hitboxWidth, hitboxHeight};
}

void Player::Draw() const {
    // Source: draw the full texture
    Rectangle source = {0.0f, 0.0f, (float)texture.width * facing,
                        (float)texture.height};

    // Destination: where and how big to draw on screen
    Rectangle dest = {position.x, position.y, width, height};

    // Origin: anchor point (top-left corner)
    Vector2 origin = {0.0f, 0.0f};

    // Rotation = 0, Color = WHITE (no tint)
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

    // TO SHOW HITBOX:
    // DrawRectangleRec(GetCollisionRect(), Fade(RED, 0.4f));
}

Player::~Player() { UnloadTexture(texture); }
