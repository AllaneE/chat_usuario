
# Chat-Online

Projeto feito para a disciplina de Linguagem de Programaçao 1, na linguagem c++. O projeto consiste em um chat que se comunica via internet por meio de uma estrutura do tipo cliente servidor.

## Guia de Uso
Para excecutar o projeto é necessário estar no SO windows 8 ou mais recente, ter um compilador C de preferencia o MinGW já que outros apresentaram incompatibilidade com algumas bibliotecas. Com esses pre requisitos, basta compilar o servidor.cpp e cliente.cpp e rodar os executaveis.
## Código
### servidor.cpp
1. Definições e Inclusões
- Desativa o suporte a Unicode.
- Define macros para minimizar a inclusão de headers do Windows e especificar a versão do Windows.
- Define constantes para porta padrão (DEFAULT_PORT), tamanho do buffer (DEFAULT_BUFLEN), e número máximo de conexões (MAX_CONEXOES).
- Declara a variável global conexoes_ativas para rastrear o número de conexões ativas.
- Inclui headers necessários para manipulação de sockets e threads.
2. Função `handle_client`
- A função handle_client gerencia a comunicação com um cliente específico.
- Recebe dados do cliente e retransmite para todos os outros clientes conectados, exceto o remetente.
- Utiliza um vetor de nomes de usuário para identificar mensagens de diferentes clientes.
- Lida com erros de comunicação e fechamento de conexões.
3. Função `main`
- A função main configura e inicia o servidor de chat.
- Inicializa a biblioteca Winsock com WSAStartup.
- Configura o endereço e porta do servidor com getaddrinfo.
- Cria um socket de escuta (ListenSocket) e o associa à porta configurada (bind).
- Coloca o socket em modo de escuta para aceitar conexões de clientes (listen).
- Aceita conexões de clientes em um loop até atingir o número máximo de conexões (accept).
- Cria uma nova thread para cada cliente conectado, chamando a função handle_client para tratar a comunicação.
- Aguarda a finalização de todas as threads antes de encerrar o servidor.
- Realiza a limpeza dos recursos usados, fechando sockets e desalocando a biblioteca Winsock (WSACleanup).
### cliente.cpp
1. Definições e Inclusões
- Define a versão mínima do Windows necessária (_WIN32_WINNT).
- Define macros para porta padrão (DEFAULT_PORT) e tamanho do buffer (DEFAULT_BUFLEN).
- Inclui headers necessários para manipulação de sockets e threads.
- Liga o código com a biblioteca ws2_32.lib necessária para o Winsock.
- Declara uma variável atômica running para controlar o estado de execução do programa de forma segura entre threads.
2. Função `receberMensagens`
- A função receberMensagens recebe dados do servidor de forma contínua.
- Exibe as mensagens recebidas no console.
- Fecha a conexão e para a execução em caso de erros ou se a conexão for fechada pelo servidor.
3. Função `enviarMensagens`
- A função enviarMensagens envia mensagens do usuário para o servidor.
- Lê a mensagem do console e envia-a para o servidor.
- Fecha a conexão se o usuário digitar "/exit".
- Lida com erros de envio e para a execução se houver falha.
4. Função `main`
- A função main inicializa a biblioteca Winsock com WSAStartup.
- Configura o endereço e porta do servidor com getaddrinfo.
- Cria um socket para se conectar ao servidor.
- Tenta conectar ao servidor usando o primeiro endereço retornado por getaddrinfo.
- Inicia uma thread para receber mensagens do servidor chamando a função receberMensagens.
- Envia mensagens ao servidor em um loop até que a variável running se torne false.
- Aguarda a finalização da thread de recepção de mensagens antes de realizar a limpeza dos recursos usados (closesocket e WSACleanup).

## Autores
- [@PuliSaglio](https://github.com/PuliSaglio)
- [@Allane](https://github.com/AllaneE)
- [@JoaoLucas](https://github.com/JoaoLucas2103)
