#pragma once

#include <string>
using namespace std;

#include "socket.h"

namespace Lyz {
namespace socket {

class ServerSocket : public Socket {
public:
  ServerSocket();
  ServerSocket(const string &ip, int port);
  virtual ~ServerSocket();
};

} // namespace socket
} // namespace Lyz
