#ifndef VV_WS_SERVER_H
#define VV_WS_SERVER_H

#include <string>
#include <json.hpp>

#include "WebSocketServer.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"

class MyContext {
    public:
        MyContext();
        ~MyContext();
        int handleMessage(const std::string& msg);
        hv::TimerID timerID;
};

class VvWsServer {
    public:
        VvWsServer();
        WebSocketService *getService();
    private:
        std::string TAG;
        WebSocketService m_wsService;
};


#endif // VV_WS_SERVER_H