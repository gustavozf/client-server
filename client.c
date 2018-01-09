#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(int argc , char *argv[]){

    int sock;
    char escolha[2];
    struct sockaddr_in server;
    char mensagem[1000] , respServidor[2000], aux[2000];

    //Criação socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
           printf("Não foi possível criar o socket");
    puts("Socket foi criado");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

  //Conexão
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("Conecção falhou!");
        return 1;
    }

    puts("Connected\n");
    printf("SUPER SISTEMA DE ALUGUEL DE CARROS\n");

    //Conexão contínua
    while(1){
        printf("Escolha uma função: \n");
        printf("\t1- Registrar um carro\n");
        printf("\t2- Alugar um carro\n");
        scanf("%s" , escolha);

        if( send(sock , escolha , 2 , 0) < 0){
            puts("Falha ao enviar");
            return 1;
        }

        if( recv(sock , respServidor , 2000 , 0) < 0){
            puts("Falha");
            break;
        }

        printf(respServidor);

        // Alugar um carro
        if(!(strcmp(escolha, "2"))){
            printf("Sendo feito");

        // Registrar um carro
        } else {
            printf("\nInsira o nome: ");
            scanf("%s", aux);
            send(sock, aux, strlen(aux), 0);
            recv(sock, respServidor , 2000, 0);
            printf(respServidor);

        }

        puts("Resposta do servidor: ");
        puts(respServidor);
    }

    close(sock);
    return 0;
}
