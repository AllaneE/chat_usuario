#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define MAX_CONEXOES 10

int conexoesAtivas = 0;

#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <thread>
#include <vector>
#include <iostream>

void handleClient(SOCKET ClientSocket[], int id) {
    int iResult, i;
    char recvBuf[DEFAULT_BUFLEN];
    int recvBuflen = DEFAULT_BUFLEN;
    const char* nomesUsuario[MAX_CONEXOES]={"Lara-croft", "Mario", "Agent-47", "Sonic", "Sackboy", "Pac-man", "Link", "Master-Chief", "Kratos", "Shadowheart"};
    char msg[DEFAULT_BUFLEN];
    strcpy(msg, nomesUsuario[id]);
    strcat(msg, " diz: \0");
    // Receber e enviar dados
    do {
        iResult = recv(ClientSocket[id], recvBuf, recvBuflen, 0);
        if (iResult > 0) {
            printf("Bytes recebidos: %d\n", iResult);

            // Ecoar o buffer de volta para o remetente
            for (i = 0; i < conexoesAtivas; i++) {
                if(ClientSocket[i] != ClientSocket[id]){
                    send(ClientSocket[i], msg, sizeof(msg), 0);
                    int iSendResult = send(ClientSocket[i], recvBuf, iResult, 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("Falha no envio: %d\n", WSAGetLastError());
                    closesocket(ClientSocket[i]);
                    conexoesAtivas -=1;
                    return;
                }
                }
                // printf("Bytes enviados: %d\nMensagem recebida: %s\n", iSendResult, recvbuf);
                // memset(recvbuf, 0, recvbuflen);
            }
        }
        else if (iResult == 0) {
            printf("Conexão fechando...\n");
        } else {
            printf("Falha na recepção: %d\n", WSAGetLastError());
            closesocket(ClientSocket[id]);
            conexoesAtivas -=1;
            return;
        }
    } while (iResult > 0);

    // Limpeza
    closesocket(ClientSocket[id]);
}

int main() {
    WSADATA wsadata;
    int iResultado;
    int id = 0;
    struct addrinfo *resultado = NULL, *ptr = NULL, hints;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET clientesConectados[MAX_CONEXOES];
    std::vector<std::thread> threads;

    // Inicializando Winsock
    iResultado = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResultado != 0) {
        printf("WSAStartup falhou: %d\n", iResultado);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolvendo endereço e porta do servidor
    iResultado = getaddrinfo(NULL, DEFAULT_PORT, &hints, &resultado);
    if (iResultado != 0) {
        printf("getaddrinfo falhou: %d\n", iResultado);
        WSACleanup();
        return 1;
    }

    // Criando um socket para conexão com clientes
    ListenSocket = socket(resultado->ai_family, resultado->ai_socktype, resultado->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Erro no socket: %d\n", WSAGetLastError());
        freeaddrinfo(resultado);
        WSACleanup();
        return 1;
    }

    // Associando o socket à porta
    iResultado = bind(ListenSocket, resultado->ai_addr, (int)resultado->ai_addrlen);
    freeaddrinfo(resultado);
    if (iResultado == SOCKET_ERROR) {
        printf("Erro no bind: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        conexoesAtivas -=1;
        WSACleanup();
        return 1;
    }

    // Ouvindo conexões de clientes
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Erro no listen: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        conexoesAtivas -=1;
        WSACleanup();
        return 1;
    }

    // Servidor aceita conexões
    while (id < MAX_CONEXOES) {
        clientesConectados[id] = accept(ListenSocket, NULL, NULL);
        if (clientesConectados[id] == INVALID_SOCKET) {
            printf("Conexão falhou: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        printf("Cliente conectado com socket: %d\n", clientesConectados[id]);
        conexoesAtivas += 1;
        // Criar uma nova thread para tratar o cliente
        threads.emplace_back(handleClient, clientesConectados, id);
        id += 1;

    }

    // Espera todas as threads terminarem
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Limpeza
    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}