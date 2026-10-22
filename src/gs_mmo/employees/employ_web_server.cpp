/**********************************************************************************
 * ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * ::                                   oooooo 8                                 ::
 * ::                                   8      8                                 ::
 * ::              .oPYo. .oPYo. .oPYo. 8pPYo. 8  .o  .oPYo.                     ::
 * ::              Yb..   8oooo8 .oooo8     `8 8oP'   8    8                     ::
 * ::                'Yb. 8.     8    8     .P 8 `b.  8    8                     ::
 * ::              `YooP' `Yooo' `YooP8 `YooP' 8  `o. `YooP8                     ::
 * :::::::::::::::::.....::.....::.....::.....:..::...:....8 ::::::::::::::::::::::
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::ooP'.::::::::::::::::::::::
 * :: .oPYo. .oPYo. ooYoYo. .oPYo.       .oPYo. .oPYo. oPYo. o    o .oPYo. oPYo. ::
 * :: 8    8 .oooo8 8' 8  8 8oooo8 ooooo Yb..   8oooo8 8  `' Y.  .P 8oooo8 8  `' ::
 * :: 8    8 8    8 8  8  8 8.             'Yb. 8.     8     `b..d' 8.     8     ::
 * :: `YooP8 `YooP8 8  8  8 `Yooo'       `YooP' `Yooo' 8      `YP'  `Yooo' 8     ::
 * ::::....8 :.....:..:..:..:.....::::::::.....::.....:..::::::...:::.....:..::::::
 * :::::ooP'.:::::::ooYoYo. ooYoYo. .oPYo.                                       ::
 * ::               8' 8  8 8' 8  8 8    8                                       ::
 * ::               8  8  8 8  8  8 8    8                                       ::
 * ::               8  8  8 8  8  8 `YooP'                                       ::
 * :::::::::::::::::..:..:....:..:..:.....:::::::::::::::::::::::::::::::::::::::::
 * ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 * MIT License
 *
 * Copyright (c) 2020-2026 Evgenii Sopov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Original repository: https://github.com/sea5kg/sea5kg-game-server-mmo
 *
 ***********************************************************************************/

#include "gs_mmo/include/gs_mmo_config.h"
#include "gs_mmo/include/gs_mmo_globals.h"
#include "gs_mmo/include/gs_mmo_web_server.h"
#include "objects/gs_mmo_error_info.h"
#include <atomic>
#include <cstring>
#include <fstream>
#include <initializer_list>
#include <optional>
#include <regex>
#include <sea5kg_logger.h>
#include <sstream>
#include <utility>
#include <vector>
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>

// libhv includes
#include "EventLoop.h"       // libhv
#include "HttpService.h"     // libhv
#include "WebSocketServer.h" // libhv
#include "hlog.h"            // libhv
#include "hssl.h"            // libhv
#include "htime.h"           // libhv


struct UserContext {
  int id = 0;
  int x = 0;
  int y = 0;
};

class employ_web_server : public WsjcppEmployBase, public gs_mmo::web_server {
public:
  employ_web_server();
  virtual bool init(const std::string &name, bool bSilent) override;
  virtual bool deinit(const std::string &name, bool bSilent) override;

  // gs_mmo::web_server
  virtual int start() override;

private:
  std::string TAG;

  int response_error(int http_code, HttpRequest *req, HttpResponse *resp, const gs_mmo::error_info &err);
  void log_err(const std::string &message);
  void log_warn(const std::string &message);
  void updateJsonCache();

  void web_socket_onopen(const WebSocketChannelPtr &channel, const HttpRequestPtr &req);
  void web_socket_onmessage(const WebSocketChannelPtr &channel, const std::string &msg);
  void web_socket_onclose(const WebSocketChannelPtr &channel);

  int httpWebFolder(HttpRequest *req, HttpResponse *resp);
  int httpApiV1Game(HttpRequest *req, HttpResponse *resp);
  int httpApiGameCurrentTime(HttpRequest *req, HttpResponse *resp);
  int httpApiV1Teams(HttpRequest *req, HttpResponse *resp);
  int httpApiV1MyIp(HttpRequest *req, HttpResponse *resp);
  int httpApiV1Scoreboard(HttpRequest *req, HttpResponse *resp);
  int httpApiV1GetPaths(HttpRequest *req, HttpResponse *resp);

