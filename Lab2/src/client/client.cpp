#include "../global.h"
#include "../transport.hpp"
#include "math.h"

int main(int argc, char *argv[]) {
  int rc = SE_OK;
  TransSocket clientSock;
  // set the port
  clientSock.setAddress("localhost", 12345);
  // init the socket
  clientSock.initSocket();
  // connect
  rc = clientSock.Connect();
  assert(rc == SE_OK);
  // disable the small package to transport
//   clientSock.disableNagle();

  std::string str = "message 1\n";
  clientSock.Send(str.c_str(), str.length());

  char buffer[2048];
  memset(buffer, 0, 2048);
  int length = 1026;
  while (!rc) {
    size_t size;
    rc = clientSock.Recv(buffer, 1024, TRANS_SOCKET_DFT_TIMEOUT, &size);
    if (!rc){
        cout << buffer;
        // cout << '\n' << size << '\n';
        // break;
    }
  }
  
}