#include "sockethandle.h"
#include "fileOp.hpp"
#include "sstream"
#include "util.hpp"
#include <regex>

namespace this_debug {
static unsigned debug_counter_1 = 0;
void break_point_1() {
  cout << " ---- default " << debug_counter_1++ << " ---- \n";
}
void break_point(int i) { cout << " ---- HANDLE " << i << " ---- \n"; }
} // namespace this_debug

using std::regex;

queue<int> client_socket_queue;
exclusiveLock client_socket_lock;
cpLock queueLock;

struct http_c {
  string mean;
  string URL;
  string version;

  std::map<string, string> kv;

  string body;
};

struct http_send {
  string version;
  string state;
  string chat;

  std::map<string, string> kv;

  string body;
};

void create_text(string *dest, http_send *hs) {
  // state line
  *dest = hs->version + ' ' + hs->state + ' ' + hs->chat + "\r\n";
  std::map<string, string>::iterator i = hs->kv.begin(), end = hs->kv.end();
  while (i != end) {
    *dest += i->first + ": " + i->second + "\r\n";
    ++i;
  }
  *dest += "\r\n";
  *dest += hs->body + "\n";
}

void show_hc(http_c *hc) {
  cout << hc->mean << ' ' << hc->URL << ' ' << hc->version << "\r\n";
  std::map<string, string>::iterator i = hc->kv.begin(), end = hc->kv.end();
  while (i != end) {
    cout << i->first << ' ' << i->second << "\r\n";
    i++;
  }
  cout << hc->body << '\n';
}

void show_hs(http_send *hs) {
  cout << " ----- \n";
  cout << hs->version << ' ' << hs->state << ' ' << hs->chat << "\r\n";
  std::map<string, string>::iterator i = hs->kv.begin(), end = hs->kv.end();
  while (i != end) {
    cout << i->first << ' ' << i->second << "\r\n";
    i++;
  }
  cout << hs->body << '\n';
  cout << "----- \n";
}

void apart(http_c *hc, string *src) {
  int loc;
  string delim = "\r\n", tmp = *src + "\r\n";
  vector<string> table;
  while ((loc = tmp.find(delim)) != string::npos) {
    if (loc != 0) {
      table.push_back(tmp.substr(0, loc));
    }
    tmp = tmp.substr(loc + delim.size());
  }

  delim = " ";
  vector<string> request_line;
  int loc2;
  tmp = table[0] + " ";
  while ((loc2 = tmp.find(delim)) != string::npos) {
    if (loc2 != 0) {
      request_line.push_back(tmp.substr(0, loc2));
    }
    tmp = tmp.substr(loc2 + delim.size());
  }

  hc->mean = request_line[0];
  hc->URL = request_line[1];
  hc->version = request_line[2];

  int l = table.size() - 2;

  for (int i = 1; i < l; i++) {
    string key, val;
    std::stringstream ss(table[i]);
    ss >> key >> val;
    hc->kv[key] = val;
  }

  hc->body = table[l + 1];
  cout << " ---- show hc ---- \n";
  show_hc(hc);
  cout << " ----  ---- \n";
}

void no_found_respose(string *dest) {
  *dest = "HTTP/1.1 404 Not Found\r\n";
  *dest += "Server: Lab Web Server\n";
  *dest += "Content-type: text/html\n";
  *dest += "Content-length: ";
  fileOp fop;
  fop.Open("./404.html", S_FILE_OP_READ_ONLY);
  size_t size = fop.getSize();
  std::stringstream ss;
  ss << size;
  string tmp;
  ss >> tmp;
  *dest += tmp;
  *dest += "\n\r\n";

  char buf[size + 1];
  buf[size - 1] = '\0';
  buf[size] = '\0';
  fop.readAt(0, size, buf, NULL);
  *dest += buf;
  fop.Close();
  return;
}

bool find_name(string src, string *name, string *ID) {
  cout << " ---- name ID --- \n";
  if ((src.find("Name=") == -1) || (src.find("&ID=") == -1)) {

    cout << src.find("Name=") << '\n' << src.find("&ID=") << '\n';
    return false;
  }
  this_debug::break_point(222);
  int loc;
  string delim = "&";
  vector<string> table;
  src += "&";
  while ((loc = src.find(delim)) != string::npos) {
    if (loc != 0) {
      table.push_back(src.substr(0, loc));
    }
    src = src.substr(loc + delim.size());
  }
  cout << table.size();
  this_debug::break_point(333);
  string name_str = table[0], id_str = table[1];
  this_debug::break_point(444);
  delim = "=";
  while ((loc = name_str.find(delim)) != string::npos) {
    name_str = name_str.substr(loc + delim.size());
  }
  *name = name_str;
  this_debug::break_point(555);
  delim = "=";
  while ((loc = id_str.find(delim)) != string::npos) {
    id_str = id_str.substr(loc + delim.size());
  }
  *ID = id_str;
  this_debug::break_point(666);
  cout << "Name :" << *name << "ID :" << *ID << '\n';

  return true;
}

void post_respose(string *dest, http_c *hc, string *name, string *ID) {
  http_send hs;
  hs.version = "HTTP/1.1";
  hs.state = "200";
  hs.chat = "OK";
  hs.kv["Server"] = "Guo's Web Server";
  hs.kv["Content-type"] = "text/html";
  hs.kv["Connection"] = "close";
  string text = "<html><title>Post method</title><body bgcolor=ffffff>\n";
  text += "Your Name: " + *name + "\n";
  text += "ID: " + *ID + "\n";
  text += "<hr><em>Http Web server</em>\n</body></html>\n";
  std::stringstream ss;
  ss << text.length();
  ss >> hs.kv["Content-length"];
  hs.body = text;
  create_text(dest, &hs);
  return;
}