  bool can_handle_flags(HttpRequest *req, int current_time_in_seconds, gs_mmo::error_info &err);
  bool find_or_detect_team_id(HttpRequest *req, std::string &team_id, gs_mmo::error_info &err);
  int httpApiV1Flag(HttpRequest *req, HttpResponse *resp);

  // int httpApiV1Metrics(HttpRequest* req, HttpResponse* resp);
  int httpLogo(const std::string &request_path, HttpRequest *req, HttpResponse *resp);

  std::shared_ptr<hv::HttpService> m_pHttpService;
  std::shared_ptr<hv::WebSocketService> m_webSocketService;
  std::string m_sApiPathPrefix;

  // std::atomic<bool> m_metrics_enabled;
  // std::atomic<bool> m_auto_detection_team_id_by_subnet_ip;

  // std::string m_sIndexHtml;
  std::string m_sHtmlFolder;
  // std::string m_sCacheResponseGameJson;
  // std::string m_sCacheResponseTeamsJson;
  std::mutex m_mutex_connected_users;
  std::map<const WebSocketChannelPtr *, std::string> m_connected_users;

  // TODO to employ
  unsigned int m_userid_counter;
  std::map<std::string, UserContext> m_user_context;
  gs_mmo::config *m_config;
};

REGISTRY_WSJCPP_EMPLOY(employ_web_server)

employ_web_server::employ_web_server() : WsjcppEmployBase({gs_mmo::web_server::name()}, {gs_mmo::config::name()}) {
  TAG = gs_mmo::web_server::name();
  m_sApiPathPrefix = "/api/v1/";
  m_userid_counter = 1;
  m_config = nullptr;
}

bool employ_web_server::init(const std::string &name, bool silent) {
  sea5kg::log::info(TAG, "init");
  m_config = findWsjcppEmploy<gs_mmo::config>();

  return true;
}

bool employ_web_server::deinit(const std::string &name, bool silent) {
  sea5kg::log::info(TAG, "deinit");
  return true;
}

static std::shared_ptr<sea5kg::logger> g_http_logger = std::shared_ptr<sea5kg::logger>(sea5kg::logger::create());

void EmployWebServer_custom_logger(int level, const char *msg, int len) {
  static const std::string TAG = "http-hv";
  std::string message(msg, len - 1); // remove last '\n' character
  switch (level) {
  case LOG_LEVEL_DEBUG:
    g_http_logger->debug(TAG, message);
    break;
  case LOG_LEVEL_INFO:
    g_http_logger->info(TAG, message);
    break;
  case LOG_LEVEL_WARN:
    sea5kg::log::warning(TAG, message);
    g_http_logger->warning(TAG, message);
    break;
  case LOG_LEVEL_ERROR:
    sea5kg::log::error(TAG, message);
    g_http_logger->error(TAG, message);
    break;
  case LOG_LEVEL_FATAL:
    sea5kg::log::error(TAG, message);
    g_http_logger->critical(TAG, message);
    break;
  default:
    sea5kg::log::error(TAG, "Unknow level: " + message);
    g_http_logger->error(TAG, message);
  }
}

int employ_web_server::start() {

  auto config = findWsjcppEmploy<gs_mmo::config>();
  g_http_logger->set_log_filename_prefix("http_hv_");
  g_http_logger->set_log_dirpath(sea5kg::log::log_dirpath());
  g_http_logger->set_rotation_period_in_seconds(sea5kg::log::rotation_period_in_seconds());
  g_http_logger->set_log_level_file_output(sea5kg::log_level::DEBUG);
  g_http_logger->set_log_level_console_output(sea5kg::log_level::DISABLE);

  m_sHtmlFolder = config->html_folder();
  // updateJsonCache();

  std::string starting_message =
    "Starting scoreboard on http://localhost:" + std::to_string(m_config->web_port()) + "/";
  g_http_logger->success(TAG, starting_message);
  sea5kg::log::success(TAG, starting_message);

  {
    logger_t *pLogger = hv_default_logger();
    logger_set_handler(pLogger, EmployWebServer_custom_logger);
    logger_set_format(pLogger, "%s"); // removing time and log level

    // Test the log
    hlogi("This is an info message.");
  }

  m_pHttpService = std::make_shared<hv::HttpService>();

  // static files
  m_pHttpService->document_root = "./html";
  m_pHttpService->GET(
    "*", std::bind(&employ_web_server::httpWebFolder, this, std::placeholders::_1, std::placeholders::_2)
  );

  // hv::HttpServer server(m_pHttpService.get());
  // server.setPort(m_config->web_port());
  // server.setThreadNum(4);
  // server.run();

  m_webSocketService = std::make_shared<hv::WebSocketService>();

  m_webSocketService->onopen =
    std::bind(&employ_web_server::web_socket_onopen, this, std::placeholders::_1, std::placeholders::_2);
  m_webSocketService->onmessage =
    std::bind(&employ_web_server::web_socket_onmessage, this, std::placeholders::_1, std::placeholders::_2);
    m_webSocketService->onclose =
      std::bind(&employ_web_server::web_socket_onclose, this, std::placeholders::_1);

  websocket_server_t server;
  server.service = m_pHttpService.get();
  server.port = m_config->web_port();
  server.worker_threads = 4; // TODO move to config
  server.ws = m_webSocketService.get();
  websocket_server_run(&server);
  return 0;
}

