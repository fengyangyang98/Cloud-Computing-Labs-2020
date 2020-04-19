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

    char *buf = (char *)malloc(100);
    memset(buf, 0, 100);

    while (1)
    {
        int clientSocket;
        // accecept a connect requset
        rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);
        if(rc == SE_OK) {
            printf("%d\n",clientSocket);
            TransSocket newSocket(&clientSocket);
            while(1) {
                rc = newSocket.Recv(buf, 5);
                if(!rc) {
                    printf("%s\n", buf);
                    break;
                }
            }
            newSocket.Close();
            break;
        }
        // sleep(0.5);
    }
    free(buf);
}