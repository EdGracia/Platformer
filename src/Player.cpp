#include "Player.h"
#include "raylib.h"

Player::Player(float x, float y) {
    position = {x, y};
    velocity = {0, 0};

    // texture = LoadTexture("assets/player.png");
    idleTexture = LoadTexture("assets/player_animations/Idle.png");
    walkTexture = LoadTexture("assets/player_animations/Sprint.png");
    jumpTexture = LoadTexture("assets/player_animations/Jump.png");
    landTexture = LoadTexture("assets/player_animations/Land on Ground.png");

    // for pixel art sharpness
    SetTextureFilter(idleTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(walkTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(jumpTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(landTexture, TEXTURE_FILTER_POINT);

    currentTexture = idleTexture;
    animation.Set(0, 4, 0.2f);
    lastState = PlayerState::None;

    float spriteWidth = SPRITE_FRAME_WIDTH * desiredScale;

    offsetX = (spriteWidth - hitboxWidth * desiredScale) / 2.0f;
    offsetY = 46 * desiredScale;
}

Vector2 Player::GetPosition() const { return position; }

float Player::GetWidth() const { return hitboxWidth * desiredScale; }

float Player::GetHeight() const { return hitboxHeight * desiredScale; }

Vector2 Player::GetCenter() const {
    return {position.x + GetWidth() / 2, position.y + GetHeight() / 2};
}

Vector2 Player::GetVelocity() const { return velocity; }

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

void Player::ResolveHorizontalCollisions(
    const std::vector<Platform> &platforms) {
    Rectangle playerRect = GetCollisionRect();

    for (const Platform &platform : platforms) {
        Rectangle platformBounds = platform.GetBounds();

        if (CheckCollisionRecs(playerRect, platformBounds)) {
            if (velocity.x > 0) {
                // Moving right → hit left side of platform
                position.x = platformBounds.x - playerRect.width;
            } else if (velocity.x < 0) {
                // Moving left → hit right side of platform
                position.x = platformBounds.x + platformBounds.width;
            }

            velocity.x = 0;
            break;
        }
    }
}

void Player::ResolveVerticalCollisions(const std::vector<Platform> &platforms,
                                       float deltaTime) {
    Rectangle playerRect = GetCollisionRect();

    bool landed = false;

    for (const Platform &platform : platforms) {
        Rectangle platformBounds = platform.GetBounds();

        if (CheckCollisionRecs(playerRect, platformBounds)) {
            if (velocity.y > 0) {
                // Falling → land on platform
                position.y = platformBounds.y - playerRect.height;
                velocity.y = 0;
                landed = true;

                // Only switch state if not dashing
                if (state != PlayerState::Dashing) {
                    state =
                        (xMove == 0) ? PlayerState::Idle : PlayerState::Walking;
                }

                timeSinceLeftGround = 0.0f;
                hasDashed = false;
            } else if (velocity.y < 0) {
                // Jumping → hit ceiling
                position.y = platformBounds.y + platformBounds.height;
                velocity.y = 0;
            }

            break;
        }
    }

    if (!landed && state != PlayerState::Dashing) {
        // We're in the air → falling or jumping
        if (velocity.y < 0)
            state = PlayerState::Jumping;
        else if (velocity.y > 0)
            state = PlayerState::Falling;

        timeSinceLeftGround += deltaTime;
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
    // Animation Controller
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
            currentTexture = jumpTexture;
            animation.Set(0, 4, 0.3f);
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
    ApplyGravity(deltaTime);

    // Apply horizontal movement
    if (state != PlayerState::Dashing)
        velocity.x = xMove;

    // Update position
    position.x += velocity.x * deltaTime;
    ResolveHorizontalCollisions(platforms);
    position.y += velocity.y * deltaTime;
    ResolveVerticalCollisions(platforms, deltaTime);

    HandleJump(deltaTime);
    UpdateState();
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

Player::~Player() {
    UnloadTexture(idleTexture);
    UnloadTexture(walkTexture);
    UnloadTexture(jumpTexture);
    UnloadTexture(landTexture);
}
