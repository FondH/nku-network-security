#include "socket.h"
#define ipaddr "127.0.0.1"   //192.168.137.1
#define port 8999

SOCKET listen_socket;
SOCKET remote_sock;
void StartClient() {
    remote_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (remote_sock == INVALID_SOCKET) {
        printf("INVALID scoket");
    }
    sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8999);
    inet_pton(AF_INET, ipaddr, &sock_addr.sin_addr);
    while (true) {
        // ����
        if (connect(remote_sock, (sockaddr*)&sock_addr, sizeof(sock_addr)) != SOCKET_ERROR)
            break;

    }
    std::cout << "StartClient connected";
}

void StartServer() {

    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(8999);
    inet_pton(AF_INET, ipaddr, &sock_addr.sin_addr);
    //sock_addr.sin_addr.S_un.S_addr = inet_addr("192.168.137.1");

    if (bind(listen_socket, (sockaddr*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR) {
        printf("Server Bind defeat��");
        closesocket(listen_socket);
    }

    int errorCode = WSAGetLastError();
    if (errorCode != 0) {
        // ��ȡ������Ϣ
        LPSTR errorMessage = nullptr;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorMessage, 0, nullptr);
        if (errorMessage != nullptr) {
            // ���������Ϣ
            std::cout << "WSA error code: " << errorCode << std::endl;
            std::cout << "WSA error message: " << errorMessage << std::endl;
            LocalFree(errorMessage);
        }
        else {
            std::cout << "Failed to get error message for WSA error code: " << errorCode << std::endl;
        }
    }

    if (listen(listen_socket, 5) == SOCKET_ERROR)
    {
        printf("listen Server Error !");
        return;
    }
    std::cout << "StartServer Listen..." << std::endl;

    sockaddr_in remote_addr;
    int lenOfaddr = sizeof(remote_addr);
    remote_sock = accept(listen_socket, (SOCKADDR*)&remote_addr, &lenOfaddr);

     if (remote_sock == INVALID_SOCKET) {
         std::cout << "listen Error\n";
         return;
     }

     std::cout << "StartServer connected";
}

void _send(std::string a){

    send(remote_sock, a.c_str(), 255, 0);
    return;
}

void RecvFromRemote(int id) {
    char* recvBuffer = new char[256];
    while (FLag_Recv) {
        //type:user:string:time
        int recvbytes = recv(remote_sock, recvBuffer, 255, 0);

        if (recvbytes > 1) 
            appendMess(std::string(recvBuffer));
    }
    std::cout << "Recv exit\n";
    delete []recvBuffer;
}

void StartServ(bool mode) {
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &wsaData) != 0) {
        std::cout << L"WSAStartup failed with error code " << WSAGetLastError();
            //<< L": " << errorMessage << std::endl;
        return;
    }

    if (mode)
        StartClient();
    else
        StartServer();

    //std::thread t = std::thread(_send, 0);
    //t0.join();
    //WSACleanup();
    return ;
}

