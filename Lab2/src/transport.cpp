#include "global.h"
#include "transport.hpp"

int TransSocket::_getAddress(sockaddr_in * addr, char * pAddress,
                            unsigned int length)
{
    int rc = SE_OK;
    length = length < NI_MAXHOST ? length : NI_MAXHOST;

    rc = getnameinfo((sockaddr*) addr, sizeof(sockaddr), pAddress, length,
                      NULL, 0, NI_NUMERICHOST);

    if(rc) {
        PD_DEBUG_PRINTF("Failed to getnameinfo, rc = %d\n", SOCKET_GETLASTERROR);
    }   

done:
    return rc;
error:
    goto done;
}

unsigned int TransSocket::_getPort(sockaddr_in *addr) 
{
    return ntohs(addr->sin_port);
}


// create a listening socket
TransSocket::TransSocket()
{
    _init = false;
    _fd = 0;
    _timeout = 0;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    // IPv4
    _sockAddress.sin_family = AF_INET; 
}


// create a listening socket
TransSocket::TransSocket(unsigned int port, int timeout)
{
    _init = false;
    _fd = 0;
    _timeout = timeout;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    // IPv4
    _sockAddress.sin_family = AF_INET; 
    // get any port as connecting
    _sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // get host port
    _sockAddress.sin_port = htons(port);
    _addressLen = sizeof(_sockAddress);
}

// create a connection socket
TransSocket::TransSocket(const char *pHostName, unsigned int port,
                         int timeout)
{
    hostent *hp;

    _init = false;
    _fd = 0;
    _timeout = timeout;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    // IPv4
    _sockAddress.sin_family = AF_INET; 
    // get ip
    if ((hp = gethostbyname(pHostName))) {
        _sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0]);
    }
    else {
        _sockAddress.sin_addr.s_addr = inet_addr(pHostName);
    }

    _sockAddress.sin_port = htons(port);
    _addressLen = sizeof(_sockAddress);
}

// create from an existing socket
TransSocket::TransSocket(SOCKET *sock, int timeout)
{
    int rc = SE_OK;

    _fd = *sock;
    _init = true;
    _timeout = timeout;
    _addressLen = sizeof(_sockAddress);
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);

    rc = getsockname(_fd, (sockaddr *)&_sockAddress, &_addressLen);
    if (rc)
    {
        PD_DEBUG_PRINTF("Fialed to get sock name, error = %d\n", SOCKET_GETLASTERROR);
        _init = false;
    }
    else
    {
        rc = getpeername(_fd, (sockaddr *)&_peerAddress, &_peerAddressLen);
        PD_DEBUG_PRINTF("Fialed to get peer name, error = %d\n", SOCKET_GETLASTERROR);
    }

done:
    return;
error:
    goto done;
}

// initialize the socket
int TransSocket::initSocket()
{
    int rc = SE_OK;
    if (_init)
    {
        goto done;
    }
    else
    {
        memset(&_peerAddress, 0, sizeof(sockaddr_in));
        _peerAddressLen = sizeof(_peerAddress);
        _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (_fd == -1)
        {
            PD_DEBUG_PRINTF("Failed to initialize socket, error = %d\n", SOCKET_GETLASTERROR);
        }
        _init = true;
        setTimeout(_timeout);
    }

done:
    return rc;
error:
    goto done;
}

// bind and listen
int TransSocket::bindListen()
{
    int rc = SE_OK;
    int temp = 1;
    
    // reuse
    rc = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&temp, sizeof(int));
    if (rc) {
        PD_DEBUG_PRINTF("Failed to setsockopt SO_REUSEADDR, rc = %d\n", SOCKET_GETLASTERROR);
    }

    // setting
    rc = setSocketLi(1, 30);
    if (rc) {
        PD_DEBUG_PRINTF("Failed to setsockopt SO_LIGERR, rc = %d\n", SOCKET_GETLASTERROR);
    }

    // bind
    rc = ::bind(_fd, (sockaddr *)&_sockAddress, _addressLen);
    if (rc) {
        PD_DEBUG_PRINTF("Failed to bind socket, rc = %d\n", SOCKET_GETLASTERROR);
    }

    // listen
    rc = listen(_fd, SOMAXCONN);
    if (rc) {
        PD_DEBUG_PRINTF( "Failed to listen socket, rc = %d\n", SOCKET_GETLASTERROR);
    }

