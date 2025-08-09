#include "Renderer.h"

Renderer::Renderer(int virtualW, int virtualH)
    : vW(virtualW),
      vH(virtualH) {}

Renderer::~Renderer() {
    if (canvas.id != 0)
        UnloadRenderTexture(canvas);
}

void Renderer::Initialize(int windowW, int windowH, const char *title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowW, windowH, title);

    canvas = LoadRenderTexture(vW, vH);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT); // crisp pixels

    OnResize();
}

void Renderer::Shutdown() {
    if (canvas.id != 0) {
        UnloadRenderTexture(canvas);
        canvas.id = 0;
    }
    CloseWindow();
}

void Renderer::OnResize() {
    winW = GetScreenWidth();
    winH = GetScreenHeight();

    // Integer scale that fits the window
    scale = winW / vW;
    int scaleH = winH / vH;
    if (scaleH < scale)
        scale = scaleH;
    if (scale < 1)
        scale = 1;

    int drawW = vW * scale;
    int drawH = vH * scale;

    offsetX = (winW - drawW) / 2;
    offsetY = (winH - drawH) / 2;

    // raylib RenderTexture is upside down — flip the source by using negative
    // height
    src = {0.0f, 0.0f, (float)vW, -(float)vH};
    dest = {(float)offsetX, (float)offsetY, (float)drawW, (float)drawH};
}

void Renderer::BeginWorld() {
    BeginTextureMode(canvas);
    // Clear here so each scene doesn't need to
    ClearBackground(SKYBLUE);
}

void Renderer::EndWorld() { EndTextureMode(); }

void Renderer::Present() {
    if (canvas.id == 0)
        return;
    BeginDrawing();
    ClearBackground(BLACK); // letterbox bars
    DrawTexturePro(canvas.texture, src, dest, {0, 0}, 0.0f, WHITE);
    EndDrawing();
}

Vector2 Renderer::GetVirtualMouse() const {
    Vector2 m = GetMousePosition();
    m.x -= (float)offsetX;
    m.y -= (float)offsetY;
    if (scale > 0) {
        m.x /= (float)scale;
        m.y /= (float)scale;
    }
    // Clamp to virtual canvas
    if (m.x < 0)
        m.x = 0;
    if (m.y < 0)
        m.y = 0;
    if (m.x > vW)
        m.x = (float)vW;
    if (m.y > vH)
        m.y = (float)vH;
    return m;
}

Vector2 Renderer::VirtualToWorld(Vector2 vpos, const Camera2D &cam) const {
    // vpos is in virtual screen space; convert to world using the camera
    return GetScreenToWorld2D(vpos, cam);
}
