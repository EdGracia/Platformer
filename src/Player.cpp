#include "Player.h"
#include "Physics.h"
#include "iostream"
#include "raylib.h"

Player::Player(float x, float y) {
    position = {x, y};
    velocity = {0, 0};

    // texture = LoadTexture("assets/player.png");
    idleTexture = LoadTexture("assets/player_animations/Idle.png");
    walkTexture = LoadTexture("assets/player_animations/Walk.png");
    jumpTexture = LoadTexture("assets/player_animations/Jump.png");

    // for pixel art sharpness
    SetTextureFilter(idleTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(walkTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(jumpTexture, TEXTURE_FILTER_POINT);

    // idle anim
    animation.Set(0, 4, 0.2f);

    float spriteWidth = SPRITE_FRAME_WIDTH * desiredScale;

    offsetX = (spriteWidth - hitboxWidth * desiredScale) / 2.0f;
    offsetY = 46 * desiredScale;
}

void Player::HandleInput(float deltaTime) {
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

    if (IsKeyPressed(KEY_LEFT_SHIFT) && state != PlayerState::Dashing &&
        timeSinceLastDash >= dashCooldown && !hasDashed) {
        state = PlayerState::Dashing;
        dashTime = 0.0f;
        timeSinceLastDash = 0.0f;
        hasDashed = true;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        timeSinceJumpPressed = 0.0f;
    } else {
        timeSinceJumpPressed += deltaTime;
    }
}

void Player::UpdateDash(float deltaTime) {
    if (state == PlayerState::Dashing) {
        dashTime += deltaTime;

        if (facing > 0)
            velocity.x = dashSpeed;
        else if (facing < 0)
            velocity.x = -dashSpeed;

        if (dashTime >= dashDuration)
            state = PlayerState::Falling; // or Jumping if still rising
    }
}

void Player::ApplyGravity(float deltaTime) {

    velocity.y += gravity * deltaTime;
}

void Player::ResolvePlatformCollisions(const std::vector<Platform> &platforms,
                                       float deltaTime) {
    // predict future position
    Rectangle playerRect = GetCollisionRect();
    std::cout << "deltaTime in collision: " << deltaTime << std::endl;
    Rectangle futureRect = {playerRect.x, playerRect.y + velocity.y * deltaTime,
                            playerRect.width, playerRect.height};

    // Check for platform collisions
    for (const Platform &platform : platforms) {
        Rectangle platformBounds = platform.GetBounds();

        bool fallingThroughPlatform = Physics::ShouldLandOnPlatform(
            playerRect, futureRect, platformBounds, velocity.y);
        if (fallingThroughPlatform) {
            // Snap player to platform
            position.y = platformBounds.y - hitboxHeight * desiredScale;
            velocity.y = 0;

            if (state != PlayerState::Dashing)
                state = (xMove == 0) ? PlayerState::Idle : PlayerState::Walking;

            timeSinceLeftGround = 0.0f;
            hasDashed = false;
            break;
        }
    }
}

void Player::UpdateState() {
    if (state != PlayerState::Dashing) {
        if (velocity.y < 0) {
            state = PlayerState::Jumping;
        } else if (velocity.y > 0) {
            state = PlayerState::Falling;
        } else if (xMove != 0) {
            state = PlayerState::Walking;
        } else {
            state = PlayerState::Idle;
        }
    }
    if (state != lastState) {
        switch (state) {
        case PlayerState::Idle:
            currentTexture = idleTexture;
            animation.Set(0, 3, 0.2f); // row, frame count, frame time
            break;
        case PlayerState::Walking:
            currentTexture = walkTexture;
            animation.Set(0, 7, 0.1f);
            break;
        case PlayerState::Jumping:
            currentTexture = jumpTexture;
            animation.Set(0, 4, 0.2f); // Often just one frame
            break;
        case PlayerState::Falling:
            break;
        case PlayerState::Dashing:
            break;
        }
        lastState = state;
    }
}

void Player::HandleJump(float deltaTime) {
    if (state == PlayerState::Falling || state == PlayerState::Jumping) {
        timeSinceLeftGround += deltaTime;
    }

    if (timeSinceLeftGround <= coyoteTime &&
        timeSinceJumpPressed <= jumpBufferTime) {
        velocity.y = jumpForce;
        state = PlayerState::Jumping;
        timeSinceLeftGround = coyoteTime + 1.0f;
    }
}

void Player::Update(float deltaTime, const std::vector<Platform> &platforms) {
    animation.Update(deltaTime);
    HandleInput(deltaTime);
    UpdateDash(deltaTime);

    // Apply horizontal movement
    if (state != PlayerState::Dashing)
        velocity.x = xMove;

    ApplyGravity(deltaTime);
    ResolvePlatformCollisions(platforms, deltaTime);
    HandleJump(deltaTime);
    UpdateState();

    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

Rectangle Player::GetCollisionRect() const {

    return Rectangle{position.x, position.y, hitboxWidth * desiredScale,
                     hitboxHeight * desiredScale};
}

void Player::Draw() const {
    // Source: draw the full texture
    Rectangle source = animation.GetFrameSource(SPRITE_FRAME_WIDTH,
                                                SPRITE_FRAME_HEIGHT, facing);
    // Destination: where and how big to draw on screen
    Rectangle dest = {position.x - offsetX, position.y - offsetY,
                      SPRITE_FRAME_WIDTH * desiredScale * facing,
                      SPRITE_FRAME_HEIGHT * desiredScale};
    // Origin: anchor point (top-left corner)
    Vector2 origin = {0.0f, 0.0f};
    // Rotation = 0, Color = WHITE (no tint)
    DrawTexturePro(currentTexture, source, dest, origin, 0.0f, WHITE);
    // TO SHOW HITBOX:
    // DrawRectangleRec(GetCollisionRect(), Fade(RED, 0.4f));
    // DrawRectangleLines(dest.x, dest.y, abs(dest.width), dest.height, BLUE);
}

Player::~Player() { UnloadTexture(idleTexture); }
