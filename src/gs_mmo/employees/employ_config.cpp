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

#include <wsjcpp_employees.h>
// #include <wsjcpp_yaml.h>
#include <wsjcpp_core.h>
#include <sstream>
#include <thread>
#include <cstring>
#include <ctime>
#include <locale>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sea5kg_logger.h>
#include "gs_mmo/include/gs_mmo_globals.h"
#include "gs_mmo/include/gs_mmo_config.h"
// #include "ctf01d/include/ctf01d_web_server.h"
#include "third_party/smallsha1/smallsha1.h"
#include "third_party/HowardHinnant/date.h"
#include <sys/stat.h>
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

class employ_config : public WsjcppEmployBase, public gs_mmo::config {
public:
  employ_config();
  ~employ_config();
  virtual bool init(const std::string &name, bool silent) override;
  virtual bool deinit(const std::string &name, bool silent) override;

  // gs_mmo::config
  virtual void set_app_version(const std::string &ctf01d_version) override;
  virtual std::string app_version() override;
  virtual void set_work_dir(const std::string &work_dir) override;
  virtual std::string get_work_dir() override;
  virtual const std::string &db_dir() override;
  virtual int web_port() const override;
  virtual std::string html_folder() const override;

private:
  void update_files_in_data();
  bool apply_config();
  // nlohmann::json load_files_sha1();
  // void save_files_sha1(nlohmann::json &files);
  // void update_data_html(nlohmann::json &files);

  // bool checkYamlMainKeys(WsjcppYaml &yaml);
  bool apply_web_port_from_env();
  // bool applyServicesConfig(WsjcppYaml &yaml);
  // bool readTeamsConf(WsjcppYaml &yaml);
  bool init_work_dir();
  bool init_logger();

  std::string TAG;
  std::string m_app_version;
  std::string m_work_dir;
  std::string m_db_dir;
  std::string m_config_filepath;
  bool m_applied_config;

//   // scoreboard config
//   ctf01d::scope_vars m_scoreboard_vars = ctf01d::scope_vars("scoreboard_config");
//   std::shared_ptr<ctf01d::var_int> m_scoreboard_port;
//   std::shared_ptr<ctf01d::var_dir> m_scoreboard_html_folder;
//   std::shared_ptr<ctf01d::var_bool> m_scoreboard_auto_detection_team_id_by_subnet_ip;
//   std::shared_ptr<ctf01d::var_bool> m_scoreboard_random;
//   std::shared_ptr<ctf01d::var_bool> m_scoreboard_metrics_enabled;
//   std::shared_ptr<ctf01d::var_allowed_ip> m_scoreboard_metrics_allowed_for;

//   // game config
//   ctf01d::game_config m_game_config;

//   ctf01d::scope_vars m_teams_config_vars = ctf01d::scope_vars("teams_config");
//   std::shared_ptr<ctf01d::var_string> m_ip_or_host_prefix;
//   std::shared_ptr<ctf01d::var_string> m_ip_or_host_suffix;

//   // teams config
//   std::vector<ctf01d::team_config> m_teams;
//   std::map<std::string, ctf01d::team_config> m_teams_cache;
//   std::vector<std::string> m_teams_ip_or_host;
//   std::map<std::string, std::string> m_teams_subnets;

//   // services config
//   std::vector<ctf01d::service_config> m_services;

//   // hot-reload: for reload config in runtime
//   std::thread m_thread_watcher;
//   std::mutex m_mutex_thread_watcher;
};

REGISTRY_WSJCPP_EMPLOY(employ_config)

employ_config::employ_config()
: WsjcppEmployBase({ gs_mmo::config::name() }, {}) {
  TAG = gs_mmo::config::name();
  m_app_version = "v??";

  // game options
  m_applied_config = false;

  // scoreboard config
  // m_scoreboard_port = ctf01d::var_int::create({"scoreboard", "port"}, 8080, m_scoreboard_vars);
  // m_scoreboard_port->set_minimum(ctf01d::MIN_TCP_PORT);
  // m_scoreboard_port->set_maximum(ctf01d::MAX_TCP_PORT);
  // m_scoreboard_random = ctf01d::var_bool::create({"scoreboard", "random"}, false, m_scoreboard_vars);
  // m_scoreboard_html_folder = ctf01d::var_dir::create({"scoreboard", "html-dir-path"}, "./html", m_work_dir, m_scoreboard_vars);
}

