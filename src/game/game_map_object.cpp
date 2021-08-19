#include "game_map_object.h"
#include <fstream>
#include <iostream>

// ---------------------------------------------------------------------
// GameMapObject

GameMapObject::GameMapObject(
    int nId, int nX, int nY, int nWidth, int nHeight,
    int nHealth, int nLayer, bool bCanbeDestroyed,
    const std::string &sImageTexture,
    const std::string &sCurrentState
) {
    TAG = "GameMapObject";
    int m_nId = nId;
    m_nX = nX;
    m_nY = nY;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nHealth = nHealth;
    m_nLayer = nLayer;
    m_bCanbeDestroyed = bCanbeDestroyed;
    m_sImageTexture = sImageTexture;
    m_sCurrentState = sCurrentState;
}

const nlohmann::json &GameMapObject::toJson() {
    m_jsonObject["v"] = 1;
    m_jsonObject["x"] = m_nX;
    m_jsonObject["y"] = m_nY;
    m_jsonObject["w"] = m_nWidth;
    m_jsonObject["h"] = m_nHeight;
    m_jsonObject["t"] = m_sImageTexture;
    m_jsonObject["l"] = m_nLayer;
    return m_jsonObject;
}

bool GameMapObject::containsXY(int x, int y) {
    return
        x >= m_nX && x <= m_nX + m_nWidth
        && y >= m_nY && y <= m_nY + m_nHeight
    ;
}

bool GameMapObject::insideRect(int x0, int y0, int x1, int y1) {
    return
        m_nX >= x0 && m_nX <= x1
        && m_nY >= y0 && m_nY <= y1
    ;
}

int GameMapObject::getX() {
    return m_nX;
}

int GameMapObject::getY() {
    return m_nY;
}

int GameMapObject::getWidth() {
    return m_nWidth;
}

int GameMapObject::getHeight() {
    return m_nHeight;
}