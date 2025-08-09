#pragma once
#include "Types.h"
#include "raylib.h"
struct TilesetResolver {
        Texture2D fgTileset{}; // set once in GameScene::OnEnter
        Texture2D bgTileset{};

        const Texture2D &For(Layer layer) const {
            return (layer == Layer::Background) ? bgTileset : fgTileset;
        }
};
