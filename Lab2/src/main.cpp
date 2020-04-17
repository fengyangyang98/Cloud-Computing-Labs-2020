#include "global.h"
#include "util.h"
#include "httpParse.hpp"

int main(int argc, char *argv[]) {
  GetOptLong(argc, argv);
  /**
   * TODOLIST:
   * @Author : Shi Yuanyou
   * 1. create socket and bind it to port(port).
   * 2. checking proxy_mode. If it's true, then open push Request and pull Response.
   * 3. create threads (thread pool, dynamic create or mix up).
   * 4. Handle HTTP GET/POST request and return an error message if an error occur.
   * (If needed proxy another request to another HTTP server).
   */



  return 0;
}