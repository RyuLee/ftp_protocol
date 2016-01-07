#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <iostream>
#define COMMAND_PORT 8000
#define DATA_PORT "8001"
#define BIG_SIZE 1024
#define SMALL_SIZE 50
using namespace std;

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
        if((send(cmd_fd,DATA_PORT,sizeof(DATA_PORT),0))==-1)
           {
               perror("failed to send port number");
               exit(0);
           }

        socket_data = socket(AF_INET,SOCK_STREAM,0);
        memset(&dataaddr,0,sizeof(dataaddr));
        dataaddr.sin_family = AF_INET;
        dataaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        dataaddr.sin_port = htons(atoi(DATA_PORT));
        if( bind(socket_data, (struct sockaddr*)&dataaddr, sizeof(dataaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
        if( listen(socket_data, 10) == -1){
            printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
            exit(0);
        }
        
        data_fd = accept(socket_data,(struct sockaddr*)NULL,NULL);

        char cmd[50];
        recv(cmd_fd,&cmd,SMALL_SIZE,0);
        cout<<cmd<<endl;
        if((strcmp(cmd,"get"))==0)
        {
            char filename[50];
            recv(cmd_fd,&filename,SMALL_SIZE,0);
            //cout<<filename<<endl;
            FILE* file;
            file = fopen(filename,"r");
            if(!file)
            {
                send(cmd_fd,"450",sizeof("450"),0);
                printf("no such file!\n");
                exit(3);
            }

            int length;
            while((length = fread(buff, sizeof(char), BIG_SIZE, file)) > 0)
            {
                cout<<length<<endl;
                if(send(data_fd, buff, length, 0) < 0)
                {
                    printf("Send File:%s Failed./n", filename);
                    break;
                }
                memset(buff,0, BIG_SIZE);
            }
            fclose(file);
            close(socket_data);
            close(data_fd);
            close(cmd_fd);
        }
        
        if((strcmp(cmd,"put"))==0)
        {
            char filename[50];
            recv(cmd_fd,&filename,SMALL_SIZE,0);
            //cout<<filename<<endl;
            FILE* file;
            file = fopen(filename,"w");
            int length;
            cout<<"hear"<<endl;
            memset(buff,0,BIG_SIZE);
            while ((length = recv(data_fd, &buff, BIG_SIZE, 0))>0) {
                cout<<"Receiving data"<<endl;
                if(fwrite(buff, sizeof(char), length, file) < length)
                {
                    printf("File:\t%s Write Failed\n", filename);
                    break;
                }
                cout<<length<<endl;
                memset(buff,0,BIG_SIZE);
                
            }
            fclose(file);
            close(socket_data);
            close(data_fd);
            close(cmd_fd);
        }



        }
      
    close(socket_fd);
    close(data_fd);
    
    
}
