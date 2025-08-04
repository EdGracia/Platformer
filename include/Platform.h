#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"

class Platform {
    public:
        Platform(float x, float y, float width, float height);
        void Draw() const;
        Rectangle GetBounds() const;

    private:
        float x, y;
        float width, height;
};

#endif
