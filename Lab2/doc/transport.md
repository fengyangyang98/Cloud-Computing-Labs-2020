# Dev doc of transport

## 客户端

1. 建立一个套接字
```
TransSocket(const char *pHostName, 
            unsigned int port, 
            int timeout=0);
```
* pHostName: 可以为 IPv4 IP，或者 url，或者 localhost
* port: 连接端口号
* \* timeout: 最大延时（选填）

2. 初始化套接字 & 建立连接
```
int initSocket();
int Connect();
```

3. 设置传输方式和关闭方式
```
int disableNagle();
```
TCP 协议会将一段信息切分成小包传输，为了避免这种切分，可以调用此函数。
```
int setSocketLi(int lOnOff, int linger);
```
设置套接字关闭的方式，具体设置参数，可以参见 *linger*。

4. 传输信息
```
int Send(const char * pMsg, 
        int length,
        int timeout=TRANS_SOCKET_DFT_TIMEOUT,
        int flag=0);
```
* pMsg: 需要传输的信息
* length: 传输信息的长度
* \*timeout: 最大延时设置
* \*flag: 传输的一些标志为，可以参见 socket::send 的文档。

5. 接收信息
```
    int Recv(char * pMsg, 
             int length, 
             int timeout=TRANS_SOCKET_DFT_TIMEOUT,
             int flag=0);
    int Recv(char * pMsg, 
             int length, 
             int timeout);
```
* pMsg: 需要传输的信息
* length: 传输信息的长度
* \*timeout: 最大延时设置
* \*flag: 传输的一些标志为，可以参见 socket::send 的文档。

如果不设置延时（第一个 Recv），包会被分割传输，如果设置延时（第二个 Recv），包会强制一次性传输。建议使用第一种。

## 服务端

1. 建立一个监听套接字
```
TransSocket(unsigned int port, int timeout=0);
```
* port: 监听端口
* \*timeout: 最大延时设置

2. 初始化套接字 & 开始监听
```
int initSocket();
int bindListen();
```

3. 从连接请求中拿取一个请求套接字
```
int Accept(SOCKET * sock, 
           sockaddr * addr, 
           socklen_t * addrlen,
           int timeout=TRANS_SOCKET_DFT_TIMEOUT);
```
* sock: （返回）连接的套接字请求
* addr: （返回）地址
* addrlen: （返回）地址长度
* \*timeout: 最大延时设置

获得 socket 之后，请调用以下方法，来获得一个客户端套接字请求封装，然后使用 `Recv` 和 `Send` 来与客户端进行数据传输。

```
TransSocket(SOCKET *sock, int timeout=0);
```
* sock: `Accept` 中返回的 socket
* \*timeout: 最大延时设置

## 其他

`get` 类的方法可以获得诸如端口、IPv4 地址等信息。

```
int setTimeout(int seconds);
```
可以设置默认的延时时间。

## 编程建议

服务器建议使用一个线程来接收连接，每次接收到一个连接请求之后，建立新的线程，或者使用线程池调用 `TransSocket(SOCKET *sock, int timeout=0);` 来与客户端进行数据交互。

一个 socket 建议只发送一次、接收一次数据包，因为数据的接收与发送需要确定包的大小。不定长的包，建议在包头指定包的大小。

socket 可以重用，每次需要先调用 `Close`，再重新设置地址，重新初始化以及连接。