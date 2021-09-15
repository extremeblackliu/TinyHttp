#include "HTTP.h"
#include <WinSock.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

HttpRequest::HttpRequest(const std::string& ip, int port) : m_ip(ip), m_port(port)
{
}

std::string HttpRequest::HttpGet(std::string req)
{
    std::string ret = "";
    try
    {
        WSADATA wData;
        ::WSAStartup(MAKEWORD(2, 2), &wData);

        SOCKET clientSocket = socket(AF_INET, 1, 0);
        struct sockaddr_in ServerAddr = { 0 };
        ServerAddr.sin_addr.s_addr = inet_addr(m_ip.c_str());
        ServerAddr.sin_port = htons(m_port);
        ServerAddr.sin_family = AF_INET;
        int errNo = connect(clientSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
        if (errNo == 0)
        {
            //  "GET /[req] HTTP/1.1\r\n"  
            //  "Accept-Encoding:gzip, deflate\r\n"  
            //  "Accept-Language:zh-CN,en,*\r\n"
              
            std::string strSend = " HTTP/1.1\r\n"
                "Host: IP/Domain Here\r\n"
                "\r\n\r\n";;
            strSend = "GET " + req + strSend;

            errNo = send(clientSocket, strSend.c_str(), strSend.length(), 0);
            if (errNo > 0)
            {
                std::cout << "sent" << std::endl;
            }
            else
            {
                std::cout << "errNo:" << errNo << std::endl;
                return ret;
            }

            char bufRecv[3069] = { 0 };
            errNo = recv(clientSocket, bufRecv, 3069, 0);
            if (errNo > 0)
            {
                ret = bufRecv;
                ret += strSend.c_str();
            }
            else
            {
                std::cout << "errNo:" << errNo << std::endl;
                return ret;
            }
        }
        else
        {
            errNo = WSAGetLastError();
            std::cout << "errNo:" << errNo << std::endl;
        }
        ::WSACleanup();
    }
    catch (...)
    {
        return "";
    }
    return ret;
}
