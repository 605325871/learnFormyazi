#pragma once

#include <pthread.h>

namespace Lyz {
namespace thread {
class Mutex {
  friend class Condition;

public:
  Mutex();
  ~Mutex();

  int lock();

  int try_lock();

  int unlock();

private:
  pthread_mutex_t m_mutex;
};
} // namespace thread
} // namespace Lyz