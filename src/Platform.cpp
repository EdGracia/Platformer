#include "Platform.h"
#include "raylib.h"
Platform::Platform(float x, float y, float width, float height)
    : x(x),
      y(y),
      width(width),
      height(height) {}

void Platform::Draw() const { DrawRectangle(int(x), int(y), int(width), int(height), DARKGRAY); }

Rectangle Platform::GetBounds() const { return Rectangle{x, y, width, height}; }
