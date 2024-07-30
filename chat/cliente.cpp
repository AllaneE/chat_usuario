#define _WIN32_WINNT 0x0501
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

#include <ws2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

// Link com a biblioteca ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

std::atomic<bool> running(true);

void receberMensagens(SOCKET ConnectSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int iResult;

    while (running) {
        // Receber dados do servidor
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';  // Null-terminate the received string
            printf("%s\n", recvbuf);
        }
        else if (iResult == 0) {
            printf("Conexão fechada pelo servidor\n");
            running = false;
        }
        else {
            printf("Falha na recepção: %d\n", WSAGetLastError());
            running = false;
        }
    }
}
int enviarMensagens(SOCKET ConnectSocket){
    int iResult;
    std::string msg;

    std::cout << "Mensagem: ";
    std::getline(std::cin, msg);
    if(msg=="/exit"){
        closesocket(ConnectSocket);
    }
    const char *sendbuf = msg.c_str();

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("Falha no envio: %d\n", WSAGetLastError());
        running = false;
        return 1;
    }
    return 0;
}
int main(int argc, char *argv[]) {
    WSADATA wsaData;
    int iResult;
    

    // Inicializando o Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup falhou: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;  // Use AF_INET para IPv4 ou AF_INET6 para IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolvendo o endereço e a porta do servidor
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo falhou: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Tentando conectar ao primeiro endereço retornado pela chamada ao getaddrinfo
    ptr = result;

    // Criando um SOCKET para conectar ao servidor
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Erro no socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Conectando ao servidor
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Não foi possível conectar ao servidor!\n");
        WSACleanup();
        return 1;
    }

    // Iniciar thread para receber mensagens
    std::thread receiver(receberMensagens, ConnectSocket);

    // Enviar mensagens ao servidor
    do {
        enviarMensagens(ConnectSocket);
    } while (running);

    // Limpeza
    receiver.join();
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
