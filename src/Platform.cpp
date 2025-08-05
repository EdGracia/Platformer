#include "Platform.h"
#include "raylib.h"

Texture2D Platform::tileset;

Platform::Platform(float x, float y, float width, float height)
    : position({x, y}),
      width(width),
      height(height) {}

Rectangle Platform::GetBounds() const {
    return Rectangle{position.x, position.y, width, height};
}

void Platform::LoadTileset() {
    tileset = LoadTexture("assets/tileset/0_Template_Tileset.png");
    // tileset = LoadTexture("assets/tileset/1_Industrial_Tileset_1B.png");
    SetTextureFilter(tileset, TEXTURE_FILTER_POINT);
}

void Platform::UnloadTileset() { UnloadTexture(tileset); }

bool Platform::IsFloatingPlatform() const { return height <= TILE_SIZE; }

int Platform::GetTileRow(int y) const {
    if (IsFloatingPlatform())
        return 0;
    if (y == 0)
        return 1;
    if (y + TILE_SIZE >= height)
        return 3;
    return 2;
}

int Platform::GetTileCol(int x) const {
    if (x == 0)
        return 0;
    if (x + TILE_SIZE >= width)
        return 2;
    return 1;
}

void Platform::Draw() const {
    // Draw a grid of tiles across the platform’s size
    for (int x = 0; x < width; x += TILE_SIZE) {
        for (int y = 0; y < height; y += TILE_SIZE) {
            Rectangle source;

            int tileRow = GetTileRow(y);
            int tileCol = GetTileCol(x);

            source = {(float)tileCol * TILE_SIZE, (float)tileRow * TILE_SIZE,
                      (float)TILE_SIZE, (float)TILE_SIZE};

            Rectangle dest = {position.x + x, position.y + y, (float)TILE_SIZE,
                              (float)TILE_SIZE};

            DrawTexturePro(tileset, source, dest, {0, 0}, 0.0f, WHITE);
        }
    }
}
