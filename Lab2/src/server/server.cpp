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
          string dest = "HTTP/1.1 404 Not Found\r\n";

          dest += "Server: Lab Web Server\n";
          dest += "Content-type: text/html\n";
          dest += "Content-length: 112\n";

          dest += "\r\n";

          dest += "<html><title>404 Not Found</title><body bgcolor=ffffff>\n";
          dest += " Not Found \n";
          dest += "<hr><em>HTTP Web server</em>\n";
          dest += "</body></html>\n";
          int rc2 = newSocket.Send(dest.c_str(), dest.length());
          break;
        }
      }
      // newSocket.Close();
      // break;
    }
    // sleep(0.5);
  }
}