/*
    C socket server example, handles multiple clients using threads
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

#define PORT 8888
#define MAX_FILA 3

// Função executada ao receber uma requisição
void *connection_handler(void *);

int main(int argc , char **argv){
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    // Cria o socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Não foi possível criar o socket");
    }
    puts("-Socket criado");

    // Configura o servidor
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Faz a ligação do socket ao servidor
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("Erro ao realizar vínculo");
        return 1;
    }
    puts("-Vínculo terminado");

    listen(socket_desc , MAX_FILA); //Começa a receber solicitações
    puts("-Aguardando conexões...");

    c = sizeof(struct sockaddr_in);
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
        puts("Conexão recebida");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0){
            perror("Não foi possível criar thread");
            return 1;
        }

        puts("-Manipulador designado");
    }

    if(client_sock < 0){
        perror("Erro na conexão");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    FILE *memoria;

    //Abre o arquivo de memoria compartilhada
    if(!(memoria = fopen("memoria.txt", "r+"))){
        memoria = fopen("memoria.txt", "w+");
    }

    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char escolha;
    char *message , client_message[2000];

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , escolha , 1 , 0)) > 0 )
    {
        switch(escolha){
            case '1':
                    message = "Insira os dados: ";
                    send(sock, message, strlen(message), 0);
                    recv(sock, client_message, 2000, 0);

                    fseek(memoria, 0, SEEK_END);
                    write(memoria, client_message, strlen(client_message));

                    message = "Carro registrado!";
                    send(sock, message, strlen(message), 0);

                    break;

            //case '2':
            //        break;

            default:
                    break;
        }

        //Send the message back to client
        fclose(memoria);
        write(sock , client_message , strlen(client_message));
    }

    if(read_size == 0)
    {
        puts("Cliente desconectado");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
