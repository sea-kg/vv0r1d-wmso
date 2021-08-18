#include "game_map_object.h"
#include <fstream>
#include <iostream>

// ---------------------------------------------------------------------
// GameMapObject

GameMapObject::GameMapObject() {
    TAG = "GameMapObject";
    m_sImageTexture = "objects/road0-50x50";
    int m_nId = 0;
    m_nX = 0;
    m_nY = 0;
    m_nWidth = 50;
    m_nHeight = 50;
    m_nHealth = 100;
    m_nLayer = LAYER_ROADS;
    m_bCanbeDestroyed = false;
    m_sCurrentState = "ok";
}

const nlohmann::json &GameMapObject::toJson() {
    m_jsonObject["x"] = m_nX;
    m_jsonObject["y"] = m_nY;
    m_jsonObject["w"] = m_nWidth;
    m_jsonObject["h"] = m_nHeight;
    m_jsonObject["t"] = m_sImageTexture;
    m_jsonObject["l"] = m_nLayer;
    return m_jsonObject;
}

int GameMapObject::getX() {
    return m_nX;
}

int GameMapObject::getY() {
    return m_nY;
}