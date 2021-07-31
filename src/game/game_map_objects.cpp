#include "game_map_objects.h"

// ---------------------------------------------------------------------
// GameMapObject

GameMapObject::GameMapObject() {
    TAG = "GameMapObject";
}

// ---------------------------------------------------------------------
// GameMapObjects

GameMapObjects::GameMapObjects() {
    TAG = "GameMapObjects";

    m_jsonMap["width"] = 100;
    m_jsonMap["height"] = 100;

    nlohmann::json jsonMapObject;
    jsonMapObject["x"] = 2;
    jsonMapObject["y"] = 2;
    jsonMapObject["t"] = "objects/tree0-50x100";
    jsonMapObject["l"] = LAYER_VEGETATION;
    jsonMapObject["w"] = 50;
    jsonMapObject["h"] = 50;

    nlohmann::json jsonMapObjects = nlohmann::json::array();
    jsonMapObjects.push_back(jsonMapObject);

    m_jsonMap["objects"] = jsonMapObjects;
}

const nlohmann::json &GameMapObjects::toJson() {
    return m_jsonMap;
}