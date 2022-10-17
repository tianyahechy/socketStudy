
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

    while (true)
    {
        //3输入请求命令
        char cmdBuf[128] = {};
        scanf("%s", cmdBuf);
        //4,处理请求命令
        if ( 0 == strcmp(cmdBuf, "exit"))
        {
            std::cout << "收到exit命令，任务结束" << std::endl;
            break;
        }
        //5,向服务器发送请求命令
        send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
        //6,接收服务器信息recv
        char recvBuf[128] = {};
        //接收的数据长度
        int nLen = recv(_sock, recvBuf,128, 0);
        if (nLen == 0)
        {
            std::cout << "接收服务器信息失败!!!" << std::endl;
            break;
        }
        std::cout << "接收到数据:" << recvBuf << std::endl;
    }
    //4,closesocket 关闭套接字
    closesocket(_sock);
    //清除windows socket环境
    WSACleanup();
    std::cout << "退出，任务结束" << std::endl;
    getchar();
    return 0;
}