done:
    return rc;
error:
    Close();
    goto done;
}

int TransSocket::Connect()
{
    int rc = SE_OK;
    rc = :: connect(_fd, (sockaddr*) &_sockAddress, _addressLen);
    if (rc) {
        PD_DEBUG_PRINTF("Fail to connect, rc = %d\n", SOCKET_GETLASTERROR);
    }

    rc = getsockname(_fd, (sockaddr*) &_sockAddress, &_addressLen);
    if(rc) {
        PD_DEBUG_PRINTF("Fail to get local address, rc = %d\n", rc);
    }

    rc = getpeername(_fd, (sockaddr*) &_peerAddress, &_peerAddressLen);
    if(rc) {
        PD_DEBUG_PRINTF( "Fail to get peer address, rc = %d\n", rc);
    }

done:
    return rc;
error:
    goto done;
}

void TransSocket::Close()
{
    if(_init) {
        int i = 0;
        i = ::close(_fd);
        if (i < 0) {
            i = -1;
        }
        _init = false;
    }
}


// return the status of connection
bool TransSocket::isConnected()
{
    int rc = SE_OK;
    rc = ::send(_fd, "", 0, MSG_NOSIGNAL);
    if (rc < 0)
    {
        return false;
    }

    return true;
}

// send massage
int TransSocket::Send(const char *pMsg, int length,
                      int timeout, int flages)
{
    int rc = SE_OK;
    int maxFD = _fd;
    timeval maxSelectTime;
    fd_set fds;

    maxSelectTime.tv_sec = timeout / 1000000;
    maxSelectTime.tv_usec = timeout % 1000000;

    if (length == 0)
    {
        return SE_OK;
    }

    // whether the resources is available
    while (true)
    {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);

        // select the socket avialable
        rc = select((int)(maxFD + 1), NULL, &fds, NULL,
                    timeout >= 0 ? &maxSelectTime : NULL);

        if (rc == 0)
        {
            rc = SE_TIMEOUT;
            goto done;
        }

        if (rc < 0)
        {
            rc = SOCKET_GETLASTERROR;
            if (rc == SE_EINTR)
            {
                continue;
            }
            PD_DEBUG_PRINTF("Failed to select from socket, rc = %d\n", SOCKET_GETLASTERROR);
        }

        if (FD_ISSET(_fd, &fds))
        {
            break;
        }
    }

    // send the massage
    while (length > 0)
    {
        rc = ::send(_fd, pMsg, length, MSG_NOSIGNAL | flages);
        if (rc == -1)
        {
            PD_DEBUG_PRINTF("Failed to send, rc = %d\n", SOCKET_GETLASTERROR);
        }

        length -= rc;
        pMsg += rc;
    }

    rc = SE_OK;

done:
    return rc;
error:
    goto done;
}

