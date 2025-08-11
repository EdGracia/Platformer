#include "Physics.h"
#include "Platform.h" // for GetBounds()

namespace Physics {

bool ShouldLandOnPlatform(const Rectangle &now, const Rectangle &future,
                          const Rectangle &plat, float vy, float feetPad) {
    if (vy <= 0.0f)
        return false; // only when falling
    // was entirely above the top (allow a tiny pad for feet)
    bool wasAbove = (now.y + now.height) <= (plat.y + feetPad);
    // will cross or touch the top next frame
    bool willCross = (future.y + future.height) >= plat.y;
    // horizontal overlap
    bool xOverlap =
        (now.x + now.width) > plat.x && now.x < (plat.x + plat.width);
    return wasAbove && willCross && xOverlap;
}

bool ResolveVerticalOneWay(Vector2 &position, float &vy, const Rectangle &now,
                           const Rectangle &plat, float hitboxHeight,
                           float desiredScale, float feetPad) {
    // Offset from logical position to hitbox top
    float offsetY = now.y - position.y;

    // Put feet on platform: top of hitbox = plat.y
    position.y = plat.y - now.height - offsetY;
    vy = 0.0f;
    return true;
}

bool ResolveHorizontalSolid(Vector2 &position, float &vx, const Rectangle &now,
                            const Rectangle &plat, float dt) {
    // Must overlap vertically to hit a side
    bool yOverlap =
        (now.y < plat.y + plat.height) && (now.y + now.height > plat.y);
    if (!yOverlap)
        return false;

    // Predict X movement only
    Rectangle futureX{now.x + vx * dt, now.y, now.width, now.height};

    // Offset from logical position to hitbox left
    float offsetX = now.x - position.x;

    // Moving right: crossing plat's left edge
    if (vx > 0.0f && now.x + now.width <= plat.x &&
        futureX.x + futureX.width > plat.x) {
        position.x = plat.x - now.width - offsetX;
        vx = 0.0f;
        return true;
    }

    // Moving left: crossing plat's right edge
    if (vx < 0.0f && now.x >= plat.x + plat.width &&
        futureX.x < plat.x + plat.width) {
        position.x = plat.x + plat.width - offsetX;
        vx = 0.0f;
        return true;
    }

    return false;
}

void DrawDebug(const Rectangle &now, const Rectangle &future,
               const std::vector<Platform> &platforms,
               const std::vector<Rectangle> &hits) {
    // Player now
    DrawRectangleLinesEx(now, 1, Fade(RED, 0.9f));
    // Player future
    DrawRectangleLinesEx(future, 1, Fade(BLUE, 0.9f));
    // Platforms
    for (const auto &p : platforms) {
        DrawRectangleLinesEx(p.GetBounds(), 1, Fade(DARKGREEN, 0.8f));
    }
    // Hits if provided
    for (auto &r : hits) {
        DrawRectangleLinesEx(r, 2, YELLOW);
    }
}

} // namespace Physics
