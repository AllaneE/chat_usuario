/* Etapas de criar o servidor 
Inicializar Winsock.
Crie um soquete.
Associe o soquete.
Ouça no soquete de um cliente.
Aceite uma conexão de um cliente.
Receber e enviar dados.
Desconectar.*/

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define WIN32_WINNT 0x0501

#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"


int main(){

    WSADATA wsadata;
    int iResultado;

    struct addrinfo *resultado = NULL, *ptr = NULL, hints;
    SOCKET ListenSocket = INVALID_SOCKET;

    // inicializando winsock
    iResultado = WSAStartup(MAKEWORD(2,2), &wsadata);
    if( iResultado != 0){
        printf("WSAStartup falhou: %d\n", iResultado);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    //local e port que o servidor vai usar

    iResultado = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resultado);
    if(iResultado != 0){
        printf("getaddrinfo falhou: %d\n", iResultado);
        WSACleanup();
        return 1;
    }

    //server para se conectar com os clientes
    ListenSocket = socket(resultado ->ai_family, resultado-> ai_socktype, resultado -> ai_protocol);
    if(ListenSocket == INVALID_SOCKET){
        printf("Error do socket: %d\n", WSAGetLastError());
        freeaddrinfo(resultado);
        WSACleanup();
        return 1;
    }

    //Associar o soquete com a porta, por bin
    iResultado = bind(ListenSocket, resultado ->ai_addr, (int)resultado->ai_addrlen);
    freeaddrinfo(resultado);

    if(listen(ListenSocket, SOMAXCONN)==SOCKET_ERROR){
        printf("Error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
   
    //Servidor aceita conexão
    while(true){
        SOCKET ClienteSocket;
        ClienteSocket = accept(ListenSocket,NULL,NULL);
        if(ClienteSocket == INVALID_SOCKET){
            printf("conexão falha: %d\n",WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

    //bloqueio de vazamento de dadoos
    //thread para varias conexões 

    }

    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}