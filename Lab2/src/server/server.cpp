#include "../global.h"
#include "../transport.hpp"

void * start(void *arg)
{
    char *buf = (char *)malloc(1024);
    memset(buf, 0, 1024);
    int rc;

    printf("%d\n", arg);
    TransSocket newSocket((SOCKET *) &arg);
    size_t size;
    rc = newSocket.Recv(buf, 1024, TRANS_SOCKET_DFT_TIMEOUT, &size);
    printf("%d\n", size);
    printf("%s\n", buf);

    sleep(5);
    std::string str = "nuasfdsag";
    newSocket.Send(str.c_str(), str.length());

    newSocket.Close();
}

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

    pthread_t con;

    while (1)
    {
        int clientSocket;
        // accecept a connect requset
        rc = serverSock.Accept((SOCKET *)&clientSocket, NULL, NULL);
        if (rc == SE_OK)
        {
            void *pData = NULL ;
            *((int *) &pData) = clientSocket;

            pthread_create(&con, NULL,  start, pData);
            pthread_detach(con);
        }
    }

}