int employ_web_server::response_error(
  int http_code, HttpRequest *req, HttpResponse *resp, const gs_mmo::error_info &err
) {
  std::string error_message =
    " Error(" + std::to_string(err.error_code()) + "): " + err.error_msg() + " (" + req->client_addr.ip + ")";
  log_err(error_message);
  resp->String(error_message);
  return http_code;
}

void employ_web_server::log_err(const std::string &message) {
  sea5kg::log::error(TAG, message);
  g_http_logger->error(TAG, message);
}

void employ_web_server::log_warn(const std::string &message) {
  sea5kg::log::warning(TAG, message);
  g_http_logger->warning(TAG, message);
}

void employ_web_server::updateJsonCache() {
  // nlohmann::json jsonGame;
  // nlohmann::json jsonTeams;

  // jsonGame[gs_mmo::json_fields::CTF01D_VERSION] = m_config->ctf01d_version();
  // jsonGame["game_id"] = m_config->game_id();
  // jsonGame["game_name"] = m_config->game_name();
  // jsonGame["game_start"] = WsjcppCore::formatTimeUTC(m_config->game_start_utc_in_seconds()) + " (UTC)";
  // jsonGame["game_end"] = WsjcppCore::formatTimeUTC(m_config->game_end_utc_in_seconds()) + " (UTC)";
  // jsonGame["game_has_coffee_break"] = m_config->game_has_coffee_break();
  // jsonGame["game_coffee_break_start"] = WsjcppCore::formatTimeUTC(m_config->game_coffee_break_start_utc_in_seconds())
  // + " (UTC)"; jsonGame["game_coffee_break_end"] =
  // WsjcppCore::formatTimeUTC(m_config->game_coffee_break_end_utc_in_seconds()) + " (UTC)"; jsonGame["teams"] =
  // nlohmann::json::array(); jsonGame["services"] = nlohmann::json::array();

  // for (unsigned int i = 0; i < m_config->services().size(); i++) {
  //   gs_mmo::service_config service_config = m_config->services()[i];
  //   if (service_config.is_enabled()) {
  //     nlohmann::json service_info;
  //     service_info["id"] = service_config.id();
  //     service_info["name"] = service_config.name();
  //     service_info["round_time_in_sec"] = service_config.round_in_seconds();
  //     jsonGame["services"].push_back(service_info);
  //   }
  // }

  // for (unsigned int i = 0; i < m_config->teams().size(); i++) {
  //     gs_mmo::team_config teamConf = m_config->teams()[i];
  //     nlohmann::json teamInfo = teamConf.to_json();
  //     jsonGame["teams"].push_back(teamInfo);
  //     jsonTeams["teams"].push_back(teamInfo);
  // }

  // m_sCacheResponseGameJson = jsonGame.dump();
  // m_sCacheResponseTeamsJson = jsonTeams.dump();
}

void employ_web_server::web_socket_onopen(const WebSocketChannelPtr &channel, const HttpRequestPtr &req) {
  // channel->c
  std::lock_guard<std::mutex> lock(m_mutex_connected_users);
  m_connected_users.insert(std::pair<const WebSocketChannelPtr *, std::string>(&channel, ""));
  std::cout << "ws. onopen = " << std::endl;
}

