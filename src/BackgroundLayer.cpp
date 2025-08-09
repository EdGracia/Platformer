#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer(Texture2D tileset, int tileSize, int gridCols,
                                 int gridRows, Vector2 originWorldPos,
                                 float parallaxFactor)
    : tileset(tileset),
      tileSize(tileSize),
      cols(gridCols),
      rows(gridRows),
      origin(originWorldPos),
      factor(parallaxFactor) {}

Rectangle BackgroundLayer::TileSrc(int col, int row) const {
    // Reuse your tileset conventions:
    // e.g., row 0 = floating, row 1 = top, row 2 = middle, row 3 = bottom
    // Here we’ll just map col,row directly to the sheet (32x32 tiles assumed)
    return Rectangle{(float)(col * tileSize), (float)(row * tileSize),
                     (float)tileSize, (float)tileSize};
}

void BackgroundLayer::Draw(Vector2 cameraTarget) const {
    // Parallax offset: farther layers move less than the camera
    // factor=1 follows camera; factor=0 is screen-locked vibe (but we’re in
    // world space)
    Vector2 parallaxOffset = {cameraTarget.x * (factor - 1.0f),
                              cameraTarget.y * (factor - 1.0f)};

    // Simple fill: you can pick different tile rows/cols per cell for variety
    for (int gy = 0; gy < rows; ++gy) {
        for (int gx = 0; gx < cols; ++gx) {
            // Example rule: top row uses a “top” tile, below uses “middle” tile
            bool isTopRow = (gy == 0);
            int srcRow = isTopRow ? 1 : 2; // adjust to your tileset rows
            int srcCol = 1; // 0=left,1=mid,2=right—tweak for edges

            Rectangle src = TileSrc(srcCol, srcRow);
            Rectangle dest = {origin.x + gx * tileSize + parallaxOffset.x,
                              origin.y + gy * tileSize + parallaxOffset.y,
                              (float)tileSize, (float)tileSize};

            DrawTexturePro(tileset, src, dest, {0, 0}, 0.0f, WHITE);
        }
    }
}
