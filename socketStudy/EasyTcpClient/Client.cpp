
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <iostream>

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    //启动windows socket2.x环境
    WSAStartup(ver, &dat);

    //用socketapi建立简易tcp客户端
    //1,建立一个socket套接字
    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == _sock)
    {

        std::cout << "建立套接字失败!!!" << std::endl;
        return 0;
    }
    std::cout << "建立套接字成功!!!" << std::endl;
    //2,连接服务器connect

    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(4567);
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
    if (SOCKET_ERROR == ret)
    {
        std::cout << "连接服务器失败!!!" << std::endl;
        return 0;
    }
    std::cout << "连接服务器成功!!!" << std::endl;
    //3,接收服务器信息recv
    char recvBuf[256] = {};
    //接收的数据长度
    int nLen = recv(_sock, recvBuf,256, 0);
    if (nLen == 0)
    {
        std::cout << "接收服务器信息" << std::endl;
        return 0;
    }
        std::cout << "接收到数据:" << recvBuf << std::endl;
   
    //4,closesocket 关闭套接字
    closesocket(_sock);
    //清除windows socket环境
    WSACleanup();
    getchar();
    return 0;
}
