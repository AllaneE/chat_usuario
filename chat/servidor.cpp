/* Etapas de criar o servidor 
Inicializar Winsock.
Crie um soquete.
Associe o soquete.
Ouça no soquete de um cliente.
Aceite uma conexão de um cliente.
Receber e enviar dados.
Desconectar.*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "80"
int main(){

    WSADATA wsadata;
    int iResultado;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo *resultado = NULL, *ptr = NULL, hints;

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
    return 0;
}