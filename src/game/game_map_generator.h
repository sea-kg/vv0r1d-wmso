#ifndef GAME_MAP_GENERATOR_H
#define GAME_MAP_GENERATOR_H

#include "game_map_object.h"
#include <string>

class GameMapGenerator {
    public:
        GameMapGenerator();
        void generate(std::vector<GameMapObject *> &vObjects, int nX, int nY, int nW, int nH);

    private:
        std::string TAG;
};

#endif // GAME_MAP_GENERATOR_H