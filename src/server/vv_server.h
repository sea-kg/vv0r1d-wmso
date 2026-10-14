#ifndef VV_SERVER_H
#define VV_SERVER_H

#include "game_map_objects.h"
#include <json.hpp>
#include <string>

class VvServer {
public:
  VvServer(GameMapObjects *pGameMapObjects);
  void startSync(int nPort);

private:
  std::string TAG;
  GameMapObjects *m_pGameMapObjects;
};

#endif // VV_SERVER_H