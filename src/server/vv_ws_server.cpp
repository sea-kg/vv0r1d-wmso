#include "vv_ws_server.h"
#include <fstream>
#include <iostream>


// ---------------------------------------------------------------------
// MyContext

MyContext::MyContext() {
    timerID = INVALID_TIMER_ID;
}
MyContext::~MyContext() {
}

int MyContext::handleMessage(const std::string& msg) {
    printf("onmessage: %s\n", msg.c_str());
    return msg.size();
}

// ---------------------------------------------------------------------
// VVWsServer

VvWsServer::VvWsServer() {
    TAG = "VvWsServer";

}

void VvWsServer::startSync() {
    int port = 1235;

    HttpService router;
    router.GET("/ping", [](HttpRequest* req, HttpResponse* resp) {
        return resp->String("pong");
    });

    router.GET("/data", [](HttpRequest* req, HttpResponse* resp) {
        static char data[] = "0123456789";
        return resp->Data(data, 10 /*, false */);
    });

    router.GET("/paths", [&router](HttpRequest* req, HttpResponse* resp) {
        return resp->Json(router.Paths());
    });

    router.GET("/get", [](HttpRequest* req, HttpResponse* resp) {
        resp->json["origin"] = req->client_addr.ip;
        resp->json["url"] = req->url;
        resp->json["args"] = req->query_params;
        resp->json["headers"] = req->headers;
        return 200;
    });

    router.POST("/echo", [](HttpRequest* req, HttpResponse* resp) {
        resp->content_type = req->content_type;
        resp->body = req->body;
        return 200;
    });

    WebSocketService ws;
    ws.onopen = [](const WebSocketChannelPtr& channel, const std::string& url) {
        printf("onopen: GET %s\n", url.c_str());
        MyContext* ctx = channel->newContext<MyContext>();
        // send(time) every 1s
        ctx->timerID = setInterval(1000, [channel](TimerID id) {
            char str[DATETIME_FMT_BUFLEN] = {0};
            datetime_t dt = datetime_now();
            datetime_fmt(&dt, str);
            channel->send(str);
        });
    };
    ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg) {
        MyContext* ctx = channel->getContext<MyContext>();
        ctx->handleMessage(msg);
    };
    ws.onclose = [](const WebSocketChannelPtr& channel) {
        printf("onclose\n");
        MyContext* ctx = channel->getContext<MyContext>();
        if (ctx->timerID != INVALID_TIMER_ID) {
            killTimer(ctx->timerID);
        }
        channel->deleteContext<MyContext>();
    };

    websocket_server_t server;
    server.service = &router;
    server.port = port;
    server.ws = &ws;
    websocket_server_run(&server);
}