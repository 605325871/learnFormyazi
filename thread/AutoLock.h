#pragma once

#include "Mutex.h"

namespace Lyz {
namespace thread {
class AutoLock {
public:
  AutoLock(Mutex* mutex);
  ~AutoLock();

private:
  Mutex *m_mutex;
};

} // namespace thread
} // namespace Lyz