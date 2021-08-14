#include "player_context.h"
#include <fstream>
#include <iostream>
#include <json.hpp>

// ---------------------------------------------------------------------
// PlayerContext

PlayerContext::PlayerContext() {
    m_nPlayerX = 0;
    m_nPlayerY = 0;
}

int PlayerContext::getPlayerX() {
    return m_nPlayerX;
}

int PlayerContext::getPlayerY() {
    return m_nPlayerY;
}

void PlayerContext::setPlayerX(int nX) {
    m_nPlayerX = nX;
}

void PlayerContext::setPlayerY(int nY) {
    m_nPlayerY = nY;
}