// receive the data
int TransSocket::Recv(char *pMsg, int length, size_t * size, int timeout,
                      int flags)
{
    int rc = SE_OK;
    int retries = 0;
    int maxFD = (int)_fd;
    timeval maxSelectTime;
    fd_set fds;

    if (length == 0)
    {
        return SE_OK;
    }

    maxSelectTime.tv_sec = timeout / 1000000;
    maxSelectTime.tv_usec = timeout % 1000000;

    // wether the resources is available
    while (true)
    {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);

        rc = select(maxFD + 1, &fds, NULL, NULL,
                    timeout >= 0 ? &maxSelectTime : NULL);

        if (rc == 0)
        {
            rc = SE_TIMEOUT;
            goto done;
        }

        if (rc < 0)
        {
            rc = SOCKET_GETLASTERROR;
            if (rc == SE_EINTR)
            {
                continue;
            }
            PD_DEBUG_PRINTF("Failed to select from socket, rc = %d\n", SOCKET_GETLASTERROR);
            goto error;
        }

        if (FD_ISSET(_fd, &fds))
        {
            break;
        }
    }

    // receive data
    if(size) * size = 0;
    while (length > 0)
    {
        rc = ::recv(_fd, pMsg, length, MSG_NOSIGNAL | flags);

        if (rc > 0) {
            if (flags & MSG_PEEK) {
                goto done;
            }

            length -= rc;
            pMsg += rc;
            if(size) * size += rc;
        }
        else if (rc == 0) {
            PD_DEBUG_PRINTF("Peer unexpected shutdown\n");
            break;
        }
        else {
            rc = SOCKET_GETLASTERROR;
            if (((rc == SE_EAGAIN) || (rc == EWOULDBLOCK)) && (_timeout > 0)) {
                PD_DEBUG_PRINTF("Revv() timeout, rc = %d\n", rc);
                break;
            }

            if ((rc == SE_EINTR) && (retries < MAX_RECV_RETRIES)) {
                retries++;
                continue;
            }
            PD_DEBUG_PRINTF("Revv() timeout, rc = %d\n", rc);
            break;
        }
    }

    rc = SE_OK;

done:
    return rc;
error:
    goto done;
}

int TransSocket::Recv(char *pMsg, int length, int timeout, size_t * size)
{
    int rc = SE_OK;
    int retries = 0;
    int maxFD = (int)_fd;
    timeval maxSelectTime;
    fd_set fds;

    if (length == 0) {
        return SE_OK;
    }

    maxSelectTime.tv_sec = timeout / 1000000;
    maxSelectTime.tv_usec = timeout % 1000000;

    while (true)
    {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);

        rc = select(maxFD + 1, &fds, NULL, NULL,
                    timeout >= 0 ? &maxSelectTime : NULL);

        if (rc == 0)
        {
            rc = SE_TIMEOUT;
            if(size) *size = rc;
            goto done;
        }

        if (rc < 0)
        {
            rc = SOCKET_GETLASTERROR;
            if (rc == SE_EINTR)
            {
                continue;
            }
            PD_DEBUG_PRINTF("Failed to select from socket, rc = %d\n", SOCKET_GETLASTERROR);
            goto error;
        }

        if (FD_ISSET(_fd, &fds))
        {
            break;
        }
    }

    rc = ::recv(_fd, pMsg, length, MSG_NOSIGNAL);
    if (rc > 0)
    {
        length = rc;
    } else if (rc == 0) {
        PD_DEBUG_PRINTF("Peer unexpected shutdown\n");
        goto done;
    } else {
        rc = SOCKET_GETLASTERROR;
        if (((rc == SE_EAGAIN) || (rc == EWOULDBLOCK)) && (_timeout > 0))
        {
            PD_DEBUG_PRINTF("Revv() timeout, rc = %d\n", rc);
            goto error;
        }

        if ((rc == SE_EINTR) && (retries < MAX_RECV_RETRIES))
        {
            retries++;
        }
        PD_DEBUG_PRINTF("Revv() timeout, rc = %d\n", rc);
        goto error;
    }

    rc = SE_OK;

done:
    return rc;
error:
    goto done;
}

