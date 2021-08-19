#ifndef VV_HTTP_SERVER_H
#define VV_HTTP_SERVER_H

#include <string>
#include <json.hpp>
#include "HttpService.h"
#include "game_map_objects.h"

class VvHttpServer {
    public:
        VvHttpServer(GameMapObjects *pGameMapObjects);
        HttpService *getService();
        int httpApiV1GetPaths(HttpRequest* req, HttpResponse* resp);

    private:
        std::string TAG;
        HttpService *m_httpService;
        GameMapObjects *m_pGameMapObjects;
};

#endif // VV_HTTP_SERVER_H