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

#include "gs_mmo_game_net_events.h"

namespace gs_mmo {

game_net_events::game_net_events() : m_collect_events(true) {
  m_resp["jsonrpc"] = "2.0";
  m_resp["method"] = "events";
  m_resp["params"] = nlohmann::json::object();
  m_resp["params"]["p"] = nlohmann::json::object();
  m_resp["params"]["e"] = nlohmann::json::array();
  m_last_events_ms = 0;
}

void game_net_events::set_method_name(const std::string &method) {
  m_resp["method"] = method;
}

void game_net_events::set_collect_events(bool v) {
  m_collect_events = v;
}

void game_net_events::add_player(const gs_mmo::player_context &ctx) {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_resp["params"]["p"][ctx.id()] = nlohmann::json::object();
  ctx.update_position_in_json(m_resp["params"]["p"][ctx.id()]);
  if (m_collect_events) {
    nlohmann::json e = nlohmann::json::object();
    e["t"] = "add";
    e["id"] = ctx.id();
    add_event_no_mutex(e);
  }
}

void game_net_events::remove_player(const std::string &userid) {
  std::lock_guard<std::mutex> lock(m_mutex);
  if (m_resp["params"]["p"].contains(userid)) {
    m_resp["params"]["p"].erase(userid);
  }
  if (m_collect_events) {
    nlohmann::json e = nlohmann::json::object();
    e["t"] = "rm";
    e["id"] = userid;
    add_event_no_mutex(e);
  }
}

void game_net_events::update_player(const gs_mmo::player_context &ctx) {
  std::lock_guard<std::mutex> lock(m_mutex);
  if (m_resp["params"]["p"].contains(ctx.id())) {
    ctx.update_position_in_json(m_resp["params"]["p"][ctx.id()]);
  }
  if (m_collect_events) {
    bool found_event = false;
    for (auto &e: m_resp["params"]["e"]) {
      if (e["t"] == "mv" && e.contains("id") && e["id"] == ctx.id()) {
        ctx.update_position_in_json(e);
        found_event = true;
        // No need update m_last_events_ms
        break;
      }
    }
    if (!found_event) {
      nlohmann::json e = nlohmann::json::object();
      e["t"] = "mv";
      e["id"] = ctx.id();
      ctx.update_position_in_json(e);
      add_event_no_mutex(e);
    }
  }
}

std::string game_net_events::to_string_json_request() {
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_resp.dump();
}

long game_net_events::get_last_time_event_ms() const {
  return m_last_events_ms;
}

void game_net_events::add_event_no_mutex(const nlohmann::json &e_data) {
  auto now = std::chrono::system_clock::now().time_since_epoch();
  long now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();

  nlohmann::json e = e_data;
  e["n"] = now_ms;
  m_resp["params"]["e"].push_back(e);
  m_last_events_ms = now_ms;
}

} // namespace gs_mmo