void employ_web_server::web_socket_onmessage(const WebSocketChannelPtr &channel, const std::string &msg) {
  const WebSocketChannelPtr *ch_ptr = &channel;
  std::cout << "m_connected_users.count(ch_ptr) = " << m_connected_users.count(ch_ptr) << std::endl;
  // VvWsConnectionContext *ctx = channel->getContext<VvWsConnectionContext>();
  std::cout << "ws. msg = " << msg << std::endl;
  // https://www.jsonrpc.org/specification
  nlohmann::json j = nlohmann::json::parse(msg);

  std::string tkn;
  {
    std::lock_guard<std::mutex> lock(m_mutex_connected_users);
    auto it = m_connected_users.find(ch_ptr);
    if (it != m_connected_users.end())
      tkn = it->second;
  }

  UserContext user_ctx;
  if (tkn != "") {
    auto it = m_user_context.find(tkn);
    if (it != m_user_context.end())
      user_ctx = it->second;
  }

  std::string sMethod = j["method"];
  if (sMethod == "hello") {
  // if (sMethod == "get_player_position") {
  //   nlohmann::json jsonResp;
  //   jsonResp["id"] = j["id"];
  //   jsonResp["method"] = sMethod;

  //   if (ctx->getPlayerContext()) {
  //     nlohmann::json jsonResult;
  //     jsonResult["x"] = ctx->getPlayerContext()->getPlayerX();
  //     jsonResult["y"] = ctx->getPlayerContext()->getPlayerY();
  //     jsonResp["result"] = jsonResult;
  //   } else {
  //     nlohmann::json jsonError;
  //     jsonError["code"] = 550;
  //     jsonError["message"] = "Not found error context";
  //     jsonResp["error"] = jsonError;
  //   }
  //   channel->send(jsonResp.dump());
  // } else if (j["method"] == "player_move_to") {
  //   ctx->handleMessage(msg);
  } else if (sMethod == "change_player_position") {
    // std::string tkn = m_connected_users.find(ch_ptr)->second;
    // if (user_ctx.id != )
    auto it = m_user_context.find(tkn);
    if (it != m_user_context.end()) {
      it->second.x = j["params"]["x"];
      it->second.y = j["params"]["y"];
      // TODO send event player changed coordinates
    } else {
      std::cout << "Not found player by token " << tkn << std::endl;
    }
  } else if (sMethod == "set_random_session_token") {
    std::string new_tkn = j["params"]["token"];
    {
      std::lock_guard<std::mutex> lock(m_mutex_connected_users);
      auto it = m_connected_users.find(ch_ptr);
      if (it != m_connected_users.end())
        it->second = new_tkn;
    }

    {
      auto it = m_user_context.find(new_tkn);
      if (it != m_user_context.end())
        user_ctx = it->second;
      else
      {
        user_ctx.id = m_userid_counter++;
        m_user_context[new_tkn] = user_ctx;
      }
    }
    nlohmann::json jsonResp;
    jsonResp["id"] = j["id"];

    nlohmann::json jsonResult;
    jsonResult["id"] = user_ctx.id;
    jsonResult["x"] = user_ctx.x;
    jsonResult["y"] = user_ctx.y;
    jsonResp["result"] = jsonResult;
    channel->send(jsonResp.dump());
  // } else if (sMethod == "get_map") {
  //   nlohmann::json jsonResp;
  //   jsonResp["id"] = j["id"];
  //   jsonResp["method"] = sMethod;
  //   if (ctx->getPlayerContext()) {
  //     nlohmann::json jsonResult;
  //     int nX = ctx->getPlayerContext()->getPlayerX();
  //     int nY = ctx->getPlayerContext()->getPlayerY();
  //     jsonResp["result"] = m_pGameMapObjects->toJson(nX - 1000, nY - 1000, 2000, 2000);
  //   } else {
  //     nlohmann::json jsonError;
  //     jsonError["code"] = 550;
  //     jsonError["message"] = "Not found error context";
  //     jsonResp["error"] = jsonError;
  //   }
  //   channel->send(jsonResp.dump());
  } else {
    std::cout << "Unknown " << sMethod << std::endl;
  }
}

void employ_web_server::web_socket_onclose(const WebSocketChannelPtr &channel) {
  std::cout << "ws. onclose = " << std::endl;
  const WebSocketChannelPtr *ch_ptr = &channel;
  std::lock_guard<std::mutex> lock(m_mutex_connected_users);
  auto iter = m_connected_users.find(ch_ptr);
  if (iter != m_connected_users.end())
    m_connected_users.erase(iter);
}

