#include "global.h"
#include "sockethandle.h"
#include "util.hpp"

namespace this_debug {
static unsigned debug_counter = 0;
void break_point() { cout << " ---- DEBUG " << debug_counter++ << " ---- \n"; }
} // namespace // this_debug

extern bool serverOn;
extern vector<int> socketFlag;
extern cpLock *cplock;

int main(int argc, char *argv[]) {
  // init
  GetOptLong(argc, argv);
  // this_debug::show_config();
  if (nullptr == thread_num) {
    thread_num = new int(get_nprocs_conf());
  }
  cplock = new cpLock(*thread_num);

  pthread_t sheduling;
  pthread_t worker[*thread_num];

  serverOn = true;

  // init the socket flag socket
  for (int i = 0; i < *thread_num; i++) {
    socketFlag.push_back(0);
  }

  // this_debug::break_point();

  // create the woker socket
  for (int i = 0; i < *thread_num; i++) {
    void *id;
    *(int *)&id = i;
    pthread_create(&worker[i], NULL, socket_worker, id);
  }

  // create the socket scheduling thread
  pthread_create(&sheduling, NULL, thread_scheduling, NULL);

  // this_debug::break_point();

  string tmp;
  while (std::cin >> tmp) {
    sleep(1);
  }

  pthread_join(sheduling, NULL);
  for (int i = 0; i < *thread_num; i++) {
    pthread_join(worker[i], NULL);
  }

  mem_free();
  return 0;
}