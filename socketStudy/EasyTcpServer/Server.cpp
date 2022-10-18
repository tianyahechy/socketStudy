
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <iostream>

struct DataPackage
{
    int age;
    char name[32];
};

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    //启动windows socket2.x环境
    WSAStartup(ver, &dat);

    //用socketapi建立简易tcp服务端
    //1,建立一个socket套接字
    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == _sock)
    {

        std::cout << "建立套接字失败!!!" << std::endl;
        return 0;
    }
    std::cout << "建立套接字成功!!!" << std::endl;
    //2,bind绑定用于接受客户端连接的网络端口
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(4567);
    _sin.sin_addr.S_un.S_addr = INADDR_ANY;
    int ret = bind(_sock, (sockaddr*) &_sin, sizeof(_sin));
    if (SOCKET_ERROR == ret)
    {
        std::cout << "绑定网络端口失败!!!" << std::endl;
        return 0;
    }
    std::cout << "绑定网络端口成功!!!" << std::endl;
    //3,listen监听网络端口
    ret = listen(_sock, 5);
    if (SOCKET_ERROR == ret)
    {
        std::cout << "监听网络端口失败!!!" << std::endl;
        return 0;
    }
    std::cout << "监听网络端口成功" << std::endl;
    //4,等待接受客户端连接
    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    SOCKET _cSock = accept(_sock, (sockaddr*)&clientAddr,&nAddrLen);
    if (INVALID_SOCKET == _cSock)
    {
        std::cout << "接受到无效客户端连接!!!" << std::endl;
        return 0;

    }
    std::cout << "新客户端加入: socket =" << _cSock <<" ,IP = " << inet_ntoa(clientAddr.sin_addr) << std::endl;
    char _recvBuf[128] = {};
    while (true)
    {
        //5.接收客户端数据
        int nLen = recv(_cSock, _recvBuf, 128, 0);
        if (nLen <= 0)
        {
            std::cout << "客户端已经退出，任务结束" << std::endl;
            break;
        }
        std::cout << "收到命令:" << _recvBuf << std::endl;
        //6,处理请求
        if (0 == strcmp(_recvBuf, "getInfo"))
        {
            //7，send 向客户端发送一条数据
            DataPackage info = { 40,"feige" };
            send(_cSock, (const char*)&info, sizeof(DataPackage), 0);
        }
        else
        {
            //7，send 向客户端发送一条数据
            char msgBuf[] = "???";
            send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
        }
    }
    //6,closesocket 关闭套接字
    closesocket(_sock);
    //清除windows socket环境
    WSACleanup();
    std::cout << "服务端已经退出，任务结束" << std::endl;
    getchar();
    return 0;
}