employ_config::~employ_config() {
  // TODO cleanup
  // m_scoreboard_vars.clear();
}

bool employ_config::init(const std::string &sName, bool bSilent) {
  if (!init_work_dir()) {
    return false;
  }

  if (!init_logger()) {
    return false;
  }

  this->update_files_in_data();

  // if (!this->apply_config()) {
  //   sea5kg::log::error(TAG, "Configuration file has some problems");
  //   return false;
  // }

  return true;
}

bool employ_config::deinit(const std::string &sName, bool bSilent) {
  sea5kg::log::info(TAG, "deinit");
  // wait stop threads
  // if (m_thread_watcher.joinable()) {
  //   m_thread_watcher.join();
  // }
  return true;
}

void employ_config::set_app_version(const std::string &ver) {
  m_app_version = ver;
}

std::string employ_config::app_version() {
  return m_app_version;
}

void employ_config::set_work_dir(const std::string &sWorkDir) {
  if (m_work_dir != "" && m_work_dir != sWorkDir) {
    std::cout << "Changed work-dir to '" + sWorkDir + "'" << std::endl;
  }
  m_work_dir = sWorkDir;
  m_config_filepath = m_work_dir + "/config.yml";
  // m_scoreboard_html_folder->set_root_dir(m_work_dir);
}

std::string employ_config::get_work_dir() {
    return m_work_dir;
}

const std::string &employ_config::db_dir() {
  return m_db_dir;
}

bool employ_config::apply_config() {
  if (m_applied_config) {
    return true;
  }

  m_applied_config = false;
  sea5kg::log::info(TAG, "Loading configuration...");

  sea5kg::log::info(TAG, "Reading config: " + m_config_filepath);

  if (!wsjcpp::file_exists(m_config_filepath)) {
    sea5kg::log::error(TAG, "File " + m_config_filepath + " does not exists");
    return false;
  }

//   WsjcppYaml yaml;
//   std::string sError;
//   if (!yaml.loadFromFile(m_config_filepath, sError)) {
//     sea5kg::log::error(TAG, "Could not parse " + m_config_filepath + ", reason: " + sError);
//     return false;
//   }

//   if (!checkYamlMainKeys(yaml)) {
//     return false;
//   }

//   auto cursor = yaml.getCursor();
//   std::string err;
//   if (!m_game_config.read(cursor, m_work_dir, err)) {
//     sea5kg::log::error(TAG, err);
//     return false;
//   }

//   if (!m_scoreboard_vars.read(cursor, err)) {
//     sea5kg::log::error(TAG, err);
//     return false;
//   }

  // GS_MMO_PORT
  if (!apply_web_port_from_env()) {
    return false;
  }

  m_applied_config = true;
  return m_applied_config;
}

int employ_config::web_port() const {
  // TODO
  return 1234;
  // return m_scoreboard_port->value();
}

std::string employ_config::html_folder() const {
  // TODO
  return "./html";
  // return m_scoreboard_html_folder->value();
}

// // helper
// std::string sha1_by_string(const std::string &data) {
//   char hexstring[41]; // 40 chars + a zero
//   std::memset(hexstring, 0, sizeof hexstring);

//   unsigned char hash[20];
//   sha1::calc(data.c_str(), data.length(), hash);
//   sha1::toHexString(hash, hexstring);
//   return std::string(hexstring);
// }

// std::string sha1_by_data(const char *data, int len) {
//   char hexstring[41]; // 40 chars + a zero
//   std::memset(hexstring, 0, sizeof hexstring);

//   unsigned char hash[20];
//   sha1::calc(data, len, hash);
//   sha1::toHexString(hash, hexstring);
//   return std::string(hexstring);
// }

