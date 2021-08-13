#ifndef VV_WS_SERVER_H
#define VV_WS_SERVER_H

#include <string>
#include <json.hpp>

#include "WebSocketServer.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"

using namespace hv;

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
        void startSync();
    private:
        std::string TAG;
};


#endif // VV_WS_SERVER_H