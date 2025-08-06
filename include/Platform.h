#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"

class Platform {
    public:
        Platform(float x, float y, float width, float height);
        void Draw() const;
        Rectangle GetBounds() const;

        static void LoadTileset();
        static void UnloadTileset();

        bool IsFloatingPlatform() const;
        bool IsSingleTilePlatform() const;
        int GetTileRow(int y) const;
        int GetTileCol(int x) const;

    private:
        Vector2 position;
        float width, height;

        static Texture2D tileset;
        static constexpr int TILE_SIZE = 32;
};

#endif