int employ_web_server::httpWebFolder(HttpRequest *req, HttpResponse *resp) {
  std::string sOriginalRequestPath = req->path;
  std::string request_path;

  // remove get params from path
  std::size_t nFoundGetParams = sOriginalRequestPath.rfind("?");
  if (nFoundGetParams != std::string::npos) {
    request_path = sOriginalRequestPath.substr(0, nFoundGetParams);
  } else {
    request_path = sOriginalRequestPath;
  }
  request_path = wsjcpp::normalize_filepath(request_path);

  // // hlogi("request_path = " + request_path);
  // if (request_path == "/flag") { // Public endpoint. Allowed without authorization.
  //   return this->httpApiV1Flag(req, resp);
  // }

  // if (request_path.rfind(m_logo_prefix, 0) == 0) {
  //   return httpLogo(request_path, req, resp);
  // }

  // if (request_path.rfind(m_sApiPathPrefix, 0) == 0) {
  //   if (request_path == "/api/v1/game") { // Public endpoint. Allowed without authorization.
  //     return this->httpApiV1Game(req, resp);
  //   } else if (request_path == "/api/v1/game/current-time") { // Public endpoint. Allowed without authorization.
  //     return this->httpApiGameCurrentTime(req, resp);
  //   } else if (request_path == "/api/v1/scoreboard") { // Public endpoint. Allowed without authorization.
  //     return this->httpApiV1Scoreboard(req, resp);
  //   } else if (request_path == "/api/v1/my-ip") { // it's ok. Because network game is public space. This endpoint
  //   need for automatic configuration network.
  //     return this->httpApiV1MyIp(req, resp);
  //   } else if (request_path == "/api/v1/teams") { // Public endpoint. Allowed without authorization.
  //     return this->httpApiV1Teams(req, resp);
  //   } else if (request_path == "/api/v1/metrics" && m_metrics_enabled.load()) { // Config-gated: disabled by default
  //   + IP allowlist (see httpApiV1Metrics).
  //     return this->httpApiV1Metrics(req, resp);
  //   }
  //   return 404;
  // }

  std::string filepath = wsjcpp::normalize_filepath(m_sHtmlFolder + "/" + request_path);
  if (wsjcpp::dir_exists(filepath)) {
    std::string indexHtml = filepath + "/index.html";
    if (wsjcpp::file_exists(indexHtml)) {
      return resp->File(indexHtml.c_str());
    }
    return 404;
  }

  if (wsjcpp::file_exists(filepath)) {
    return resp->File(filepath.c_str());
  }

  std::string sResPath = wsjcpp::normalize_filepath("./data/html/" + request_path);
  if (WsjcppResourcesManager::has(sResPath)) {
    WsjcppResourceFile *pFile = WsjcppResourcesManager::get(sResPath);
    resp->Data(
      (void *)pFile->getBuffer(),
      pFile->getBufferSize(),
      true // nocopy
    );
    resp->SetContentTypeByFilename(sResPath.c_str());
    return 200;
  }
  return 404; // Not found
}

int employ_web_server::httpApiV1Game(HttpRequest *req, HttpResponse *resp) {
  // // std::cout << m_sCacheResponseGameJson << std::endl;
  // resp->Data(
  //   (void *)(m_sCacheResponseGameJson.c_str()),
  //   m_sCacheResponseGameJson.length(),
  //   true // nocopy
  // );
  // resp->SetContentTypeByFilename("game.json");
  return 200;
}

int employ_web_server::httpApiGameCurrentTime(HttpRequest *req, HttpResponse *resp) {
  // // TODO maybe need optimization keep json response for every second.
  // auto now = std::chrono::system_clock::now().time_since_epoch();
  // int nCurrentTimeSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();
  // nlohmann::json current_time;
  // current_time["current-time"] = nCurrentTimeSec - m_config->game_start_utc_in_seconds();
  // std::string json_str = current_time.dump();
  // resp->Data(
  //     (void *)(json_str.c_str()),
  //     json_str.length(),
  //     false // force copy
  // );
  // resp->SetContentTypeByFilename("current-time.json");
  return 200;
}

int employ_web_server::httpApiV1Teams(HttpRequest *req, HttpResponse *resp) {
  // resp->Data(
  //   (void *)(m_sCacheResponseTeamsJson.c_str()),
  //   m_sCacheResponseTeamsJson.length(),
  //   true // nocopy
  // );
  // resp->SetContentTypeByFilename("teams.json");
  return 200;
}