// std::string sha1_by_file(const std::string &sFilename) {
//   std::ifstream f(sFilename, std::ifstream::binary);
//   if (!f) {
//     return "Could not open file";
//   }
//   // get length of file:
//   f.seekg (0, f.end);
//   int nBufferSize = f.tellg();
//   f.seekg (0, f.beg);
//   char *pBuffer = new char [nBufferSize];
//   // read data as a block:
//   f.read(pBuffer, nBufferSize);
//   if (!f) {
//     delete[] pBuffer;
//     // f.close();
//     sea5kg::log::critical("sha1_by_file", "Could not read file. Only " + std::to_string(f.gcount()) + " could be read");
//     return "";
//   }
//   f.close();
//   char hexstring[41]; // 40 chars + a zero
//   std::memset(hexstring, 0, sizeof hexstring);
//   unsigned char hash[20];
//   sha1::calc(pBuffer, nBufferSize, hash);
//   sha1::toHexString(hash, hexstring);
//   delete[] pBuffer;
//   return std::string(hexstring);
// }

void employ_config::update_files_in_data() {
  std::string sError;
  if (!wsjcpp::dir_exists(m_work_dir + "/logs")) {
    WsjcppCore::makeDir(m_work_dir + "/logs");
    if (!WsjcppCore::setFilePermissions(m_work_dir + "/logs", WsjcppFilePermissions(0x755), sError)) {
      sea5kg::log::critical(TAG, sError);
    }
  }

  // nlohmann::json previous_files_sha1 = load_files_sha1();

  // if (!wsjcpp::file_exists(m_work_dir + "/config.yml")) {
  //   sea5kg::log::warning(TAG, "Extracting config.yml and files");
  //   sea5kg::log::warning(TAG, "Extracting checker_example_*");
  //   const std::vector<WsjcppResourceFile*> &vFiles = WsjcppResourcesManager::list();
  //   std::vector<std::string> vExecutableFiles;
  //   for (int i = 0; i < vFiles.size(); i++) {
  //     std::string filepath = vFiles[i]->getFilename();
  //     if (filepath.rfind("./data_sample/checker_example_", 0) == 0) {
  //       std::vector<std::string> vPath = wsjcpp::split(filepath, "/");
  //       std::string sDirname = vPath[2];
  //       vPath.erase (vPath.begin(),vPath.begin()+3);
  //       std::string sNewFilepath = wsjcpp::join(vPath, "/");
  //       sNewFilepath = wsjcpp::normalize_filepath(m_work_dir + "/" + sDirname + "/" + sNewFilepath);
  //       if (!wsjcpp::file_exists(sNewFilepath)) {
  //         std::cout << "Extracting file '" << filepath << "' to '" << sNewFilepath << "'" << std::endl;
  //       } else {
  //         std::cout << "File '" << sNewFilepath << "' already exists. Skip." << std::endl;
  //         continue;
  //       }

  //       // prepare folder
  //       std::string sFolder = wsjcpp::normalize_filepath(m_work_dir + "/" + sDirname + "/");
  //       if (!wsjcpp::dir_exists(sFolder)) {
  //         WsjcppCore::makeDir(sFolder);
  //       }

  //       if (!WsjcppCore::writeFile(sNewFilepath, vFiles[i]->getBuffer(), vFiles[i]->getBufferSize())) {
  //         std::cout << "ERROR. Could not write file. " << std::endl;
  //         continue;
  //       } else {
  //         std::cout << "Successfully created file. " << std::endl;
  //         // TODO redesign set permission via wsjcpp
  //         if (chmod(sNewFilepath.c_str(), S_IRWXU|S_IRWXG) != 0) {
  //           std::cout << "ERROR. Could not change permissions for. " << sNewFilepath << std::endl;
  //         } else {
  //           struct stat info;
  //           stat(sNewFilepath.c_str(), &info);
  //           printf("after chmod(), permissions are: %08x\n", info.st_mode);
  //         }
  //       }
  //     }
  //   }

  //   WsjcppResourceFile* pConfigYml = WsjcppResourcesManager::get("./data_sample/config.yml");
  //   std::string sNewFilepath = wsjcpp::normalize_filepath(m_work_dir + "/config.yml");
  //   if (!WsjcppCore::writeFile(sNewFilepath, pConfigYml->getBuffer(), pConfigYml->getBufferSize())) {
  //     std::cout << "ERROR. Could not write file. " << std::endl;
  //   } else {
  //     std::cout << "Successfully created file. " << std::endl;
  //   }
  // }

  // update_data_html(previous_files_sha1);
  // save_files_sha1(previous_files_sha1);
}

