#include "CameraController.h"
#include "raymath.h" // for Vector2Lerp

CameraController::CameraController(float screenWidth, float screenHeight) {
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    camera.target = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}
Vector2 CameraController::GetTarget() const { return target; }

void CameraController::SetTarget(Vector2 newTarget) { target = newTarget; }

void CameraController::SetFollowSmoothing(float factor) { smoothing = factor; }

void CameraController::SetZoom(float zoom) { camera.zoom = zoom; }

void CameraController::Update(float deltaTime) {
    Vector2 targetWithLook = Vector2Add(target, lookAheadOffset);
    // Smoothly move the camera target toward the desired target
    camera.target = Vector2Lerp(camera.target, targetWithLook, smoothing);
}

void CameraController::Apply() const { BeginMode2D(camera); }

void CameraController::UpdateLookAhead(float xVelocity) {
    if (xVelocity > 0.1f)
        desiredLookAhead = {lookAheadDistance, 0.0f};
    else if (xVelocity < -0.1f)
        desiredLookAhead = {-lookAheadDistance, 0.0f};
    else
        desiredLookAhead = {0.0f, 0.0f};

    // Smoothly interpolate current offset
    lookAheadOffset =
        Vector2Lerp(lookAheadOffset, desiredLookAhead, lookAheadSmoothing);
}
