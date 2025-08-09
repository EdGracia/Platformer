#pragma once
#include "GameObject.h"
#include "TilesetResolver.h"
#include "raylib.h"

class Platform : public GameObject {
    public:
        Platform(float x, float y, float width, float height,
                 Layer layer = Layer::Foreground);

        void Update(float deltaTime) override;
        void Draw() const override;
        Rectangle GetBounds() const;

        void SetTilesetResolver(const TilesetResolver *r) { resolver = r; }
        void UnloadTileset();

        bool IsFloatingPlatform() const;
        bool IsSingleTilePlatform() const;
        int GetTileRow(int y) const;
        int GetTileCol(int x) const;

    private:
        float width, height;
        Layer layer;
        const TilesetResolver *resolver = nullptr;
        static Texture2D bgTileset;
        static constexpr int TILE_SIZE = 32;
};