int employ_web_server::httpApiV1MyIp(HttpRequest *req, HttpResponse *resp) {
  resp->json["my-ip"] = req->client_addr.ip;
  return 200;
}

int employ_web_server::httpApiV1Scoreboard(HttpRequest *req, HttpResponse *resp) {
  // auto teamLogos = findWsjcppEmploy<gs_mmo::images>();
  // teamLogos->update_last_change_time();

  // nlohmann::json jsonScoreboard = m_scoreboard->to_json();
  // teamLogos->update_scoreboard_json(jsonScoreboard);
  // std::string sScoreboardJson = jsonScoreboard.dump();
  // resp->Data(
  //     (void *)(sScoreboardJson.c_str()),
  //     sScoreboardJson.length(),
  //     false // nocopy - force copy
  // );
  // resp->SetContentTypeByFilename("scoreboard.json");
  return 200;
}

int employ_web_server::httpApiV1GetPaths(HttpRequest *req, HttpResponse *resp) {
  // TODO
  return resp->Json(m_pHttpService->Paths());
}

bool employ_web_server::can_handle_flags(HttpRequest *req, int current_time_in_seconds, gs_mmo::error_info &err) {
  // if (current_time_in_seconds < m_config->game_start_utc_in_seconds()) {
  //   err = gs_mmo::error_info(2001, "Game not started yet");
  //   return false;
  // }

  // if (m_config->game_has_coffee_break()
  //   && current_time_in_seconds > m_config->game_coffee_break_start_utc_in_seconds()
  //   && current_time_in_seconds < m_config->game_coffee_break_end_utc_in_seconds()
  // ) {
  //   err = gs_mmo::error_info(2002, "Game on coffee break now");
  //   return false;
  // }

  // if (current_time_in_seconds > m_config->game_end_utc_in_seconds()) {
  //   err = gs_mmo::error_info(2002, "Game already ended");
  //   return false;
  // }
  return true;
}

bool employ_web_server::find_or_detect_team_id(HttpRequest *req, std::string &team_id, gs_mmo::error_info &err) {
  // team_id = "";
  // std::string request_ip = req->client_addr.ip;
  // if (!m_auto_detection_team_id_by_subnet_ip.load()) {
  //   team_id = req->GetParam("team_id");
  //   team_id = WsjcppCore::trim(team_id);
  //   team_id = WsjcppCore::toLower(team_id);
  //   if (team_id == "") {
  //     err = gs_mmo::error_info(1001, "Not found get-parameter 'team_id' or parameter is empty");
  //     return false;
  //   }
  // } else {
  //   team_id = m_config->find_team_id_by_subnet(request_ip);
  //   if (team_id == "") {
  //     err = gs_mmo::error_info(1002, "Could not detect 'team_id' by subnet");
  //     return false;
  //   }
  // }
  // if (team_id == "") {
  //   err = gs_mmo::error_info(1003, "Not found or not detect 'team_id'");
  //   return false;
  // }

  // if (!m_config->contains_team_id(team_id)) {
  //   err = gs_mmo::error_info(1004, "Team not found");
  //   return false;
  // }

  return true;
}

