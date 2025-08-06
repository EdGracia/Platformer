#ifndef PHYSICS_H
#define PHYSICS_H

#include "raylib.h"

namespace Physics {
bool ShouldLandOnPlatform(const Rectangle &player, const Rectangle &future,
                          const Rectangle &platform, float verticalVelocity);
bool IsTouchingSide(const Rectangle &playerFuture,
                    const Rectangle &platformBounds);
} // namespace Physics

#endif
