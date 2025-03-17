#include <iostream>

class Logger {
public:
  /**
   * Logs the given message to the standard output, type info
   *
   * @param message The message to be logged.
   */

  static void LOGI(const std::string &message) {
    std::cout << "INFO: " << message << std::endl;
  }

  /**
   * Logs the given message to the standard output, type warning
   *
   * @param message The message to be logged.
   */

  void LOGW(const std::string &message) const {
    std::cout << "WARN: " << message << std::endl;
  }

  /**
   * Logs the given message to the standard output, type error
   *
   * @param message The message to be logged.
   */

  void LOGE(const std::string &message) const {
    std::cout << "ERR: " << message << std::endl;
  }
};