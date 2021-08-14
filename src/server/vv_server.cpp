
#include "vv_server.h"
#include "vv_ws_server.h"
#include "vv_http_server.h"

#include "WebSocketServer.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"

using namespace hv;


VvServer::VvServer(GameMapObjects *pGameMapObjects) {
    TAG = "VvServer";
    m_pGameMapObjects = pGameMapObjects;
}

void VvServer::startSync(int nPort) {
    VvWsServer wsServer;
    WebSocketService *pWs = wsServer.getService();
    VvHttpServer httpServer(m_pGameMapObjects);
    HttpService *pRouter = httpServer.getService();

    websocket_server_t server;
    server.service = pRouter;
    server.port = nPort;
    server.ws = pWs;
    websocket_server_run(&server);
}