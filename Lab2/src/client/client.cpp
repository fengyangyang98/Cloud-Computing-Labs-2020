#include "../global.h"
#include "../transport.hpp"

int main()
{
    int rc = SE_OK;
    TransSocket clientSock;
    // set the port 
    clientSock.setAddress("localhost", 12345);
    // init the socket
    clientSock.initSocket();
    // connect
    rc = clientSock.Connect();
    assert(rc == SE_OK);
    // disable the small package to transport
    clientSock.disableNagle();

    std::string str;
    while(clientSock.isConnected() && std::cin >> str) {
        clientSock.Send(str.c_str(), str.length());
    }
}