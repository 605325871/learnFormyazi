#pragma once
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace Lyz
 {
namespace socket {

class EventPoller {
  friend class SocketHandler;

public:
  EventPoller(bool et = true);
  ~EventPoller();

  void create(int max_connections);

  void add(int fd, void *ptr, __uint32_t events);

  void mod(int fd, void *ptr, __uint32_t events);

  void del(int fd, void *ptr, __uint32_t events);

  int wait(int millsecond);

protected:
  /*
    EPOLLPRI 表示对应的文件描述符有紧急的数据可读
                 EPOLLERR 表示对应的文件描述符发生错误
                   EPOLLHUP 表示对应的文件描述符被挂断
                  EPOLLET  表示对应的文件描述符有事件发生
  */

  void ctrl(int fd, void *ptr, __uint32_t events, int op);

protected:
  int m_epfd;

  int m_max_connections;

  struct epoll_event *m_events;

  bool m_et;
};

} // namespace socket
} // namespace yazi
