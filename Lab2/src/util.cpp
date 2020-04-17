#include "util.h"

char *ip = nullptr;
int *port = nullptr;
char *upstream_url = nullptr;
int *upstream_port = nullptr;
int *thread_num = nullptr;
bool proxy_mode = false;

#define debug

int get_port(char *url) {
  int ret = 0;
  int i = 0;
  bool f = false;
  for (i = 0; *(url + i) != '\0'; ++i) {
    if (':' == *(url + i)) {
      f = true;
      break;
    }
  }
  if (f) {
    while (*(url + i) != '\0') {
      ret *= 10;
      ret += (int)(*(url + i) - '0');
      ++i;
    }
  } else {
    ret = 80;
  }
  return ret;
}

void show_config() {
  cout << *ip << '\n';
  cout << *port << '\n';
  cout << *upstream_url << '\n';
  cout << *upstream_port << '\n';
  cout << *thread_num << '\n';
}

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
      {"port", required_argument, port, 'p'},
      {"proxy", required_argument, NULL, 'P'},
      {"number-thread", required_argument, thread_num, 'n'},
      {0, 0, 0, 0} // 添加 {0, 0, 0, 0} 是为了防止输入空值
  };

  while (true) {
    if (-1 == (opt = getopt_long(argc, argv, optstring, long_options,
                                 &option_index))) {
      break;
    }
    if (0 == strcmp(optarg, "ip")) {
      ip = argv[optind - 1];
    }
    if (0 == strcmp(optarg, "proxy")) {
      upstream_url = argv[optind - 1];
      proxy_mode = true;
      *upstream_port = get_port(upstream_url);
    }
#ifdef debug
    printf("opt = %c\n", opt);       // 命令参数，亦即 -a -b -n -r
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