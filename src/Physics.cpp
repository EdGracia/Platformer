#include "Physics.h"

namespace Physics {

bool ShouldLandOnPlatform(const Rectangle &player, const Rectangle &future,
                          const Rectangle &platform, float verticalVelocity) {
    return verticalVelocity > 0 &&
           player.y + player.height <= platform.y && // currently above
           future.y + future.height >= platform.y && // will cross platform top
           player.x + player.width > platform.x &&   // horizontal overlap
           player.x < platform.x + platform.width;
}

bool IsTouchingSide(const Rectangle &playerFuture, const Rectangle &platform) {
    return CheckCollisionRecs(playerFuture, platform);
}

} // namespace Physics
