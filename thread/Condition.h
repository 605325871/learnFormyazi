#pragma once
#include "Mutex.h"
#include <pthread.h>

namespace Lyz {
namespace thread {
class Condition {
public:
  Condition();
  ~Condition();

  int wait(Mutex *mutex);

  int signal();

  int broadcast();

protected:
  pthread_cond_t m_cond;
};

} // namespace thread
} // namespace Lyz