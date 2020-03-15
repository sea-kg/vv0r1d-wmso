#include <string.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <wsjcpp_light_web_http_handler_rewrite_folder.h>

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_NAME);
    std::string appVersion = std::string(WSJCPP_VERSION);
    if (!WSJCppCore::dirExists(".logs")) {
        WSJCppCore::makeDir(".logs");
    }
    WSJCppLog::setPrefixLogFile("gcw");
    WSJCppLog::setLogDirectory(".logs");

    WSJCppLightWebServer httpServer;
    httpServer.setPort(8081);
    httpServer.setMaxWorkers(1);
    httpServer.addHandler((WSJCppLightWebHttpHandlerBase *)new WSJCppLightWebHttpHandlerRewriteFolder("/", "./web"));
    httpServer.startSync(); // this method will be hold current thread, if you with you can call just start/stop command

    return 0;}

