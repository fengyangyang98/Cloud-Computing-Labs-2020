#include "../fileOp.hpp"
#include "../global.h"
#include "../transport.hpp"

int main(int argc, char *argv[]) {

  int rc = SE_OK;
  // set the port
  TransSocket serverSock(12345);

  // init the socket
  rc = serverSock.initSocket();
  PD_DEBUG(rc);

  // listen the port
  rc = serverSock.bindListen();
  PD_DEBUG(rc);

  while (1) {
    int clientSocket;
    // accecept a connect requset
    rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);
    if (rc == SE_OK) {
      char *buf = (char *)malloc(1024);
      memset(buf, 0, 1024);
      TransSocket newSocket(&clientSocket);
      while (1) {
        size_t size;
        int rc = newSocket.Recv(buf, 1024, TRANS_SOCKET_DFT_TIMEOUT, NULL);
        if (rc == SE_OK) {
          cout << buf << "\n";
          char *buf2 = (char *)malloc(1026);
          memset(buf2, '1', 1024);
          buf2[1024] = '2';
          buf2[1025] = '\0';
          int rc2 = newSocket.Send(buf2, 1026);
          break;
        }
      }
      newSocket.Close();
      // break;
    }
    // sleep(0.5);
  }
}