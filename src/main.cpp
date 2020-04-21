#include <string.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <wsjcpp_light_web_http_handler_rewrite_folder.h>

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_NAME);
    std::string appVersion = std::string(WSJCPP_VERSION);
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("gcw");
    WsjcppLog::setLogDirectory(".logs");

    WsjcppLightWebServer httpServer;
    httpServer.setPort(8081);
    httpServer.setMaxWorkers(1);
    httpServer.addHandler(new WsjcppLightWebHttpHandlerRewriteFolder("/", "./web"));
    httpServer.startSync(); // this method will be hold current thread, if you with you can call just start/stop command

    return 0;}

