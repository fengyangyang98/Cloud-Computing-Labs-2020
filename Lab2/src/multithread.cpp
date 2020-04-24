#include "multithread.h"

void make_respose(char *buf) {}

void *listen(void *arg) {
  int clientSocket = (long)arg;
  char *buf = (char *)malloc(1024);
  memset(buf, 0, 100);
  pthread_detach(pthread_self());
  TransSocket newSocket(&clientSocket);
  string http_buf = "";
  std::map<string, string> post;
  while (1) {
    size_t* length;
    int rc = newSocket.Recv(buf, 1024, length);
    // newSocket.Send(f_buf, l);
    if (!rc) {
      make_respose(buf);
      printf("%s\n", buf);
      break;
    }
    cout << rc << '\n';
  }
  newSocket.Close();
  return 0;
}

void handle() {
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
    long clientSocket;
    // accecept a connect requset
    rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);
    if (rc == SE_OK) {
      pthread_create(&socket_thread, NULL, listen, (void *)clientSocket);
      // break;
    }
    // sleep(0.5);
  }
}