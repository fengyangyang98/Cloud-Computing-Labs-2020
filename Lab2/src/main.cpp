#include "global.h"
#include "sockethandle.h"
#include "util.hpp"

extern bool serverOn;
extern vector<int> socketFlag;
extern cpLock *cplock;

int main(int argc, char *argv[]) {
  // init
  GetOptLong(argc, argv);
  this_debug::show_config();

  if (NULL != thread_num) {
    *thread_num = get_nprocs_conf();
  }

  cplock = new cpLock(8);

  pthread_t sheduling;
  pthread_t worker[*thread_num];

  serverOn = true;

  // init the socket flag socket
  for (int i = 0; i < *thread_num; i++) {
    socketFlag.push_back(0);
  }

  // create the woker socket
  for (int i = 0; i < *thread_num; i++) {
    void *id;
    *(int *)&id = i;
    pthread_create(&worker[i], NULL, socket_worker, id);
  }

  // create the socket scheduling thread
  pthread_create(&sheduling, NULL, thread_scheduling, NULL);

  mem_free();
  return 0;
}