#include "global.h"
#include "transport.hpp"
#include "util.hpp"
#include "multithread.h"

int main(int argc, char *argv[]) {
  // init
  GetOptLong(argc, argv);
  this_debug::show_config();

  if (NULL != thread_num) {
    *thread_num = get_nprocs_conf();
  }
  /**
   * TODOLIST:
   * @Author : Shi Yuanyou
   * 1. create socket and bind it to port(port).
   * 2. checking proxy_mode. If it's true, then open push Request and pull
   * Response.
   * 3. create threads (thread pool, dynamic create or mix up).
   * 4. Handle HTTP GET/POST request and return an error message if an error
   * occur. (If needed proxy another request to another HTTP server).
   */

  handle();

  // release ops but in this lab, code will never get here.
  // thread_pool.StopAll();
  mem_free();
  return 0;
}