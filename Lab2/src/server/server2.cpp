#include "../global.h"
#include "../transport.hpp"
#include "../fileOp.hpp"

void *listen(void* arg) {
  int clientSocket = (long) arg;
  char *buf = (char *)malloc(1024);
  memset(buf, 0, 1024);
  pthread_detach(pthread_self());
  TransSocket newSocket(&clientSocket);
  while (1) {
    cout << "1\n";
    size_t size;
    cout << "2\n";
    int rc = newSocket.Recv(buf, 1024,&size);
    cout << "3\n";
    if (!rc) {
      cout << "4\n";
      printf("%s\n", buf);
      break;
    }
    cout << rc << '\n';
  }
  newSocket.Close();
  return 0;
}

int main(int argc, char *argv[]) {

  pthread_t socket_thread;

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
      pthread_create(&socket_thread, NULL, listen, (void *)clientSocket);
      // break;
    }
    // sleep(0.5);
  }
}