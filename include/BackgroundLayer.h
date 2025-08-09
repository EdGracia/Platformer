#pragma once
#include "raylib.h"

class BackgroundLayer {
    public:
        BackgroundLayer(
            Texture2D tileset, int tileSize, int gridCols, int gridRows,
            Vector2 originWorldPos, // where the grid starts in world coords
            float parallaxFactor);  // 0..1 (smaller = farther)

        void Draw(Vector2 cameraTarget) const;

    private:
        Texture2D tileset;
        int tileSize;
        int cols, rows;
        Vector2 origin;
        float factor;

        // helpers to pick which tile from the sheet to draw
        Rectangle TileSrc(int col, int row) const;
};
