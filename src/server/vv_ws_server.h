#ifndef VV_WS_SERVER_H
#define VV_WS_SERVER_H

#include <string>
#include <json.hpp>

#include "player_context.h"
#include "WebSocketServer.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"

class VvWsConnectionContext {
    public:
        VvWsConnectionContext();
        ~VvWsConnectionContext();
        int handleMessage(const std::string& msg);
        hv::TimerID &getTimerId();
        void setTimerId(hv::TimerID &timerID);
        PlayerContext *getPlayerContext();
        void setPlayerContext(PlayerContext *);
    private:
        hv::TimerID m_nTimerID;
        PlayerContext *m_pPlayerContext;
};

class VvWsServer {
    public:
        VvWsServer();
        WebSocketService *getService();
    private:
        void onMessage(const WebSocketChannelPtr& channel, const std::string& msg);

        std::string TAG;
        WebSocketService m_wsService;
        std::map<std::string, PlayerContext*> m_mapSessions;
};


#endif // VV_WS_SERVER_H