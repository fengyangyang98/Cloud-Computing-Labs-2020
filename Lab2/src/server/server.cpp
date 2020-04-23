#include "../global.h"
#include "../transport.hpp"
#include "../fileOp.hpp"

void *listen(void* arg) {
  int clientSocket = (long) arg;
  char *buf = (char *)malloc(100);
  memset(buf, 0, 100);
  pthread_detach(pthread_self());
  TransSocket newSocket(&clientSocket);
  while (1) {
    int rc = newSocket.Recv(buf, 100);
    fileOp f;
    f.Open("./test.html");
    int l = f.getSize();
    char f_buf[l];
    f.readAt(0,l,f_buf,NULL);
    newSocket.Send(f_buf,l);
    if (!rc) {
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