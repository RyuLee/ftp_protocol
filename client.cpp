#include<stdio.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define command "clear"
#include<errno.h>
#include<string.h>
#include<cstdlib>
#include<cerrno>
#include<arpa/inet.h>
#include<iostream>
#include <unistd.h>


using namespace std;

#define BIG_SIZE 1024
#define SMALL_SIZE 50


int main(int argc, char const *argv[])
{
	if(argc<3)
	{
		cout<<"Usage: client<address><port>[command]"<<endl;
		return 0;
	}
    char *cmd = new char [sizeof(argv[3])];
    strcpy(cmd,argv[3]);
    printf(cmd);
	if((strcmp(cmd,"get")==0)||(strcmp(cmd,"put")==0)||(strcmp(cmd,"cd"))==0)
	{
		if(argc<5)
		{
			cout<<"Usage: client<address><port>[command]<filename>|<dir>";
            return 0;
		}
        
	}

	char buf[BIG_SIZE] = {0},file[SMALL_SIZE],host_name[SMALL_SIZE]={0};

	int target,ret;
	char dataport[50];
	register int bytes,sockfd,sockdata;

    struct sockaddr_in scom,sdata;

	//strcpy(file,argv[3]);

	system(command);
	sprintf(host_name,"%s","Server");
    cout<<"Ready..."<<endl;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		fprintf(stderr, "Failed to initial socket\n");
		exit(2);
	}

	scom.sin_family = AF_INET;
	scom.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&scom.sin_addr);

	if((ret = connect(sockfd,(struct sockaddr*)&scom,sizeof(scom)))==-1)
	{
		fprintf(stderr, "Can't connect Server\n");
		exit(3);
	}
    cout<<"ready2"<<endl;
    //printf("Server connnected, ready to establish service");
    int len;
    if (recv(sockfd, &dataport, sizeof("8000"),0)<=0) {
        printf("Failed to receive port number.\n");
        exit(3);
    }
    cout<<"ready3"<<endl;
    cout<<dataport<<endl;
    //向服务器发起数据连接的请求
    if((sockdata = socket(AF_INET,SOCK_STREAM,0))<0)
    	{
    		fprintf(stderr, "Failed to initial socket in service\n");
    		exit(2);
    	}

    	sdata.sin_family = AF_INET;
    	sdata.sin_port = htons(atoi(dataport));
        cout<<dataport<<endl;
 		inet_pton(AF_INET,argv[1],&sdata.sin_addr);
        int dat;
 		if((dat = connect(sockdata,(struct sockaddr*)&sdata,sizeof(sdata)))==-1)
 		{
 			fprintf(stderr, "Can't connect Server\n");
 			exit(3);
 		}
 		printf("Service established.\n");
/////////////////////GET/////////////////////////
    cout<<cmd<<endl;
		if(strcmp(cmd,"get")==0)
		{
            cout<<"File ready"<<endl;
            char* filename = new char[sizeof(argv[4])];
            strcpy(filename, argv[4]);
		
//transfer the command through the command port
			send(sockfd,cmd,sizeof(cmd),0);//send the command
			send(sockfd,filename,sizeof(filename),0);
            char status[SMALL_SIZE];
			recv(sockfd,&status,SMALL_SIZE,0);
			if((strcmp(status, "450"))==0)
			{
				printf("File action not taken, file unavailible\n");
				exit(3);
			}
            
	 		FILE* file = fopen(filename,"w");
	 		int length;
            while ((length = recv(sockdata, &buf, BIG_SIZE, 0))>0) {
                cout<<"Receiving data"<<endl;
                if(fwrite(buf, sizeof(char), length, file) < length)
                {
                    printf("File:\t%s Write Failed\n", filename);
                    break;
                }
                cout<<length<<endl;
                memset(buf,0,BIG_SIZE);

            }
	 		printf("Closing data connection.\n");

	 		fclose(file);
	 		close(sockdata);
            close(sockfd);
            exit(3);
 		}

    if((strcmp(cmd,"put"))==0)
    {
        char filename[50];
        strcpy(filename,argv[4]);
        FILE* file;
        file = fopen(filename,"r");
        if(!file)
        {
            printf("no such file!\n");
            exit(3);
        }
        send(sockfd,filename,sizeof(filename),0);
        
        int length;
        //length = fread(&buf, sizeof(char), BIG_SIZE, file);
        //cout<<buf<<endl;
        //cout<<length<<endl;
        while((length = fread(&buf, sizeof(char), BIG_SIZE, file)) > 0)
        {
            cout<<length<<endl;
            if(send(sockdata, buf, length, 0) < 0)
            {
                printf("Send File:%s Failed./n", filename);
                break;
            }
            memset(buf,0, BIG_SIZE);
        }
	 		printf("Closing data connection.\n");

	 		fclose(file);
	 		close(sockdata);
            close(sockfd);
            exit(3);
 		}

    //Try to connect server to establish the service


    /*
     sprintf(buf,"%s_%s","receive",file);
	if((target=open(file,O_WRONLY|O_CREAT|O_TRUNC,0644))<0)
	{
		perror("Can't open file");
		exit(4);
	}
	memset(buf,0,BIG_SIZE);
	while((bytes = read(sockfd,buf,sizeof(buf)))>0)
	{
		write(target,buf,bytes);
		memset(buf,0,BIG_SIZE);
	}*/
	printf("Success\n");
	close(sockfd);
	close(target);
    close(sockdata);
	return 0;
}
