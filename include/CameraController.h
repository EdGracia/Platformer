#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"

class CameraController {
    public:
        CameraController(float screenWidth, float screenHeight);

        void SetTarget(Vector2 newTarget);
        void Update(float deltaTime);
        void Apply() const;

        void SetFollowSmoothing(float factor);
        void SetZoom(float zoom);
        void UpdateLookAhead(float xVelocity);

    private:
        Camera2D camera;
        Vector2 target;
        float smoothing = 0.1f; // how fast it follows

        Vector2 lookAheadOffset = {0.0f, 0.0f};  // actual applied offset
        Vector2 desiredLookAhead = {0.0f, 0.0f}; // where we want it to go
        float lookAheadDistance = 75.0f;         // how far ahead to look
        float lookAheadSmoothing =
            0.03f; // how fast it follows movement direction
};

#endif
