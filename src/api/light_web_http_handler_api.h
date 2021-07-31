#ifndef LIGHT_WEB_HTTP_HANDLER_API_H
#define LIGHT_WEB_HTTP_HANDLER_API_H

#include <wsjcpp_light_web_server.h>
#include "game_map_objects.h"

class LightWebHttpHandlerApi : public WsjcppLightWebHttpHandlerBase {
    public:
        LightWebHttpHandlerApi(GameMapObjects *pGameMap);
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        GameMapObjects *m_pGameMap;
        std::string m_sPrefixPath;
};

#endif // LIGHT_WEB_HTTP_HANDLER_API_H
