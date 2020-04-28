#include "util.hpp"
#include "lock.hpp"

char *ip = nullptr;
int *port = nullptr;
char *upstream_url = nullptr;
int *upstream_port = nullptr;
int *thread_num = nullptr;
cpLock *cplock = nullptr;
bool proxy_mode = false;

// #define debug

void set_upstream(char *str, char *url, int *port) {
  const char *d = ":";
  char *p;
  string message[3];
  int i = 0;
  p = strtok(str, d);
  while (p) {
    message[i++] = string(p);
    p = strtok(NULL, d);
  }
  if (i==2){
    message[0].copy(url,message[0].length());
    *port=atoi(message[1].c_str());
  } else if(i==3) {
    message[0] += ":" + message[1] ;
    message[0].copy(url,message[0].length());
    *port=atoi(message[2].c_str());
  }
}

void mem_free() {
  delete[] ip;
  delete port;
  delete upstream_port;
  delete[] upstream_url;
  delete thread_num;
  delete cplock;
}

namespace this_debug {
void show_config() {
  cout << " --------- \n";
  if (ip) {
    cout << "ip : " << ip << '\n';
  }
  if (port) {
    cout << "port : " << *port << '\n';
  }
  if (upstream_url) {
    cout << "upstream_url : " << upstream_url << '\n';
  }
  if (upstream_port) {
    cout << "upstream_port : " << *upstream_port << '\n';
  }
  if (thread_num) {
    cout << "thread_num : " << *thread_num << '\n';
  }
  cout << " --------- \n";
}
} // namespace this_debug

void GetOptLong(int argc, char *argv[]) {
  int opt;
  int digit_optind = 0; // 设置短参数类型及是否需要参数

  // 如果option_index非空，它指向的变量将记录当前找到参数符合long_opts里的
  // 第几个元素的描述，即是long_opts的下标值
  int option_index = 0;
  // 设置短参数类型及是否需要参数
  const char *optstring = ":ipPn:";
  static struct option long_options[] = {
      {"ip", required_argument, NULL, 'i'},
      {"port", required_argument, NULL, 'p'},
      {"proxy", required_argument, NULL, 'P'},
      {"number-thread", required_argument, NULL, 'n'},
      {0, 0, 0, 0} // 添加 {0, 0, 0, 0} 是为了防止输入空值
  };

  while (true) {
    if (-1 == (opt = getopt_long(argc, argv, optstring, long_options,
                                 &option_index))) {
      break;
    }
    if ('i' == opt) {
      ip = new char[17];
      strcpy(ip, argv[optind - 1]);
      ip[16] = 0;
    } else if ('P' == opt) {
      upstream_url = new char[256];
      memset(upstream_url, 0, 256);
      upstream_port = new int;
      set_upstream(argv[optind - 1], upstream_url, upstream_port);
      proxy_mode = true;
    } else if ('p' == opt) {
      port = new int;
      *port = atoi(argv[optind - 1]);
    } else if ('n' == opt) {
      thread_num = new int;
      *thread_num = atoi(argv[optind - 1]);
    }

#ifdef debug
    printf("opt = %c\n", opt);       // 命令参数
    printf("optarg = %s\n", optarg); // 参数内容
    printf("optind = %d\n", optind); // 下一个被处理的下标值
    printf("argv[optind - 1] = %s\n", argv[optind - 1]); // 参数内容
    printf("option_index = %d\n", option_index); // 当前打印参数的下标值
    printf("\n");
#endif // debug
  }
#ifdef debug
  show_config();
#endif // debug
}