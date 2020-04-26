#include "sockethandle.h"
#include <regex>

using std::regex;

queue<int> client_socket_queue;
exclusiveLock client_socket_lock;
cpLock queueLock;

void *get_socket_func(void *arg) {
  int rc = SE_OK;
  // set the port
  TransSocket serverSock(*port);

  // init the socket
  rc = serverSock.initSocket();
  PD_DEBUG(rc);

  // listen the port
  rc = serverSock.bindListen();
  PD_DEBUG(rc);

  while (1) {
    long client_socket;
    // accecept a connect requset
    client_socket_lock.get();
    rc = serverSock.Accept((SOCKET *)&client_socket, NULL, NULL);
    if (rc == SE_OK) {
      queueLock.cGet();
      client_socket_queue.push(client_socket);
      queueLock.cRelease();
    }
    client_socket_lock.release();
  }
}

void error_respose(char *src, char *dest) { return; }

void post_respose(char *src, char *dest) { return; }

void get_respose(char *src, char *dest) {
  
  return;
}

void make_respose(char *src, char *dest) {
  regex get("GET");
  regex post("POST");
  regex version("HTTP/1.1");
  if (regex_match(src, version)) {
    if (regex_match(src, get)) {
      get_respose(src, dest);
    } else if (regex_match(src, post)) {
      post_respose(src, dest);
    }
  } else {
    error_respose(src, dest);
  }
  return;
}

void *handle_requsts(void *arg) {
  while (1) {

    queueLock.pGet();
    int client_socket = client_socket_queue.front();
    client_socket_queue.pop();
    queueLock.pRelease();

    TransSocket newSocket(&client_socket);
    size_t size;
    char buf[2048], res[2048];
    memset(buf, 0, 2048);
    memset(res, 0, 2048);
    int rc = newSocket.Recv(buf, 2048, TRANS_SOCKET_DFT_TIMEOUT, &size);
    PD_DEBUG(rc);
    make_respose(buf, res);
    int l = strlen(res);
    newSocket.Send(res, l);

  }
}

void thread_manager() {
  pthread_t handles[*thread_num - 2];
  for (int i = 0; i < *thread_num - 2; ++i) {
    pthread_create(&handles[i], NULL, handle_requsts, NULL);
  }
}