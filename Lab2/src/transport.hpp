#ifndef TRANSPORT_HPP_
#define TRANSPORT_HPP_

#include "global.h"

#define SOCKET_GETLASTERROR         errno
#define TRANS_MAX_HOSTNAME          NI_MAXHOST
#define TRANS_MAX_SERVICENAME       NI_MAXSERV
#define TRANS_SOCKET_DFT_TIMEOUT    100000000
#define TRANS_EAGAIN	            EINPROGRESS
#define TRANS_EINTR	                EINTR
#define MAX_RECV_RETRIES            5

typedef int SOCKET;

class TransSocket 
{
private:

    SOCKET _fd;
    socklen_t _addressLen;
    socklen_t _peerAddressLen;
    struct sockaddr_in _sockAddress;
    struct sockaddr_in _peerAddress;
    bool _init;
    int _timeout;

protected:
    unsigned int _getPort(sockaddr_in * addr);
    int _getAddress(sockaddr_in * addr, char * pAddress,
                   unsigned int length);
public:
    // create a listening socket
    TransSocket();

    // create a listening socket
    TransSocket(unsigned int port, int timeout=0, const char *pHostName = nullptr);
    
    // create a connection socket
    TransSocket(const char *pHostName, unsigned int port, 
               int timeout=0);

    // create from an existing socket
    TransSocket(SOCKET *sock, int timeout=0);

    ~TransSocket()
    {
        Close();
    }

    // init a socket
    int initSocket();

    // bind and listen
    int bindListen();

    // connect 
    int Connect();

    // close the socket
    void Close();

    // return the status of connection
    bool isConnected();

    // send massage
    int Send(const char * pMsg, int length,
             int timeout=TRANS_SOCKET_DFT_TIMEOUT,
             int flag=0);

    // receive the data
    int Recv(char * pMsg, int length, 
             size_t * size = nullptr,
             int timeout=TRANS_SOCKET_DFT_TIMEOUT,
             int flag=0);
    int Recv(char * pMsg, int length, int timeout, size_t * size = nullptr, int flag = 0);

    // get one connecting request in the waiting queue, and create a
    // new socket
    int Accept(SOCKET * sock, sockaddr * addr, 
               socklen_t * addrlen,
               int timeout=TRANS_SOCKET_DFT_TIMEOUT);
    
    // diable the small segment send
    int disableNagle();

    // define how to close
    int setSocketLi(int lOnOff, int linger);

    // set the peer's address
    void setAddress(const char * pHostName, unsigned int port);

    // get the peer info
    unsigned int getPeerPort();
    int getPeerAddress(char * pAddress, unsigned int length);
    
    // get the local info
    unsigned int getLocalPort();
    int getLocalAddress(char * pAddress, unsigned int length);

    static int getHostName(char * pName, int nameLen);
    static int getPort(const char * pServiceName, unsigned short & port);

    int setTimeout(int seconds);
} ;


typedef class TransSocket Sock;



#endif