// get one connecting request in the waiting queue, and create a
// new socket
int TransSocket::Accept(SOCKET * sock, sockaddr *addr, 
                       socklen_t * addrlen, int timeout)
{
    int rc = SE_OK;
    int maxFD = (int)_fd;
    timeval maxSelectTime;
    fd_set fds;

    maxSelectTime.tv_sec = timeout / 1000000;
    maxSelectTime.tv_usec = timeout % 1000000;

    while (true)
    {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);

        rc = select(maxFD + 1, &fds, NULL, NULL,
                    timeout >= 0 ? &maxSelectTime : NULL);

        if (rc == 0)
        {
            rc = SE_TIMEOUT;
            goto done;
        }

        if (rc < 0)
        {
            rc = SOCKET_GETLASTERROR;
            if (rc == SE_EINTR)
            {
                continue;
            }
            PD_DEBUG_PRINTF("Failed to select from socket, rc = %d\n", SOCKET_GETLASTERROR);
            goto error;
        }

        if (FD_ISSET(_fd, &fds))
        {
            break;
        }
    }

    rc = SE_OK;
    *sock = ::accept(_fd, addr, addrlen);
    if(*sock == -1) {
        PD_DEBUG_PRINTF("Failed to accept socket, rc = %d\n", SOCKET_GETLASTERROR);
        goto error;
    }

done:
    return rc;
error:
    Close();
    goto done;
}

// If you wanna keep the completeness of each package, call this method.
int TransSocket::disableNagle()
{
    int rc = SE_OK;
    int temp = 1;

    rc = setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, (char*) &temp, sizeof(int));
    if(rc) {
        PD_DEBUG_PRINTF("Failed to setsockopt, rc = %d\n", SOCKET_GETLASTERROR);
    }

    rc = setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, (char*) &temp, sizeof(int));
    if(rc) {
        PD_DEBUG_PRINTF("Failed to setsockopt, rc = %d\n", SOCKET_GETLASTERROR);
    }
    return rc;
}

// define how to close
int TransSocket::setSocketLi(int lOnOff, int linger)
{
    int rc = SE_OK;
    struct linger _linger;

    _linger.l_linger = lOnOff;
    _linger.l_linger = linger;

    rc = setsockopt(_fd, SOL_SOCKET, SO_LINGER,
                    (const char *)&_linger, sizeof(_linger));
    return rc;
}

// set the peer's address
void TransSocket::setAddress(const char *pHostName, unsigned int port)
{
    hostent *hp;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    _sockAddress.sin_family = AF_INET;

    if ((hp = gethostbyname(pHostName))) {
        _sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0]);
    }
    else {
        _sockAddress.sin_addr.s_addr = inet_addr(pHostName);
    }

    _sockAddress.sin_port = htons(port);
    _addressLen = sizeof(_sockAddress);
}


/*
    >>> INFORMATION
*/
unsigned int TransSocket::getLocalPort()
{
    return _getPort(&_sockAddress);
}

unsigned int TransSocket::getPeerPort() 
{
    return _getPort(&_sockAddress);
}

int TransSocket::getLocalAddress(char * pAddress, unsigned int length)
{
    return _getAddress(&_sockAddress, pAddress, length);
}

int TransSocket::getPeerAddress(char * pAddress, unsigned int length)
{
    return _getAddress(&_peerAddress, pAddress, length);
}

int TransSocket::getHostName(char * pName, int nameLen)
{
    return gethostname(pName, nameLen);
}

int TransSocket::getPort(const char * pServiceName, unsigned short & port)
{
    int rc =SE_OK;
    struct servent * servinfo;
    servinfo = getservbyname(pServiceName, "tcp");

    if(!servinfo) {
        port = atoi(pServiceName);
    } else {
        port = (unsigned short) ntohs(servinfo->s_port);
    }

    return rc;
}

int TransSocket::setTimeout(int seconds)
{
    int rc = SE_OK;
    timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    rc = setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(tv));
    if(rc) {
        PD_DEBUG_PRINTF("Failed to setsockopt, rc = %d\n", SOCKET_GETLASTERROR);
    }

    rc = setsockopt(_fd, SOL_SOCKET, SO_SNDTIMEO, (char*) &tv, sizeof(tv));
    if(rc) {
        PD_DEBUG_PRINTF("Failed to setsockopt, rc = %d\n", SOCKET_GETLASTERROR);
    }

    return rc;
}