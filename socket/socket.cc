#include "socket.h"
#include "../utility/Logger.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>

using namespace Lyz::utility;
using namespace Lyz::socket;

#include <cerrno>
#include <cstring>

using namespace std;

Socket::Socket() : m_sockefd(0) {}

Socket::Socket(const string &ip, int port)
    : m_ip(ip), m_port(port), m_sockefd(0) {}

Socket::~Socket() { close(); }

bool Socket::bind(const string &ip, int port) {
  struct sockaddr_in sockaddr;
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;

  if (ip != "") {
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
  } else {
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  sockaddr.sin_port = htons(port);

  if (::bind(m_sockefd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
    error("socket bind error: errno=%d errstr=%s", errno, strerror(errno));
    return false;
  }
  return true;
}

bool Socket::connect(const string &ip, int port) {
  struct sockaddr_in sockaddr;
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
  sockaddr.sin_port = htons(port);
  if (::connect(m_sockefd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) <
      0) {
    error("socket connect error: errno=%d errstr=%s", errno, strerror(errno));
    return false;
  }
  return true;
}

bool Socket::close() {
  if (m_sockefd > 0) {
    ::close(m_sockefd);
    m_sockefd = 0;
  }
  return true;
}

int Socket::recv(char *buf, int len) { return ::recv(m_sockefd, buf, len, 0); }

int Socket::send(const char *buf, int len) {
  return ::send(m_sockefd, buf, len, 0);
}

bool Socket::set_non_blocking() {
  int flags = fcntl(m_sockefd, F_GETFL, 0);
  if (flags < 0) {
    error("Socket::set_non_blocking(F_GETFL, O_NONBLOCK): errno=%d errstr=%s",
          errno, strerror(errno));
    return false;
  }
  flags |= O_NONBLOCK;
  if (fcntl(m_sockefd, F_SETFL, flags) < 0) {
    error("Socket::set_non_blocking(F_SETFL, O_NONBLOCK): errno=%d errstr=%s",
          errno, strerror(errno));
    return false;
  }
  return true;
}
/*Linux下tcp连接断开的时候调用close()函数，有优雅断开和强制断开两种方式。

那么如何设置断开连接的方式呢？是通过设置socket描述符一个linger结构体属性。

linger结构体数据结构如下：

#include <arpa/inet.h>

struct linger {
　　int l_onoff;
　　int l_linger;
};
三种断开方式：
1. l_onoff = 0; l_linger忽略
close()立刻返回，底层会将未发送完的数据发送完成后再释放资源，即优雅退出。
2. l_onoff != 0; l_linger = 0;
close()立刻返回，但不会发送未发送完成的数据，而是通过一个REST包强制的关闭socket描述符，即强制退出。
3. l_onoff != 0; l_linger > 0;
close()不会立刻返回，内核会延迟一段时间，这个时间就由l_linger的值来决定。如果超时时间到达之前，发送完未发送的数据(包括FIN包)并得到另一端的确认，close()会返回正确，socket描述符优雅性退出。否则，close()会直接返回错误值，未发送数据丢失，socket描述符被强制性退出。
需要注意的时，如果socket描述符被设置为非堵塞型，则close()会直接返回值。*/



bool Socket::set_send_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockefd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        error("socket set send buffer error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_recv_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockefd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        error("socket set recv buffer error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_linger(bool active, int seconds)
{
    struct linger l;
    memset(&l, 0, sizeof(l));

    if (active)
        l.l_onoff = 1;
    else
        l.l_onoff = 0;
    l.l_linger = seconds;

    if (setsockopt(m_sockefd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0)
    {
        error("socket set sock linger error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_keep_alive()
{
    int flag = 1;
    if (setsockopt(m_sockefd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0)
    {
        error("socket set sock keep alive error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_reuse_addr()
{
    int flag = 1;
    if (setsockopt(m_sockefd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
    {
        error("socket set sock reuser addr error: errno=%s errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_reuse_port()
{
    int flag = 1;
    if (setsockopt(m_sockefd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) < 0)
    {
        error("socket set sock reuser port error: errno=%d errstr=%s", errno, strerror(errno));
        return false;
    }
    return true;
}
