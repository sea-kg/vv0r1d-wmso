#pragma once

#include <string>

namespace gs_mmo {

class web_server {
public:
  static std::string name() { return "web_server"; }
  virtual int start() = 0;
};

} // namespace gs_mmo
