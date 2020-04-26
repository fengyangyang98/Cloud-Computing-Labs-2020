#include "global.h"
#include "util.hpp"
#include "sockethandle.h"

// int main(int argc, char *argv[]) {
//   // init
//   GetOptLong(argc, argv);
//   this_debug::show_config();

//   if (NULL != thread_num) {
//     *thread_num = get_nprocs_conf();
//   }
//   /**
//    * TODOLIST:
//    * @Author : Shi Yuanyou
//    * 1. [DONE] create socket and bind it to port(port).
//    * 2. checking proxy_mode. If it's true, then open push Request and pull
//    * Response.
//    * 3. create threads (thread pool, dynamic create or mix up).
//    * 4. Handle HTTP GET/POST request and return an error message if an error
//    * occur. (If needed proxy another request to another HTTP server).
//    */

//   //

//   pthread_t get_socket;
//   pthread_t server;

//   thread_manager();
//   pthread_create(&get_socket, NULL, get_socket_func , NULL);

//   // release ops but in this lab, code will never get here.
//   // need no join.
//   mem_free();
//   return 0;
// }

extern bool serverOn;
extern vector<int> socketFlag;
extern cpLock * cplock;

int main()
{
  /*
    REPLACE THE METHOD BY USING ARG.
  */
  thread_num = new int(8);
  port = new int(12345);
  cplock = new cpLock(8);
  
  pthread_t sheduling;
  pthread_t worker[*thread_num];

  serverOn = true;

  // init the socket flag socket
  for(int i = 0; i < *thread_num; i++) 
  {
    socketFlag.push_back(0);
  }

  // create the woker socket
  for(int i = 0; i < *thread_num; i++) 
  {
    void * id;
    * (int *) &id = i;
    pthread_create(&worker[i], NULL, socket_worker, id);
  }

  // create the socket scheduling thread
  pthread_create(&sheduling, NULL, thread_scheduling, NULL);

  std::string temp;
  while(std::cin >> temp) { 
    sleep(1);
  }

  serverOn = false;

  // wait
  pthread_join(sheduling, NULL);
  for(int i = 0; i < *thread_num; i++) 
  {
    pthread_join(worker[i], NULL);
  }

  mem_free();
}