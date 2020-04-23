#include "global.h"
#include "httpParse.hpp"
#include "threadpool.hpp"
#include "util.hpp"

using std::endl;

void fun(int x) {
  cout << "task " << x << " working in thread "
            << std::this_thread::get_id() << endl;
  cout << "task " << x << " working in thread "
            << std::this_thread::get_id() << endl;
  cout << "task " << x << " working in thread "
            << std::this_thread::get_id() << endl;
  cout << "task " << x << " working in thread "
            << std::this_thread::get_id() << endl;
  cout << "---------------------------------------------\n";
}

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

  ThreadPool pool(*thread_num);
  vector<future<int> > results;

  for (int i = 0; i < 8; ++i) {
    results.emplace_back(pool.enqueue([i] {
      cout << "hello " << i << endl;
      std::this_thread::sleep_for(chrono::seconds(1));
      cout << "world " << i << endl;
      return i * i;
    }));
  }

  for (auto &&result : results) cout << result.get() << ' ';
  cout << endl;


  mem_free();
  return 0;
}