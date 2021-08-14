#include "vv_ws_server.h"
#include <fstream>
#include <iostream>

using namespace hv;

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

    // m_wsService = new WebSocketService();
    m_wsService.onopen = [](const WebSocketChannelPtr& channel, const std::string& url) {
        printf("onopen: GET %s\n", url.c_str());
        MyContext* ctx = channel->newContext<MyContext>();
        // send(time) every 1s
        ctx->timerID = hv::setInterval(1000, [channel](hv::TimerID id) {
            char str[DATETIME_FMT_BUFLEN] = {0};
            datetime_t dt = datetime_now();
            datetime_fmt(&dt, str);
            channel->send(str);
        });
    };
    m_wsService.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg) {
        MyContext* ctx = channel->getContext<MyContext>();
        ctx->handleMessage(msg);
    };
    m_wsService.onclose = [](const WebSocketChannelPtr& channel) {
        printf("onclose\n");
        MyContext* ctx = channel->getContext<MyContext>();
        if (ctx->timerID != INVALID_TIMER_ID) {
            killTimer(ctx->timerID);
        }
        channel->deleteContext<MyContext>();
    };

}

WebSocketService *VvWsServer::getService() {
    return &m_wsService;
}