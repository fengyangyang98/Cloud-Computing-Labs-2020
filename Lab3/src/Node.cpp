#include "Node.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <ctype.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace kv_2pc_sys {
void Coor::get_command() { return; }
bool Coor::parse_command() { return true; }

void Coor::init() {
  // Prepare socket
  int server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (server_sockfd == -1) {
    close(server_sockfd);
    perror("socket error!");
  }
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons((uint16_t)coordinator[0].port);
  inet_pton(AF_INET, coordinator[0].ip.c_str(), &server_addr.sin_addr.s_addr);

  if (bind(server_sockfd, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    close(server_sockfd);
    perror("bind error");
  }

  if (listen(server_sockfd, participant.size()))
    ;
}
void Coor::wait() { return; }
void Coor::abort() { return; }
void Coor::commit() { return; }
void Coor::pc() { return; }
} // namespace kv_2pc_sys

namespace kv_2pc_sys {
void Part::exe() { return; }

void Part::init() { return; }
void Part::wait() { return; }
void Part::abort() { return; }
void Part::commit() { return; }
void Part::pc() { return; }
} // namespace kv_2pc_sys