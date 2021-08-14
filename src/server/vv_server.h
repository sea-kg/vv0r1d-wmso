#ifndef VV_SERVER_H
#define VV_SERVER_H

#include <string>
#include <json.hpp>
#include "game_map_objects.h"

class VvServer {
    public:
        VvServer(GameMapObjects *pGameMapObjects);
        void startSync(int nPort);

    private:
        std::string TAG;
        GameMapObjects *m_pGameMapObjects;
};

#endif // VV_SERVER_H