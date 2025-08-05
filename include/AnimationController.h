#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include "raylib.h"

class AnimationController {
    public:
        AnimationController();

        void Set(int row, int frameCount, float frameTime);
        void Update(float deltaTime);
        Rectangle GetFrameSource(int frameWidth, int frameHeight,
                                 int facing) const;

    private:
        int currentFrame = 0;
        int totalFrames = 1;
        float timer = 0.0f;
        float frameTime = 0.1f;
        int row = 0;
};

#endif
