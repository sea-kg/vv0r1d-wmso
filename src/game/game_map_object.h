#ifndef GAME_MAP_OBJECT_H
#define GAME_MAP_OBJECT_H

#include <string>
#include <json.hpp>

const int LAYER_BACKGROUND = 1;
const int LAYER_ROADS = 2;
const int LAYER_VEGETATION = 3;
const int LAYER_BUILDING = 4;

class GameMapObject {
    public:
        GameMapObject(
            int nId, int nX, int nY, int nWidth, int nHeight,
            int nHealth, int nLayer, bool bCanbeDestroyed,
            const std::string &sImageTexture,
            const std::string &sCurrentState
        );
        const nlohmann::json &toJson();
        bool containsXY(int x, int y);
        bool insideRect(int x0, int y0, int x1, int y1);
        int getX();
        int getY();
        int getWidth();
        int getHeight();

    private:
        std::string TAG;
        std::string m_sImageTexture;
        int m_nId;
        int m_nX;
        int m_nY;
        int m_nWidth;
        int m_nHeight;
        int m_nLayer;
        bool m_bCanbeDestroyed;
        std::string m_sCurrentState;
        int m_nHealth;
        nlohmann::json m_jsonObject;
};

#endif // GAME_MAP_OBJECT_H