#ifndef GAME_MAP_OBJECTS_H
#define GAME_MAP_OBJECTS_H

#include "game_map_object.h"
#include <string>
#include <json.hpp>

class GameMapObjects {
    public:
        GameMapObjects();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        nlohmann::json m_jsonMap;
        std::vector<GameMapObject *> m_vObjects;

        // TODO
        // m_vObjects - must be renamed to cache
        // redesign some fast search by x,y, block must be like 256x256
};

#endif // GAME_MAP_OBJECTS_H