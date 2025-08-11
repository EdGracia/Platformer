#include "Player.h"
#include "Physics.h"
#include "raylib.h"

Player::Player(float x, float y)
    : GameObject(x, y) {
    tag = Tag::Player;
    layer = Layer::Foreground;
    position = {x, y};
    velocity = {0, 0};

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

void Player::SetPosition(float x, float y) { position = {x, y}; }

void Player::SetPlatforms(std::vector<Platform> &platforms_) {
    platforms = platforms_;
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

void Player::UpdateState() {
    if (state != PlayerState::Dashing && state != PlayerState::Landing) {
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
        case PlayerState::None:
            break;
        case PlayerState::Idle:
            currentTexture = idleTexture;
            animation.Set(0, 3, 0.3f); // row, frame count, frame time
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
        case PlayerState::Landing:
            currentTexture = landTexture;
            animation.Set(0, 4, 0.1f);
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

void Player::Update(float deltaTime) { Update(deltaTime, platforms); }
void Player::Update(float deltaTime, const std::vector<Platform> &platforms) {
    animation.Update(deltaTime);
    HandleInput(deltaTime);
    UpdateDash(deltaTime);

    Physics::ApplyGravity(velocity.y, deltaTime, gravity);

    // Apply horizontal movement
    if (state != PlayerState::Dashing)
        velocity.x = xMove;

    Rectangle now = GetCollisionRect();
    Rectangle future = Physics::FutureRect(now, velocity, deltaTime);

    // Horizontal (side wall) collisions
    for (const Platform &p : platforms) {
        if (Physics::ResolveHorizontalSolid(position, velocity.x, now,
                                            p.GetBounds(), deltaTime)) {
            // update 'now' since position.x changed
            now = GetCollisionRect();
            future = Physics::FutureRect(now, velocity, deltaTime);
        }
    }

    // Upward (ceiling) collisions
    if (velocity.y < 0.0f) {
        for (const Platform &p : platforms) {
            if (Physics::ResolveCeilingSolid(position, velocity.y, now,
                                             p.GetBounds(), deltaTime)) {
                // Rebuild rects after snap
                now = GetCollisionRect();
                future = Physics::FutureRect(now, velocity, deltaTime);
                break;
            }
        }
    }

    // Vertical one way landing
    bool landed = false;
    for (const Platform &p : platforms) {
        Rectangle plat = p.GetBounds();
        if (Physics::ShouldLandOnPlatform(now, future, plat, velocity.y,
                                          /*feetPad=*/4.0f)) {
            Physics::ResolveVerticalOneWay(position, velocity.y, now, plat,
                                           hitboxHeight, desiredScale,
                                           /*feetPad=*/4.0f);
            landed = true;
            // reset state/timers on land
            timeSinceLeftGround = 0.0f;
            hasDashed = false;
            state = (xMove == 0) ? PlayerState::Idle : PlayerState::Walking;
            break;
        }
    }

    // Track coyote/jump buffer timers
    if (landed) {
        timeSinceLeftGround = 0.0f;
    } else {
        if (state == PlayerState::Falling || state == PlayerState::Jumping)
            timeSinceLeftGround += deltaTime;
    }

    // 7) Jump buffer + coyote check (your existing logic)
    if (timeSinceLeftGround <= coyoteTime &&
        timeSinceJumpPressed <= jumpBufferTime) {
        velocity.y = jumpForce;
        state = PlayerState::Jumping;
        timeSinceLeftGround = coyoteTime + 1.0f; // consume
    }

    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

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
