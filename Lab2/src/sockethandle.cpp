#include "sockethandle.h"
#include "util.hpp"
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

 /**
   * @Author : Feng Yangyang
   */

// There is a circle queue.
vector<int> socketFlag;
// The server is one
bool serverOn = true;
extern cpLock * cplock;


void * socket_worker(void * arg)
{
  // get the id of the worker
  int id = (long) arg;

  while(1) 
  {
    if(!serverOn) {
      break;
    }
    // FIXME: if the the thread dosen't sleep, the program cannot be stopped.
    sleep(0.1);

    if(socketFlag[id] == 0) {
      continue;
    }
    TransSocket newSocket((SOCKET *)&socketFlag[id]);

    /*
      THE METHOD DURING THE CONNECTION:
        >>> Replace the method below:
    */

    char *buf = (char *)malloc(1024);
    memset(buf, 0, 1024);

    // recv
    int rc = newSocket.Recv(buf, 1024, TRANS_SOCKET_DFT_TIMEOUT, NULL);
    printf("%s\n", buf);

    // send
    std::string c = "test from thread\n";
    newSocket.Send(c.c_str(), c.size());

    free(buf);

    /*
      THE METHOD DURING THE CONNECTION
        >>> Replace the method above:
    */


    newSocket.Close();
    
    // release the seme
    cplock->cGet();
    socketFlag[id] = 0;
    cplock->cRelease();
  }
}

void * thread_scheduling(void * arg)
{
  int lastFitPos = 0;
  int rc = SE_OK;

  unsigned int myPort = *port;

  // set the port
  TransSocket serverSock(myPort);

  // init the socket
  rc = serverSock.initSocket();
  PD_DEBUG(rc);

  // listen the port
  rc = serverSock.bindListen();
  PD_DEBUG(rc);

  while(1)
  {
    if(!serverOn) {
      break;
    }
    sleep(0.1);

    int clientSocket;

    // accecept a connect requset
    rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);

    if (rc == SE_OK) {
      cplock->pGet();
      // chose the free pos
      while(socketFlag[lastFitPos] != 0) {
          lastFitPos = (lastFitPos + 1) % *thread_num;
      }
      // insert inti the circle list
      socketFlag[lastFitPos] = clientSocket;
      cplock->pRelease();
    }

  }
  serverSock.Close();
}

