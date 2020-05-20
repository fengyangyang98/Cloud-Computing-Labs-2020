#ifndef NODE_HPP
#define NODE_HPP

#include "include/kv.hpp"
#include "queue"
#include "read_conf.hpp"
#include "thread"

using std::queue;

namespace kv_2pc_sys {

struct Event {
  std::string command_list;
  unsigned long long e_id;
  string ip;
  int port;
};

class Node {
private:
  queue<std::string> command_list;
  KV kvdb;

public:
  Node(){}
  ~Node(){}

  virtual void get_command(){}
  virtual bool parse_command(){}
  virtual void exe(){}

  virtual void pc() = 0;
  virtual void init() = 0;
  virtual void wait() = 0;
  virtual void abort() = 0;
  virtual void commit() = 0;
};

class Coor : public Node {
private:
public:
  Coor(){}
  ~Coor(){}

  virtual void get_command();
  virtual bool parse_command();

  virtual void init();
  virtual void wait();
  virtual void abort();
  virtual void commit();
  virtual void pc();
};

class Part : public Node {
private:
public:
  Part(){}
  ~Part(){}

  virtual void exe();

  virtual void init();
  virtual void wait();
  virtual void abort();
  virtual void commit();
  virtual void pc();
};

}; // namespace kv_2pc_sys

#endif // NODE_HPP