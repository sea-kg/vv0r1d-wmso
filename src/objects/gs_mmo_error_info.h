#pragma once

#include <string>

namespace gs_mmo {

class error_info {
public:
  error_info();
  error_info(int error_code, const std::string &error_msg);
  int error_code() const;
  std::string error_msg() const;

private:
  int m_error_code;
  std::string m_error_msg;
};

} // namespace gs_mmo
