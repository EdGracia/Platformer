#pragma once
#include "raylib.h"

class Renderer {
    public:
        Renderer(int virtualW, int virtualH);
        ~Renderer();

        void Initialize(int windowW, int windowH, const char *title);
        void Shutdown();

        // Call when the OS window is resized
        void OnResize();

        // World rendering into the virtual canvas
        void BeginWorld();
        void EndWorld();

        // Blit the canvas to the real window (with letterboxing/integer scale)
        void Present();

        // Helpers
        int GetVirtualWidth() const { return vW; }
        int GetVirtualHeight() const { return vH; }
        int GetScale() const { return scale; }
        Rectangle GetDestRect() const { return dest; }
        Rectangle GetSrcRect() const { return src; }

        // Convert mouse to virtual canvas coords
        Vector2 GetVirtualMouse() const;
        // Convert (virtual) screen coords to world with a Camera2D
        Vector2 VirtualToWorld(Vector2 vpos, const Camera2D &cam) const;

    private:
        int vW, vH;               // virtual canvas size
        RenderTexture2D canvas{}; // the virtual framebuffer

        // computed each frame or when resized
        int winW = 0, winH = 0;
        int scale = 1;
        int offsetX = 0, offsetY = 0;
        Rectangle src{};  // source rect from canvas
        Rectangle dest{}; // destination rect on window
};
