#include "game_map_generator.h"
#include <fstream>
#include <iostream>

// ---------------------------------------------------------------------
// GameMapGenerator

GameMapGenerator::GameMapGenerator() {
    TAG = "GameMapGenerator";
}


void GameMapGenerator::generate(std::vector<GameMapObject *> &vObjects, int nX, int nY, int nW, int nH) {
    int size = 50;
    for (int x = nX; x <= nX + nW; x += size) {
        for (int y = nY; y <= nY + nH; y += size) {
            vObjects.push_back(new GameMapObject(
                0, x, y, size, size, 1, LAYER_BACKGROUND, true, "background/grace0-50x50", "ok"
            ));
        }
    }

    int nMaxTrees = rand() % 20;
    for (int i = 0; i < nMaxTrees; i++) {
        int x = nX + rand() % nW;
        int y = nY + rand() % nH;
        vObjects.push_back(new GameMapObject(
            0, x, y, 50, 100, 20, LAYER_VEGETATION, true, "objects/tree0-50x100", "ok"
        ));
    }


    // TODO save to database
    // TODO generate rocks and trees
}