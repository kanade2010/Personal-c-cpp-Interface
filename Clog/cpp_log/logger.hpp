#pragma once

#include <memory>
#include <mutex>
#include <string>

namespace sola {

//!
//! logger_iface
//! should be inherited by any class intended to be used for logging
//!
class logger_iface {
public:
  //! ctor
  logger_iface(void) = default;
  //! dtor
  virtual ~logger_iface(void) = default;

  //! copy ctor
  logger_iface(const logger_iface&) = default;
  //! assignment operator
  logger_iface& operator=(const logger_iface&) = default;

public:
  //!
  //! debug logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void debug(const std::string& msg, const std::string& file, std::size_t line) = 0;

  //!
  //! info logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void info(const std::string& msg, const std::string& file, std::size_t line) = 0;

  //!
  //! warn logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void warn(const std::string& msg, const std::string& file, std::size_t line) = 0;

  //!
  //! error logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  virtual void error(const std::string& msg, const std::string& file, std::size_t line) = 0;
};

//!
//! default logger class provided by the library
//!
class logger : public logger_iface {
public:
  //!
  //! log level
  //!
  enum class log_level {
    error = 0,
    warn  = 1,
    info  = 2,
    debug = 3
  };

public:
  //! ctor
  logger(log_level level = log_level::debug);
  //! dtor
  ~logger(void) = default;

  //! copy ctor
  logger(const logger&) = default;
  //! assignment operator
  logger& operator=(const logger&) = default;

public:
  //!
  //! debug logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void debug(const std::string& msg, const std::string& file, std::size_t line);

  //!
  //! info logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void info(const std::string& msg, const std::string& file, std::size_t line);

  //!
  //! warn logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void warn(const std::string& msg, const std::string& file, std::size_t line);

  //!
  //! error logging
  //!
  //! \param msg message to be logged
  //! \param file file from which the message is coming
  //! \param line line in the file of the message
  //!
  void error(const std::string& msg, const std::string& file, std::size_t line);

private:
  //!
  //! current log level in use
  //!
  log_level m_level;

  //!
  //! mutex used to serialize logs in multithreaded environment
  //!
  std::mutex m_mutex;
};

//!
//! variable containing the current logger
//! by default, not set (no logs)
//!
extern std::unique_ptr<logger_iface> active_logger;

//!
//! debug logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void debug(const std::string& msg, const std::string& file, std::size_t line);

//!
//! info logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void info(const std::string& msg, const std::string& file, std::size_t line);

//!
//! warn logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void warn(const std::string& msg, const std::string& file, std::size_t line);

//!
//! error logging
//! convenience function used internally to call the logger
//!
//! \param msg message to be logged
//! \param file file from which the message is coming
//! \param line line in the file of the message
//!
void error(const std::string& msg, const std::string& file, std::size_t line);

//!
//! convenience macro to log with file and line information
//!
/*#ifdef __SOLA_LOGGING_ENABLED
#define __SOLA_LOG(level, msg) sola::level(msg, __FILE__, __LINE__);
#else
#define __SOLA_LOG(level, msg)
#endif /* __SOLA_LOGGING_ENABLED
*/
#define __SOLA_LOG(level, msg) sola::level(msg, __FILE__, __LINE__);

} // namespace sola
