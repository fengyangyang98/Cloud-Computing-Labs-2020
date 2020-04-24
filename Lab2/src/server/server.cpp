#include "../global.h"
#include "../transport.hpp"

int main()
{
    int rc = SE_OK;
    // set the port
    TransSocket serverSock(12345);

    // init the socket
    rc = serverSock.initSocket();
    PD_DEBUG(rc);

    // listen the port
    rc = serverSock.bindListen();
    PD_DEBUG(rc);

    char *buf = (char *)malloc(1024);
    memset(buf, 0, 1024);

    while (1)
    {
        int clientSocket;
        // accecept a connect requset
        rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);
        if(rc == SE_OK) {
            printf("%d\n",clientSocket);
            TransSocket newSocket(&clientSocket);
            size_t size;
            rc = newSocket.Recv(buf, 1024, TRANS_SOCKET_DFT_TIMEOUT, &size);
            printf("%d\n", size);
            printf("%s\n", buf);
            break;
            
            newSocket.Close();
        }
    }

    free(buf);
}