int employ_web_server::httpApiV1Flag(HttpRequest *req, HttpResponse *resp) {
  // auto now = std::chrono::system_clock::now().time_since_epoch();
  // int current_time_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(now).count();
  // long current_time_in_milliseconds = long(current_time_in_seconds)*1000;
  // std::string request_ip = req->client_addr.ip;
  // std::string sRequestIP_MsgSuffix = " (" + request_ip + ")";
  // gs_mmo::error_info err;

  // if (!can_handle_flags(req, current_time_in_seconds, err)) {
  //   return response_error(400, req, resp, err);
  // }

  // std::string team_id = "";
  // if (!find_or_detect_team_id(req, team_id, err)) {
  //   return response_error(400, req, resp, err);
  // }

  // std::string flag_value = req->GetParam("flag");
  // flag_value = WsjcppCore::trim(flag_value);
  // flag_value = WsjcppCore::toLower(flag_value);

  // if (flag_value == "") {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-11): Not found get-parameter 'flag' or parameter is empty";
  //   log_err(sErrorMsg + sRequestIP_MsgSuffix);
  //   resp->String(sErrorMsg);
  //   return 400;
  // }

  // // TODO test speed of regexp and if will be simple compare every symbol.
  // const static std::regex
  // reFlagFormat("c01d[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}[0-9]{8,8}"); if
  // (!std::regex_match(flag_value, reFlagFormat)) {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-140): flag has wrong format";
  //   log_err(sErrorMsg + sRequestIP_MsgSuffix);
  //   resp->String(sErrorMsg);
  //   return 400;
  // }

  // m_scoreboard->insert_flag_attempt(team_id, flag_value, request_ip);

  // gs_mmo::flag flag;
  // if (!findWsjcppEmploy<gs_mmo::alive_flags>()->find_alive_flag(flag_value, flag)) {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-150): flag is too old or flag never existed or flag already stole.";
  //   g_http_logger->info(TAG, sErrorMsg + ". Received flag {" + flag_value + "} from {" + team_id + "}" +
  //   sRequestIP_MsgSuffix); resp->String(sErrorMsg); return 403;
  // }

  // if (flag.time_end_in_milliseconds() < current_time_in_milliseconds) {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-151): flag is too old";
  //   log_err(sErrorMsg + ". Received flag {" + flag_value + "} from {" + team_id + "}" + sRequestIP_MsgSuffix);
  //   resp->String(sErrorMsg);
  //   return 403;
  // }

  // // if (flag.teamStole() == team_id) {
  // //   response.forbidden().sendText("Error(-160): flag already stole by your team");
  // //   log_err("Error(-160): Received flag {" + flag_value + "} from {" + team_id + "} (flag already stole by your
  // team)");
  // //   return true;
  // // }

  // if (flag.team_id() == team_id) {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-180): this is your flag";
  //   log_err(sErrorMsg + ". Received flag {" + flag_value + "} from {" + team_id + "}" + sRequestIP_MsgSuffix);
  //   resp->String(sErrorMsg);
  //   return 403;
  // }

  // std::string sServiceStatus = m_scoreboard->service_status(team_id, flag.service_id());

  // // std::cout << "sServiceStatus: " << sServiceStatus << "\n";

  // if (sServiceStatus != gs_mmo::service_status_cell::SERVICE_UP) {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-190): Your same service is dead. Try later.";
  //   log_err(sErrorMsg + ". Received flag {" + flag_value + "} from {" + team_id + "}" + sRequestIP_MsgSuffix);
  //   resp->String(sErrorMsg);
  //   return 403;
  // }

  // // TODO light update scoreboard
  // // incrementAttackScore performs the dedup check under its own mutex,
  // // so check-then-insert is atomic against concurrent submissions.
  // std::optional<int> oPoints = m_scoreboard->increment_attack_score(flag, team_id);
  // if (!oPoints.has_value()) {
  //   // TODO server statistics
  //   static const std::string sErrorMsg = "Error(-170): flag already stolen by your team";
  //   log_err(sErrorMsg + ". Received flag {" + flag_value + "} from {" + team_id + "}" + sRequestIP_MsgSuffix);
  //   resp->String(sErrorMsg);
  //   return 403;
  // }
  // std::string sPoints = std::to_string(oPoints.value());

  // std::string sResponse = "Accepted: Received flag {" + flag_value + "} from {" + team_id + "} (Accepted + " +
  // sPoints + ")";
  // // really need send to current ???
  // g_http_logger->success(TAG, sResponse + sRequestIP_MsgSuffix);
  // sea5kg::log::success(TAG, sResponse + sRequestIP_MsgSuffix);
  // resp->Data(
  //     (void *)(sResponse.c_str()),
  //     sResponse.size(),
  //     false // copy buffer
  // );
  // resp->content_type = TEXT_PLAIN;
  return 200;
}

int employ_web_server::httpLogo(const std::string &request_path, HttpRequest *req, HttpResponse *resp) {
  // TODO refactoring it

  // std::string id = request_path.substr(m_logo_prefix_length, request_path.length() - m_logo_prefix_length);
  // auto images = findWsjcppEmploy<gs_mmo::images>();
  // std::shared_ptr<gs_mmo::image> img = images->find_image(id);

  // if (!img) {
  //   return 404;
  // }
  // resp->Data(
  //   img->pBuffer,
  //   img->nBufferSize,
  //   true // nocopy
  // );
  // resp->SetContentTypeByFilename(img->filename().c_str());
  return 200;
}
