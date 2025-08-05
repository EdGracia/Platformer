#include "AnimationController.h"

AnimationController::AnimationController() {}

void AnimationController::Set(int rowIndex, int frameCount,
                              float frameDuration) {
    row = rowIndex;
    totalFrames = frameCount;
    frameTime = frameDuration;
    currentFrame = 0;
    timer = 0.0f;
}

void AnimationController::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= frameTime) {
        timer = 0.0f;
        currentFrame = (currentFrame + 1) % totalFrames;
    }
}

Rectangle AnimationController::GetFrameSource(int frameWidth, int frameHeight,
                                              int facing) const {
    return Rectangle{(float)currentFrame * frameWidth, (float)row * frameHeight,
                     (float)frameWidth * facing, (float)frameHeight};
}
