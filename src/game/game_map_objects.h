#ifndef GAME_MAP_OBJECTS_H
#define GAME_MAP_OBJECTS_H

#include <string>
#include <json.hpp>

const int LAYER_VEGETATION = 2;

class GameMapObject {
    public:
        GameMapObject();

    private:
        std::string TAG;
};

class GameMapObjects {
    public:
        GameMapObjects();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        nlohmann::json m_jsonMap;
};

#endif // GAME_MAP_OBJECTS_H