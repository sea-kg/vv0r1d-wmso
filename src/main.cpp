#include <string.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <wsjcpp_light_web_http_handler_rewrite_folder.h>


int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("wsjcpp");
    WsjcppLog::setLogDirectory(".logs");
    // TODO your code here

    WsjcppLightWebServer httpServer;
    httpServer.setPort(1234);
    httpServer.setMaxWorkers(4);
    httpServer.addHandler(new WsjcppLightWebHttpHandlerRewriteFolder("/", "./html"));
    httpServer.startSync(); // this method will be hold current thread, if you with you can call just start/stop command

    return 0;
}

