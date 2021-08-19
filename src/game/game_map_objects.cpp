#include "game_map_objects.h"
#include <fstream>
#include <iostream>

// ---------------------------------------------------------------------
// GameMapCache
GameMapCache::GameMapCache() {
    m_nCellCacheSize = 256;
}

int GameMapCache::getCellCacheSize() {
    return m_nCellCacheSize;
}

bool GameMapCache::hasObjects(int cx, int cy) {
    if (m_mapCache.count(cx) == 0) {
        m_mapCache[cx] = std::map<int, std::vector<GameMapObject *>>();
    }
    if (m_mapCache[cx].count(cy) == 0) {
        m_mapCache[cx][cy] = std::vector<GameMapObject *>();
    }
    return m_mapCache[cx][cy].size() > 0;
}

void GameMapCache::addObjects(int cx, int cy, const std::vector<GameMapObject *> &vObjects) {
    if (m_mapCache.count(cx) == 0) {
        m_mapCache[cx] = std::map<int, std::vector<GameMapObject *>>();
    }
    if (m_mapCache[cx].count(cy) == 0) {
        m_mapCache[cx][cy] = std::vector<GameMapObject *>();
    }
    for (int i = 0; i < vObjects.size(); i++) {
        m_mapCache[cx][cy].push_back(vObjects[i]);
    }
}

const std::vector<GameMapObject *> &GameMapCache::getObjects(int cx, int cy) {
    if (m_mapCache.count(cx) == 0) {
        m_mapCache[cx] = std::map<int, std::vector<GameMapObject *>>();
    }
    if (m_mapCache[cx].count(cy) == 0) {
        m_mapCache[cx][cy] = std::vector<GameMapObject *>();
    }
    return m_mapCache[cx][cy];
}

// ---------------------------------------------------------------------
// GameMapObjects

GameMapObjects::GameMapObjects(DatabaseMap *pDatabaseMap) {
    TAG = "GameMapObjects";
    m_pDatabaseMap = pDatabaseMap;
    m_pMapCache = new GameMapCache();
    m_pGameMapGenerator = new GameMapGenerator();

    // int nX = -400;
    // int nY = -400;
    // std::vector<GameMapObject *> vObjects = pDatabaseMap->getMapObjectsRect(nX, nY, m_pMapCache->getCellCacheSize(), m_pMapCache->getCellCacheSize());
    // if (vObjects.size() < 26) {
    //     // TODO generate map
    //     m_pGameMapGenerator->generate(vObjects, nX, nY, m_pMapCache->getCellCacheSize(), m_pMapCache->getCellCacheSize());
    // }
    // for (int i = 0; i < vObjects.size(); i++) {
    //     m_vObjects.push_back(vObjects[i]);
    // }

    // std::ifstream ifs("data/game-map.json");
    // nlohmann::json jsonGameMap = nlohmann::json::parse(ifs);
    // nlohmann::json jsonMapObjects = nlohmann::json::array();

    // m_jsonMap["width"] = jsonGameMap["width"];
    // m_jsonMap["height"] = jsonGameMap["height"];

    /*nlohmann::json jsonLayerBackground = jsonGameMap["render"]["layer-background"];
    for (auto it : jsonLayerBackground) {
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
    }*/
    

    // m_jsonMap["objects"] = jsonMapObjects;
}

nlohmann::json GameMapObjects::toJson(int nX, int nY, int nW, int nH) {
    nlohmann::json jsonRet;
    nlohmann::json jsonMapObjects = nlohmann::json::array();

    // alignment by segments
    int nStep = m_pMapCache->getCellCacheSize();
    int nCx0 = alignmentByCell(nX, -1) / nStep;
    int nCx1 = alignmentByCell(nX + nW, 1) / nStep;
    int nCy0 = alignmentByCell(nY, -1) / nStep;
    int nCy1 = alignmentByCell(nY + nH, 1) / nStep;

    for (int cx = nCx0; cx <= nCx1; cx++) {
        for (int cy = nCy0; cy <= nCy1; cy++) {
            // caching
            if (!m_pMapCache->hasObjects(cx, cy)) {
                std::vector<GameMapObject *> vObjects = m_pDatabaseMap->getMapObjectsRect(cx, cy, m_pMapCache->getCellCacheSize(), m_pMapCache->getCellCacheSize());
                if (vObjects.size() < 26) {
                    m_pGameMapGenerator->generate(vObjects, cx * nStep, cy * nStep, m_pMapCache->getCellCacheSize(), m_pMapCache->getCellCacheSize());
                }
                m_pMapCache->addObjects(cx, cy, vObjects);
            }
            const std::vector<GameMapObject *> &vObjects = m_pMapCache->getObjects(cx,cy);
            for (int i = 0; i < vObjects.size(); i++) {
                jsonMapObjects.push_back(vObjects[i]->toJson());
            }
        }
    }
    jsonRet["objects"] = jsonMapObjects;
    return jsonRet;
}

int GameMapObjects::alignmentByCell(int n, int d) {
    return n - n % m_pMapCache->getCellCacheSize() + d * m_pMapCache->getCellCacheSize();
}