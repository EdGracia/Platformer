#pragma once
#include "raylib.h"
#include <vector>

struct Platform; // forward declare if needed

namespace Physics {

// AABB helper
inline bool AABB(const Rectangle &a, const Rectangle &b) {
    return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height &&
           a.y + a.height > b.y;
}

// Gravity (clamps to terminal)
inline void ApplyGravity(float &vy, float dt, float g,
                         float terminal = 1200.0f) {
    vy += g * dt;
    if (vy > terminal)
        vy = terminal;
}

// Predict rect after velocity*dt
inline Rectangle FutureRect(const Rectangle &now, Vector2 vel, float dt) {
    return Rectangle{now.x + vel.x * dt, now.y + vel.y * dt, now.width,
                     now.height};
}

// One-way landing rule (top surface only)
bool ShouldLandOnPlatform(const Rectangle &now, const Rectangle &future,
                          const Rectangle &plat, float vy,
                          float feetPad = 0.0f);

// Snap to top; zero vy; write back to position.y
bool ResolveVerticalOneWay(Vector2 &position, float &vy, const Rectangle &now,
                           const Rectangle &plat, float hitboxHeight,
                           float desiredScale, float feetPad = 0.0f);

// Optional: collide with platform sides as solid walls.
// We only resolve if there is vertical overlap (so you can't stick to the
// underside).
bool ResolveHorizontalSolid(Vector2 &position, float &vx, const Rectangle &now,
                            const Rectangle &plat, float dt);

bool ResolveCeilingSolid(Vector2 &position, float &vy, const Rectangle &now,
                         const Rectangle &plat, float dt);

// Debug overlay (call from scene when toggled)
void DrawDebug(const Rectangle &now, const Rectangle &future,
               const std::vector<Platform> &platforms,
               const std::vector<Rectangle> &hits = {});

} // namespace Physics
