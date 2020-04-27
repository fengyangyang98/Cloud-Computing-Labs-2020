#include "sockethandle.h"
#include "fileOp.hpp"
#include "util.hpp"
#include <regex>
#include "sstream"

namespace this_debug {
static unsigned debug_counter_1 = 0;
void break_point_1() {
  cout << " ---- default " << debug_counter_1++ << " ---- \n";
}
void break_point(int i){
  cout << " ---- HANDLE " << i << " ---- \n";
}
} // namespace this_debug

using std::regex;

queue<int> client_socket_queue;
exclusiveLock client_socket_lock;
cpLock queueLock;

void error_respose(string *dest, string *src) {
  *dest = "HTTP/1.1 404 Not Found\r\n";
  *dest += "Server: Lab Web Server\n";
  *dest += "Content-type: text/html\n";
  *dest += "Content-length: 115";
  fileOp fop;
  fop.Open("./404.html",S_FILE_OP_READ_ONLY);
  size_t size = fop.getSize();
  dest += size;
  *dest += "\n\r\n";
  
  char buf[size];
  buf[size - 1] = '\0';
  fop.readAt(0, size, buf, NULL);
  *dest += buf;
  fop.Close();
  return;
}

void post_respose(string *dest, string *src) {
  *dest = "HTTP/1.1 404 Not Found\r\n";

  *dest += "Server: Lab Web Server\n";
  *dest += "Content-type: text/html\n";
  *dest += "Content-length: 115\n";
  *dest += "\r\n";
  return;
}

struct http_c {
  string mean;
  string URL;
  string version;

  std::map<string, string> kv;

  string body;
};

void show_hc(http_c* hc){
  cout << hc->mean << ' ' << hc->URL << ' ' << hc->version << "\r\n";
  std::map<string, string>::iterator i = hc->kv.begin(), end = hc->kv.end();
  while(i != end) {
    cout << i->first << ' ' << i->second << "\r\n";
    i++;
  }
  cout << hc->body << '\n';
}

void apart(http_c *hc, string *src) {
  int loc;
  string delim = "\r\n";
  this_debug::break_point_1();
  vector<string> table;
  while ((loc = (*src).find(delim)) != string::npos) {
    if (loc != 0) {
      table.push_back((*src).substr(0, loc));
    }
    *src = (*src).substr(loc + delim.size());
  }
  this_debug::break_point_1();
  hc->body = table.back();
  this_debug::break_point_1();
  delim = " ";
  vector<string> request_line;
  
  while ((loc = table[0].find(delim)) != string::npos) {
    if (loc != 0) {
      request_line.push_back(table[0].substr(0, loc));
    }
    *src = table[0].substr(loc + delim.size());
  }

  this_debug::break_point_1();
  hc->mean=request_line[0];
  hc->URL=request_line[1];
  hc->version=request_line[2];
  this_debug::break_point_1();
  int l = table.size()-1;
  this_debug::break_point_1();
  for (int i = 1;i < l;i++) {
    string key,val;
    std::stringstream ss(table[i]);
    ss >> key >> val;
    hc->kv[key] = val;
  }
  this_debug::break_point_1();
  show_hc(hc);
}

void get_respose(string *dest, string *src) { return; }

void make_respose(string *dest, string *src) {
  error_respose(dest,src);
  // http_c hc;
  // this_debug::break_point(3);
  // apart(&hc,src);
  // this_debug::break_point(4);
}

/**
 * @Author : Feng Yangyang
 */

// There is a circle queue.
vector<int> socketFlag;
// The server is one
bool serverOn = true;
extern cpLock *cplock;

void *socket_worker(void *arg) {
  // get the id of the worker
  int id = (long)arg;

  while (1) {
    if (!serverOn) {
      break;
    }
    // FIXME: if the the thread dosen't sleep, the program cannot be stopped.
    sleep(0.1);

    if (socketFlag[id] == 0) {
      continue;
    }
    TransSocket newSocket((SOCKET *)&socketFlag[id]);

    /*
      THE METHOD DURING THE CONNECTION:
        >>> Replace the method below:
    */

    size_t size;
    string recv_buf, send_buf;
    int rc = SE_OK;

    while (!rc) {
      size_t size;
      char tmp[1024];
      memset(tmp, 0, 1024);
      rc = newSocket.Recv(tmp, 1024, TRANS_SOCKET_DFT_TIMEOUT, &size);
      if (!rc) {
        recv_buf += tmp;
      }
    }

    // PD_DEBUG(rc);
    if (!proxy_mode) {
      this_debug::break_point(1);
      make_respose(&send_buf, &recv_buf);
      this_debug::break_point(2);
    } else {
      int c_rc = SE_OK;
      TransSocket clientSock;
      // set the port
      clientSock.setAddress(upstream_url, *upstream_port);

      // init the socket
      clientSock.initSocket();
      // connect
      c_rc = clientSock.Connect();
      assert(c_rc == SE_OK);
      // disable the small package to transport
      //   clientSock.disableNagle();
      clientSock.Send(recv_buf.c_str(), recv_buf.length());

      while (!c_rc) {
        size_t size;
        char tmp[1024];
        memset(tmp, 0, 1024);
        c_rc = clientSock.Recv(tmp, 1024, TRANS_SOCKET_DFT_TIMEOUT, &size);
        if (!c_rc) {
          send_buf += tmp;
        }
      }
      cout << send_buf << '\n';
      clientSock.Close();
    }

    newSocket.Send(send_buf.c_str(), send_buf.length());

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

void *thread_scheduling(void *arg) {
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

  while (1) {
    if (!serverOn) {
      break;
    }
    sleep(0.1);

    int clientSocket;

    // accecept a connect requset
    rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);

    if (rc == SE_OK) {
      cplock->pGet();
      // chose the free pos
      while (socketFlag[lastFitPos] != 0) {
        lastFitPos = (lastFitPos + 1) % *thread_num;
      }
      // insert inti the circle list
      socketFlag[lastFitPos] = clientSocket;
      cplock->pRelease();
    }
  }
  serverSock.Close();
}