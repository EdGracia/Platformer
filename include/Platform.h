#ifndef PLATFORM_H
#define PLATFORM_H

#include "GameObject.h"
#include "raylib.h"

class Platform : public GameObject {
    public:
        Platform(float x, float y, float width, float height,
                 Texture2D tileset);

        void Update(float deltaTime);
        void Draw() const override;
        Rectangle GetBounds() const;

        void UnloadTileset();

        bool IsFloatingPlatform() const;
        bool IsSingleTilePlatform() const;
        int GetTileRow(int y) const;
        int GetTileCol(int x) const;

    private:
        float width, height;
        Texture2D tileset;
        static Texture2D bgTileset;
        static constexpr int TILE_SIZE = 32;
};

#endif
