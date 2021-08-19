
#include "vv_http_server.h"

#include "WebSocketServer.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"

using namespace hv;


VvHttpServer::VvHttpServer(GameMapObjects *pGameMapObjects) {
    TAG = "VvHttpServer";
    m_pGameMapObjects = pGameMapObjects;
    m_httpService = new HttpService();

    // static files
    m_httpService->document_root = "./html";

    m_httpService->GET("/api/", std::bind(&VvHttpServer::httpApiV1GetPaths, this, std::placeholders::_1, std::placeholders::_2));
    m_httpService->GET("/api/v1/", std::bind(&VvHttpServer::httpApiV1GetPaths, this, std::placeholders::_1, std::placeholders::_2));

    m_httpService->GET("/get", [](HttpRequest* req, HttpResponse* resp) {
        resp->json["origin"] = req->client_addr.ip;
        resp->json["url"] = req->url;
        resp->json["args"] = req->query_params;
        resp->json["headers"] = req->headers;
        return 200;
    });
}

HttpService *VvHttpServer::getService() {
    return m_httpService;
}

int VvHttpServer::httpApiV1GetPaths(HttpRequest* req, HttpResponse* resp) {
    return resp->Json(m_httpService->Paths());
}
