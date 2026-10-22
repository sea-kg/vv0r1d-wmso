
// TODO get variables from server
VVAPI_LAYER_BACKGROUND = 1;
VVAPI_LAYER_ROADS = 2;
VVAPI_LAYER_VEGETATION = 3;
VVAPI_LAYER_BUILDING = 4;

class VvApi {
  constructor() {
    this.base_url = location.protocol + "//" + location.host;
    this.ws_req_id = 0;
    this.player_x = 0;
    this.player_y = 0;
    this.ws_requests = {};
    this.ws_state = "connecting";   // "connecting" | "connected" | "disconnected"
    this.ws_state_listeners = [];

    var self = this;

    if ("WebSocket" in window) {
      this.ws = new WebSocket("ws://" + location.host + "/ws-api/");

      this.ws.onopen = function () {
        console.log("open");
        // still "connecting" until the auth response comes back
        if (!sessionStorage.getItem("random-session-token")) {
          var tkn = self.random_session_token();
          sessionStorage.setItem("random-session-token", tkn);
          self._set_ws_state("connected");
        }
        self.ws.send(JSON.stringify({
          "id": "auth",
          "method": "set_random_session_token",
          "params": {
            "token": sessionStorage.getItem("random-session-token")
          }
        }));
        self.ws.send(JSON.stringify({ "method": "hello" }));
        if (!localStorage.getItem("last-room")) {
          self.ws.send(JSON.stringify({ "method": "goto_room" }));
        }
      };

      this.ws.onmessage = function (ev) {
        var received_msg = ev.data;
        console.log("received_msg:", received_msg);
        var resp = JSON.parse(received_msg);
        var msg_id = resp["id"];

        if (msg_id == "auth") {
          this.player_x = resp["result"]["x"];
          this.player_y = resp["result"]["y"];
          self._set_ws_state("connected");
        }

        if (self.ws_requests[msg_id]) {
          self.ws_requests[msg_id].resolve(resp["result"]);
          delete self.ws_requests[msg_id];
        }
        console.log("received websocket message: " + received_msg);
      };

      this.ws.onclose = function () {
        console.log("closed");
        self._set_ws_state("disconnected");
      };

      this.ws.onerror = function () {
        console.log("ws error");
        self._set_ws_state("disconnected");
      };
    } else {
      console.log("not found WebSocket!");
      this.ws_state = "disconnected";
    }
  }

  _set_ws_state(state) {
    if (this.ws_state === state) return;
    this.ws_state = state;
    this.ws_state_listeners.forEach(fn => fn(state));
  }

  on_ws_state(fn) {
    this.ws_state_listeners.push(fn);
    fn(this.ws_state); // fire immediately with current state
    return () => {
      this.ws_state_listeners = this.ws_state_listeners.filter(f => f !== fn);
    };
  }

  random_session_token() {
    var length = 50;
    var result = '';
    var characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var charactersLength = characters.length;
    for (var i = 0; i < length; i++) {
      result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
    return result;
  };

  promise() {
    var d = {};
    d.completed = false;
    d.failed = false;
    d.successed = false;
    d.done = function (callback) {
      d.done_callback = callback;
      if (d.completed && typeof d.done_callback === "function" && d.successed) {
        d.done_callback.apply(this, d.result_arguments);
      }
      return d;
    }

    d.fail = function (callback) {
      d.fail_callback = callback;
      if (d.completed && typeof d.fail_callback === "function" && d.failed) {
        d.fail_callback.apply(this, d.error_arguments);
      }
      return d;
    }

    d.resolve = function () {
      if (!d.completed) {
        d.result_arguments = arguments; // [];
        if (typeof d.done_callback === "function") {
          d.done_callback.apply(this, d.result_arguments);
        }
      }
      d.successed = true;
      d.completed = true;
    }
    d.reject = function () {
      if (!d.completed) {
        d.error_arguments = arguments;
        if (typeof d.fail_callback === "function") {
          d.fail_callback.apply(this, d.error_arguments);
        }
      }
      d.failed = true;
      d.completed = true;
    }
    return d;
  };

  waitPromises(arr_promise) {
    var p = this.promise();
    var max_len = arr_promise.length;
    var result = [];
    function cmpl(r) {
      result.push(r);
      if (result.length == max_len) {
        p.resolve(result);
      }
    };
    for (var i in arr_promise) {
      arr_promise[i].done(cmpl).fail(cmpl);
    }
    return p;
  }

  request(path, d) {
    var xhr = new XMLHttpRequest()
    xhr.onload = function () {
      // alert(`Загружено: ${xhr.status} ${xhr.response}`);
    };
    xhr.onerror = function () { // происходит, только когда запрос совсем не получилось выполнить
      console.error('Ошибка соединения');
      d.reject("error");
    };
    xhr.onreadystatechange = function () {
      if (xhr.readyState != 4) {
        return
      }
      if (xhr.status === 200) {
        d.resolve(JSON.parse(xhr.responseText));
      } else {
        d.reject("error: " + xhr.responseText);
      }
    }
    xhr.open(
      'GET',
      this.base_url + path,
      true
    );
    xhr.send();
  }

  load_map() {
    var d = this.promise();
    this.request("/api/v1/get_map?v=1", d);
    return d;
  }

  ws_request(methodname, params, d) {
    var defer = d || this.promise();
    var _params = params || {};
    if (!this.ws_state) {
      var self = this;
      setTimeout(function () {
        self.ws_request(methodname, _params, defer);
      }, 100);
    } else {
      console.log(methodname, params);
      var new_id = this.ws_req_id++;
      this.ws.send(JSON.stringify({
        "id": new_id,
        "method": methodname,
        "params": params
      }))
      this.ws_requests[new_id] = defer;
    }
    return defer;
  }

  ws_change_player_position(x, y) {
    return this.ws_request("change_player_position", { "x": parseInt(x, 10), "y": parseInt(y, 10) });
  }

  ws_get_player_position() {
    var defer = this.promise();
    defer.resolve({
      "x": this.player_x,
      "y": this.player_y,
    });
    return defer;
    // console.log("get_player_position");
    // return this.ws_request("get_player_position", {});
  }

  ws_destroy_road(road) {
    console.log("ws_destroy_road");
    return this.ws_request("ws_destroy_road", road);
  }

  ws_get_map(data) {
    return this.ws_request("get_map", data);
  }
};
