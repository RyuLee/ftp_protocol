#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#define COMMAND_PORT 8000
#define DATA_PORT 8001
#define MAXLINE 4096

int main(int argc, char* argv[])
{
    int socket_fd,socket_data,cmd_fd,data_fd;
    struct sockaddr_in servaddr,dataaddr;
    char buff[1024];
    int n;
    
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(COMMAND_PORT);
    
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    
    if( listen(socket_fd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("======waiting for client's request======\n");
    while(1){
        
        if( (cmd_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        if((send(cmd_fd,DATA_PORT,2,0)==-1))
           {
               perror("failed to send port number");
               exit(0);
           }

        socket_data = socket(AF_INET,SOCK_STREAM,0);
        memset(&dataaddr,0,sizeof(dataaddr));
        dataaddr.sin_family = AF_INET;
        dataaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        dataaddr.sin_port = htons(DATA_PORT);
        if( bind(socket_data, (struct sockaddr*)&dataaddr, sizeof(dataaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
        if( listen(socket_data, 10) == -1){
            printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
            exit(0);
        }
        
        data_fd = accept(socket_data,(struct sockaddr*)NULL,NULL);

        char* cmd;
        recv(cmd_fd,&cmd,sizeof(char),0);

        if((strcmp(cmd,"get")
        {
            char* filename;
            recv(cmd_fd,&filename,sizeof(char),0);
            FILE* file;
            file = fopen(filename,"r");
            if(!file)
            {
                send(cmd_fd,450,sizeof(int),0);
                exit(3);
            }
            int n;
            do
            {
                fread(buff,sizeof(char),n,file)
                send(sockdata,&buff,sizeof(buff),0);
                
            }while(n<BIG_SIZE);
        }


        }
      
    close(socket_fd);
    
    
}
