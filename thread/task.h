#pragma once
#include "Mutex.h"

using namespace Lyz::thread;
namespace Lyz {
namespace thread {
class Task {
public:
  Task();
  Task(void *data);
  virtual ~Task();

  void *get_data();
  void set_data(void *data);

  virtual void run() = 0;
  virtual void destory() = 0;

protected:
  void *m_data;
  Mutex m_mutex;
};
} // namespace thread
} // namespace yazi