// nlohmann::json employ_config::load_files_sha1() {
//   nlohmann::json files_sha1;
//   if (wsjcpp::file_exists(m_work_dir + "/files_sha1.json")) {
//     std::ifstream ifs(m_work_dir + "/files_sha1.json");
//     files_sha1 = nlohmann::json::parse(ifs);
//   }
//   return files_sha1;
// }

// void employ_config::save_files_sha1(nlohmann::json &files) {
//   std::ofstream output(m_work_dir + "/files_sha1.json");
//   output << std::setw(2) << files << std::endl;
// }

// void employ_config::update_data_html(nlohmann::json &previous_files_sha1) {
//   sea5kg::log::warning(TAG, "Updating files in data/html");
//   if (!wsjcpp::dir_exists(m_work_dir + "/html")) {
//     WsjcppCore::makeDir(m_work_dir + "/html");
//   }

//   const std::vector<WsjcppResourceFile*> &vFiles = WsjcppResourcesManager::list();
//   for (int i = 0; i < vFiles.size(); i++) {
//     std::string source_filepath = vFiles[i]->getFilename();
//     if (source_filepath.rfind("./data_sample/html/", 0) != 0) {
//       continue;
//     }
//     // remove base folder
//     std::vector<std::string> vPath = wsjcpp::split(source_filepath, "/");
//     vPath.erase (vPath.begin(),vPath.begin()+3);
//     std::string target_filepath = wsjcpp::join(vPath, "/");
//     target_filepath = wsjcpp::normalize_filepath(m_work_dir + "/html/" + target_filepath);

//     // prepare folders
//     if (!wsjcpp::file_exists(target_filepath)) {
//       std::string dirpath = wsjcpp::normalize_filepath(m_work_dir + "/html/");
//       for (int p = 0; p < vPath.size()-1; p++) {
//         dirpath = wsjcpp::normalize_filepath(dirpath + "/" + vPath[p]);
//         if (!wsjcpp::dir_exists(dirpath)) {
//           if (!WsjcppCore::makeDir(dirpath)) {
//             std::cout << "ERROR. Could not create: " << dirpath << std::endl;
//             continue;
//           }
//         }
//       }
//     }

//     std::string previous_sha1 = "";
//     if (previous_files_sha1.contains(source_filepath)) {
//       previous_sha1 = previous_files_sha1[source_filepath];
//     }

//     if (wsjcpp::file_exists(target_filepath) && previous_sha1 != "") {
//       if (previous_sha1 != sha1_by_file(target_filepath)) {
//         // Skip. file has changes by user. Skip.
//         std::cout << "Warning. Could not override file, because has changes: " << target_filepath << std::endl;
//         continue;
//       }
//     }

//     std::string new_sha1 = sha1_by_data(vFiles[i]->getBuffer(), vFiles[i]->getBufferSize());
//     if (wsjcpp::file_exists(target_filepath) && new_sha1 == previous_sha1) {
//       // Skip. file has same content
//       continue;
//     }

//     if (!WsjcppCore::writeFile(target_filepath, vFiles[i]->getBuffer(), vFiles[i]->getBufferSize())) {
//       std::cout << "ERROR. Could not write/override file. " << std::endl;
//       continue;
//     }

//     std::cout << "Successfully created/updated file: " << target_filepath << std::endl;
//     std::string err;
//     if (!WsjcppCore::setFilePermissions(target_filepath, WsjcppFilePermissions(0x644), err)) {
//       sea5kg::log::critical(TAG, err);
//     }
//     previous_files_sha1[source_filepath] = new_sha1;
//   }
// }

// bool employ_config::checkYamlMainKeys(WsjcppYaml &yaml) {
//   // check main keys
//   auto cur = yaml.getCursor();
//   // TODO list from vars
//   std::vector<std::string> expected_keys = {
//     "scoreboard",
//     "game",
//     "services",
//     "teams",
//   };
//   std::vector<std::string> main_keys = cur.keys();
//   for (int i = 0; i < main_keys.size(); i++) {
//     if (std::find(expected_keys.begin(), expected_keys.end(), main_keys[i]) == expected_keys.end()) {
//       sea5kg::log::error(TAG, "Got unexpected key in main: '" + main_keys[i] + "'");
//       return false;
//     }
//   }
//   for (int i = 0; i < expected_keys.size(); i++) {
//     if (std::find(main_keys.begin(), main_keys.end(), expected_keys[i]) == main_keys.end()) {
//       sea5kg::log::error(TAG, "Not found expected key in config: '" + expected_keys[i] + "'");
//       return false;
//     }
//   }
//   return true;
// }

