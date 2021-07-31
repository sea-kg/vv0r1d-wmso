
#include "light_web_http_handler_api.h"
#include <wsjcpp_core.h>
#include <json.hpp>

// ---------------------------------------------------------------------
// LightWebHttpHandlerApi

LightWebHttpHandlerApi::LightWebHttpHandlerApi(GameMapObjects *pGameMap) 
: WsjcppLightWebHttpHandlerBase("api") {
    TAG = "LightWebHttpHandlerApi";
    m_sPrefixPath = "/api/";
    m_pGameMap = pGameMap;
}

// ---------------------------------------------------------------------

bool LightWebHttpHandlerApi::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    WsjcppLog::warn(_tag, "canHandle: " + sRequestPath);

    std::string sPrefixPath = sRequestPath.substr(0, m_sPrefixPath.length()); 
    if (sPrefixPath != m_sPrefixPath) {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool LightWebHttpHandlerApi::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());

    if (sRequestPath == "/api/v1/get_map") {
        resp.noCache().notImplemented().ok().sendJson(m_pGameMap->toJson());
        return true;
    }

    WsjcppLog::warn(_tag, sRequestPath);
    resp.noCache().notImplemented().sendEmpty();
    return true;
}


