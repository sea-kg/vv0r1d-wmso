#include "gs_mmo_error_info.h"

namespace gs_mmo {

error_info::error_info() : m_error_code(0) {

}

error_info::error_info(int error_code, const std::string &error_msg)
: m_error_code(error_code), m_error_msg(error_msg) {

}

int error_info::error_code() const {
  return m_error_code;
}

std::string error_info::error_msg() const {
  return m_error_msg;
}

} // namespace gs_mmo