bool employ_config::apply_web_port_from_env() {
  // TODO
  std::string str_port;
  if (WsjcppCore::getEnv(gs_mmo::keys::GS_MMO_PORT, str_port)) {
  //  sea5kg::log::warning(TAG, ctf01d::keys::CTF01D_PORT + "='" + str_port + "'");
  //   try {
  //     int port = std::stoi(str_port);
  //     std::string err;
  //     if (!m_scoreboard_port->set_value(port, err)) {
  //       sea5kg::log::error(TAG, ctf01d::keys::CTF01D_PORT + "='" + str_port + "' is wrong. " + err);
  //       return false;
  //     }
  //   } catch (const std::invalid_argument& e) {
  //     sea5kg::log::error(TAG, "No conversion could be performed. " + ctf01d::keys::CTF01D_PORT + "='" + str_port + "'");
  //     std::cerr << "Error: \n";
  //     return false;
  //   } catch (const std::out_of_range& e) {
  //     sea5kg::log::error(TAG, "The converted value is too big for an int.. " + ctf01d::keys::CTF01D_PORT + "='" + str_port + "'");
  //     return false;
  //   } catch (...) {
  //     sea5kg::log::error(TAG, "The converted value is too big for an int.. " + ctf01d::keys::CTF01D_PORT + "='" + str_port + "'");
  //     return false;
  //   }
  //   sea5kg::log::info(TAG, "scoreboard.port will be overridden from environment variable. " + ctf01d::keys::CTF01D_PORT + "='" + str_port + "'");
  //   return true;
  }
  return true;
}

bool employ_config::init_work_dir() {
  sea5kg::log::info(TAG, "Work Directory is " + m_work_dir);
  std::string sWorkDir = this->get_work_dir();
  if (sWorkDir == "") {
    sea5kg::log::critical(TAG, "Work Directory not defined.");
    return false;
  }
  if (!wsjcpp::dir_exists(sWorkDir)) {
    sea5kg::log::error(TAG, "Directory " + sWorkDir + " does not exists");
    return false;
  }

  m_db_dir = wsjcpp::normalize_filepath(m_work_dir + "/db");
  if (!wsjcpp::dir_exists(m_db_dir)) {
    if (!WsjcppCore::makeDir(m_db_dir)) {
      sea5kg::log::error(TAG, "Could not create " + m_db_dir);
      return false;
    }
    std::string error;
    if (!WsjcppCore::setFilePermissions(m_db_dir, WsjcppFilePermissions(0x776), error)) {
      sea5kg::log::critical(TAG, error);
      return false;
    }
  }
  return true;
}

bool employ_config::init_logger() {
  // init logger
  std::string sLogDir = m_work_dir + "/logs/" + WsjcppCore::getCurrentTimeForFilename();
  sLogDir = wsjcpp::normalize_filepath(sLogDir);
  if (!wsjcpp::dir_exists(sLogDir)) {
    if (!WsjcppCore::makeDirsPath(sLogDir)) {
      sea5kg::log::error(TAG, "Could not make dirs for logs: " + sLogDir);
      return false;
    }
    std::string sError;
    if (!WsjcppCore::setFilePermissions(sLogDir, WsjcppFilePermissions(0x776), sError)) {
      sea5kg::log::critical(TAG, sError);
    }
  }
  if (!wsjcpp::dir_exists(sLogDir)) {
    std::cout << "Error: Folder '" << sLogDir << "' does not exists and could not created, please check access rights to parent folder.\n";
    return false;
  }
  sea5kg::log::set_log_filename_prefix("ctf01d_");
  sea5kg::log::set_log_dirpath(sLogDir);
  sea5kg::log::set_rotation_period_in_seconds(600); // every 10 min  // TODO rotation period must be in config.yml
  sea5kg::log::set_log_level_file_output(sea5kg::log_level::DEBUG);
  std::cout << "Logger: '" + sLogDir + "' \n";
  return true;
}

