#include "game_map_objects.h"
#include <fstream>
#include <iostream>

// ---------------------------------------------------------------------
// GameMapObjects

GameMapObjects::GameMapObjects() {
    TAG = "GameMapObjects";

    // TODO load from sqlite
    m_vObjects.push_back(new GameMapObject());

    std::ifstream ifs("data/game-map.json");
    nlohmann::json jsonGameMap = nlohmann::json::parse(ifs);
    nlohmann::json jsonMapObjects = nlohmann::json::array();

    m_jsonMap["width"] = jsonGameMap["width"];
    m_jsonMap["height"] = jsonGameMap["height"];

    nlohmann::json jsonLayerBackground = jsonGameMap["render"]["layer-background"];
    for (auto it : jsonLayerBackground){
        nlohmann::json jsonMapObject;
        jsonMapObject["x"] = it["x"];
        jsonMapObject["y"] = it["y"];
        jsonMapObject["w"] = it["width"];
        jsonMapObject["h"] = it["height"];
        jsonMapObject["t"] = it["texture"];
        jsonMapObject["l"] = LAYER_BACKGROUND;
        jsonMapObjects.push_back(jsonMapObject);
        // std::cout << "value: " << it << '\n';
    }

    nlohmann::json jsonLayerRoads = jsonGameMap["render"]["layer-roads"];
    for (auto it : jsonLayerRoads){
        nlohmann::json jsonMapObject;
        jsonMapObject["x"] = it["x"];
        jsonMapObject["y"] = it["y"];
        jsonMapObject["w"] = it["width"];
        jsonMapObject["h"] = it["height"];
        jsonMapObject["t"] = it["texture"];
        jsonMapObject["l"] = LAYER_ROADS;
        jsonMapObjects.push_back(jsonMapObject);
    }

    nlohmann::json jsonLayerVegetation = jsonGameMap["render"]["layer-vegetation"];
    for (auto it : jsonLayerVegetation){
        nlohmann::json jsonMapObject;
        jsonMapObject["x"] = it["x"];
        jsonMapObject["y"] = it["y"];
        jsonMapObject["w"] = it["width"];
        jsonMapObject["h"] = it["height"];
        jsonMapObject["t"] = it["texture"];
        jsonMapObject["l"] = LAYER_VEGETATION;
        jsonMapObjects.push_back(jsonMapObject);
    }

    nlohmann::json jsonLayerBuildings = jsonGameMap["render"]["layer-buildings"];
    for (auto it : jsonLayerBuildings){
        nlohmann::json jsonMapObject;
        jsonMapObject["x"] = it["x"];
        jsonMapObject["y"] = it["y"];
        jsonMapObject["w"] = it["width"];
        jsonMapObject["h"] = it["height"];
        jsonMapObject["t"] = it["texture"];
        jsonMapObject["l"] = LAYER_BUILDING;
        jsonMapObjects.push_back(jsonMapObject);
    }

    for (int i = 0; i < m_vObjects.size(); i++) {
        jsonMapObjects.push_back(m_vObjects[i]->toJson());
    }
    

    m_jsonMap["objects"] = jsonMapObjects;
}

const nlohmann::json &GameMapObjects::toJson() {
    return m_jsonMap;
}