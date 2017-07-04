#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SERVER_IP   "129.170.213.101"
#define MAXBUF         4096

void showMenu();
void water();
void reactor();
void power();
int connectTo(const char* ipAddress,short port);

int main()
{
    char input;

    printf("WELCOME TO THE THREE MILE ISLAND SENSOR NETWORK\n\n\n");
    for(;;){
        showMenu();
        scanf("%c",&input);
        if(input == '1'){
            water();
        }
        else if(input == '2'){
            reactor();
        }
        else if(input == '3'){
            power();
        }
        else{
            printf("Unknow sensor: %c\n",input);
        }
        while((input = getchar()) != '\n' && input != EOF);
    }
    return 0;
}


void showMenu()
{
    printf("Which sensor would you like to read:\n\n");
    printf("\t\t(1) Water temperature\n");
    printf("\t\t(2) Reactor temperature\n");
    printf("\t\t(3) Power level\n\n");
    printf("Selection:");
}

int connectTo(const char* ipAddress,short port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;

    if(sock  == -1){
        perror("create socket");
        exit(-1);
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ipAddress);

    if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0){
        perror("connect");
        exit(-1);
    }

    return sock;
}

/*
static parsePort(char* str)
{

}
*/


void water()
{
    int sock;
    short port;
    char buf[MAXBUF];
    char* token;
    char data1[] = {0x41,0x55,0x54,0x48,0x20,
                     0x73,0x65,0x63,0x72,0x65,
                     0x74,0x70,0x61,0x73,0x73,
                     0x77,0x6f,0x72,0x64,0x0a,0};
    char data2[] = {0x41,0x55,0x54,0x48,0x20,
                    0x6e,0x65,0x74,0x77,0x6f,
                    0x72,0x6b,0x73,0x0a,0x00};
    char data3[] = {0x57,0x41,0x54,0x45,0x52,
                    0x20,0x54,0x45,0x4d,0x50,
                    0x45,0x52,0x41,0x54,0x55,
                    0x52,0x45,0x0a,0x00};

    sock = connectTo(SERVER_IP,47789);
    send(sock,data1,strlen(data1),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    close(sock);

    token= strtok(buf," ");
    port = atoi(token);
    while(port == 0 && token != NULL){
        token = strtok(NULL," ");
        port = atoi(token);
    }

    if(port == 0){
        perror("parse port");
        exit(-1);
    }

    sock = connectTo(SERVER_IP,port);
    send(sock,data2,strlen(data2),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    send(sock,data3,strlen(data3),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    time_t local;

    token = strtok(buf," ");
    local = (time_t)atoll(token);
    token = strtok(NULL," ");
    int tem = atoi(token);
    memset(buf,0,sizeof(buf));
    char timebuf[64];
    int len = sprintf(timebuf,"%s",asctime(localtime(&local)));
    if(timebuf[len - 1] == '\n'){
        timebuf[len - 1] = 0;
    }
    sprintf(buf,"\n\tThe last WATER TEMPERATURE was taken %s and was %d %s\n\n",timebuf,tem,"F");

    printf("%s",buf);
}


void reactor()
{
    int sock;
    char* token;
    int port;
    char buf[MAXBUF];
    char data1[] = {0x41,0x55,0x54,0x48,0x20,
                     0x73,0x65,0x63,0x72,0x65,
                     0x74,0x70,0x61,0x73,0x73,
                     0x77,0x6f,0x72,0x64,0x0a,0};
    char data2[] = {0x41,0x55,0x54,0x48,0x20,
                    0x6e,0x65,0x74,0x77,0x6f,
                    0x72,0x6b,0x73,0x0a,0x00};
    char data3[] = {0x52,0x45,0x41,0x43,0x54,
                    0x4f,0x52,0x20,0x54,0x45,
                    0x4d,0x50,0x45,0x52,0x41,
                    0x54,0x55,0x52,0x45,0x0a,
                    0x00};
    sock = connectTo(SERVER_IP,47789);
    send(sock,data1,strlen(data1),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    close(sock);

    token= strtok(buf," ");
    port = atoi(token);
    while(port == 0 && token != NULL){
        token = strtok(NULL," ");
        port = atoi(token);
    }

    if(port == 0){
        perror("parse port");
        exit(-1);
    }

    sock = connectTo(SERVER_IP,port);
    send(sock,data2,strlen(data2),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    send(sock,data3,strlen(data3),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    time_t local;
    token = strtok(buf," ");
    local = (time_t)atoll(token);
    token = strtok(NULL," ");
    int tem = atoi(token);
    char timebuf[64];
    int len = sprintf(timebuf,"%s",asctime(localtime(&local)));
    if(timebuf[len - 1] == '\n'){
        timebuf[len - 1] = 0;
    }
    
    memset(buf,0,sizeof(buf));
    sprintf(buf,"\n\tThe last REACTOR TEMPERATURE was taken %s and was %d %s\n\n",timebuf,tem,"F");
    printf("%s",buf);
}

void power()
{

    int sock;
    char* token;
    int port;
    char buf[MAXBUF];
    char data1[] = {0x41,0x55,0x54,0x48,0x20,
                     0x73,0x65,0x63,0x72,0x65,
                     0x74,0x70,0x61,0x73,0x73,
                     0x77,0x6f,0x72,0x64,0x0a,0};
    char data2[] = {0x41,0x55,0x54,0x48,0x20,
                    0x6e,0x65,0x74,0x77,0x6f,
                    0x72,0x6b,0x73,0x0a,0x00};
    char data3[] = {0x50,0x4f,0x57,0x45,0x52,
                    0x20,0x4c,0x45,0x56,0x45,
                    0x4c,0x0a,0x00};
    sock = connectTo(SERVER_IP,47789);
    send(sock,data1,strlen(data1),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    close(sock);

    token= strtok(buf," ");
    port = atoi(token);
    while(port == 0 && token != NULL){
        token = strtok(NULL," ");
        port = atoi(token);
    }

    if(port == 0){
        perror("parse port");
        exit(-1);
    }

    sock = connectTo(SERVER_IP,port);
    send(sock,data2,strlen(data2),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    send(sock,data3,strlen(data3),0);
    memset(buf,0,sizeof(buf));
    recv(sock,buf,sizeof(buf),0);
    time_t local;
    token = strtok(buf," ");
    local = (time_t)atoll(token);
    token = strtok(NULL," ");
    int tem = atoi(token);
    char timebuf[64];
    int len = sprintf(timebuf,"%s",asctime(localtime(&local)));
    if(timebuf[len - 1] == '\n'){
        timebuf[len - 1] = 0;
    }
    
    memset(buf,0,sizeof(buf));
    sprintf(buf,"\n\tThe last POWER LEVEL was taken %s and was %d %s\n\n",timebuf,tem,"MW");
    printf("%s",buf);
}
