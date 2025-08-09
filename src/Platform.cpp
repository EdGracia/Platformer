#include "Platform.h"
#include "GameObject.h"
#include "TilesetResolver.h"
#include "raylib.h"

Platform::Platform(float x, float y, float width, float height, Layer lay)
    : GameObject(x, y),
      width(width),
      height(height) {
    position = {x, y};
    tag = Tag::Platform;
    layer = lay;
}

Rectangle Platform::GetBounds() const {
    return Rectangle{position.x, position.y, width, height};
}

bool Platform::IsFloatingPlatform() const { return height <= TILE_SIZE; }
bool Platform::IsSingleTilePlatform() const {
    return (int)width == TILE_SIZE && (int)height == TILE_SIZE;
}

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

void Platform::Update(float deltaTime) {}
void Platform::Draw() const {
    if (!resolver)
        return;
    const Texture2D &tex = resolver->For(layer);
    // Draw a grid of tiles across the platform’s size
    for (int x = 0; x < width; x += TILE_SIZE) {
        for (int y = 0; y < height; y += TILE_SIZE) {
            Rectangle source;
            int tileRow;
            int tileCol;
            if (!IsSingleTilePlatform()) {
                tileRow = GetTileRow(y);
                tileCol = GetTileCol(x);
            } else {
                tileRow = 0;
                tileCol = 3;
            }

            source = {(float)tileCol * TILE_SIZE, (float)tileRow * TILE_SIZE,
                      (float)TILE_SIZE, (float)TILE_SIZE};

            Rectangle dest = {position.x + x, position.y + y, (float)TILE_SIZE,
                              (float)TILE_SIZE};

            DrawTexturePro(tex, source, dest, {0, 0}, 0.0f, WHITE);
            // HITBOX DEBUG:
            // DrawRectangleLinesEx(GetBounds(), 3.0f, Fade(GREEN, 0.5f));
        }
    }
}
