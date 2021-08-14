#ifndef PLAYER_CONTEXT_H
#define PLAYER_CONTEXT_H

#include <string>

class PlayerContext {
    public:
        PlayerContext();
        int getPlayerX();
        int getPlayerY();
        void setPlayerX(int);
        void setPlayerY(int);

    private:

        std::string m_sToken;
        int m_nPlayerX;
        int m_nPlayerY;
};

#endif // PLAYER_CONTEXT_H