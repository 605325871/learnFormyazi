#pragma once
#include <fstream>
#include <string>
using namespace std;
namespace Lyz {

namespace utility {
#define debug(format, ...)                                                     \
  Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format,    \
                          ##__VA_ARGS__)

#define info(format, ...)                                                      \
  Logger::instance()->log(Logger::info, __FILE__, __LINE__, format,     \
                          ##__VA_ARGS__)

#define warn(format, ...)                                                      \
  Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format,     \
                          ##__VA_ARGS__)

#define error(format, ...)                                                     \
  Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format,    \
                          ##__VA_ARGS__)

#define fatal(format, ...)                                                     \
  Logger::instance()->log(Logger::fatal, __FILE__, __LINE__, format,    \
                          ##__VA_ARGS__)

class Logger {
public:
  enum level { DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT };

  static Logger *instance();
  void open(const string &filename);
  void close();
  void log(level level, const char *file, int line, const char *format, ...);
  void max(int bytes);
  void setlevel(int level);

private:
  Logger();
  ~Logger();
  void rotate();
private:
  string m_filname;
  ofstream m_fout;
  int m_max;
  int m_len;
  int m_level;
  static const char *s_level[LEVEL_COUNT];
  static Logger *m_instance;
};
} // namespace utility

} // namespace Lyz