void get_respose(string *dest, http_c *hc) {
  http_send hs;
  hs.version = "HTTP/1.1";
  string file_url;
  if (hc->URL == "/") {
    file_url = "./index.html";
  } else if (hc->URL.find(".") == -1) {
    file_url = "." + hc->URL + "/index.html";
  } else {
    file_url = "." + hc->URL;
  }

  fileOp fop;
  fop.Open(file_url.c_str(), S_FILE_OP_READ_ONLY);
  if (fop.isValid()) {
    size_t size = fop.getSize();
    hs.state = "200";
    hs.chat = "OK";
    hs.kv["Server"] = "Guo's Web Server";
    std::stringstream ss;
    ss << size;
    ss >> hs.kv["Content-length"];
    if (hc->URL.find(".html") != -1) {
      hs.kv["Content-type"] = "text/html";
    } else if (hc->URL.find(".html") != -1) {
      hs.kv["Content-type"] = "application/x-jpg";
    } else if (hc->URL.find(".html") != -1) {
      hs.kv["Content-type"] = "text/css";
    } else if (hc->URL.find(".html") != -1) {
      hs.kv["Content-type"] = "application/x-javascript";
    }
    hs.kv["Connection"] = "close";
    
    char tmp_buf[size + 1];
    tmp_buf[size] = '\0';
    fop.readAt(0, size, tmp_buf, NULL);
    hs.body = string(tmp_buf);
  } else {
    hs.state = "404";
    hs.chat = "Not Found";
    string text = "<html><title>404 Not Found</title><body bgcolor=ffffff>\n";
    text += " Not Found\n<p>Couldn't find this file: ";
    text += file_url + "\n<hr><em>HTTP Web server</em>\n</body></html>";
    hs.kv["Server"] = "Guo's Web Server";
    std::stringstream ss;
    ss << text.length();
    ss >> hs.kv["Content-length"];
    hs.kv["Connection"] = "close";
    hs.kv["Content-type"] = "text/html";
    hs.body = text;
  }
  create_text(dest, &hs);
  show_hs(&hs);
  return;
}

void not_implemented(string *dest, http_c *hc) {
  http_send hs;
  hs.version = "HTTP/1.1";
  hs.state = "501";
  hs.chat = "Not Implemented";
  string text = "<html><title>404 Not Found</title><body bgcolor=ffffff>\n";
  text += " Not Implemented\n<p>Does not implement this method: ";
  text += hc->mean + "\n<hr><em>HTTP Web server</em>\n</body></html>";
  hs.kv["Server"] = "Guo's Web Server";
  std::stringstream ss;
  ss << text.length();
  ss >> hs.kv["Content-length"];
  hs.kv["Content-type"] = "text/html";
  hs.body = text + "\n";
  create_text(dest, &hs);
}

void make_respose(string *dest, string *src) {
  this_debug::break_point(1);
  http_c hc;
  this_debug::break_point_1();
  cout << *src << "\n";
  if (src->empty()) {
    cout << " it is empty ! \n";
  }
  apart(&hc, src);
  this_debug::break_point_1();
  this_debug::break_point(2);
  if (hc.mean == "GET") {
    this_debug::break_point(3);
    get_respose(dest, &hc);
    this_debug::break_point(3);
  } else if (hc.mean == "POST") {
    string name, ID;
    if (hc.URL == "/Post_show" && find_name(hc.body, &name, &ID)) {
      this_debug::break_point(4);
      post_respose(dest, &hc, &name, &ID);
      this_debug::break_point(4);
    } else {
      this_debug::break_point(5);
      no_found_respose(dest);
      this_debug::break_point(5);
    }
  } else {
    this_debug::break_point(6);
    not_implemented(dest, &hc);
    this_debug::break_point(6);
  }
  this_debug::break_point(1);

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
      rc = newSocket.Recv(tmp, 1, 100, NULL, MSG_PEEK);
      if(rc) break;
    }

    // PD_DEBUG(rc);
    if (!proxy_mode) {
      // this_debug::break_point(1);
      if (!recv_buf.empty())
        make_respose(&send_buf, &recv_buf);
      // this_debug::break_point(2);
    } else {
      int c_rc = SE_OK;
      TransSocket clientSock;
      // set the port
      clientSock.setAddress(upstream_url, *upstream_port);

      // init the socket
      clientSock.initSocket();
      // connect
      c_rc = clientSock.Connect();
      if(c_rc != SE_OK)
        goto done;

      c_rc = clientSock.Send(recv_buf.c_str(), recv_buf.length());
      if(c_rc != SE_OK)
        goto done;

      /*
        1. Get the Packet
        2. Using the short timeout to check the packet in the buf
      */

      while (!c_rc) {
        size_t size;
        char tmp[1024];
        memset(tmp, 0, 1024);
        c_rc = clientSock.Recv(tmp, 1024, TRANS_SOCKET_DFT_TIMEOUT, &size);

        if (!c_rc) {
          send_buf += tmp;
        }

        c_rc = clientSock.Recv(tmp, 1, 100, NULL, MSG_PEEK);
        if(c_rc) break;
      }

      cout << send_buf << '\n';
      clientSock.Close();
    }

    newSocket.Send(send_buf.c_str(), send_buf.length());

    /*
      THE METHOD DURING THE CONNECTION
        >>> Replace the method above:
    */

done:
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
  TransSocket serverSock(myPort, ip);

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