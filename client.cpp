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
	char* cmd = strcpy(cmd,argv[3]);

	if((strcmp(cmd,"get")!=0)||(strcmp(cmd,"put")!=0)||(strcmp(cmd,"cd"))!=0)
	{
		if(argc<4)
		{
			cout<<"Usage: client<address><port>[command]<filename>|<dir>";
		}
	}

	char buf[BIG_SIZE] = {0},file[SMALL_SIZE],host_name[SMALL_SIZE]={0};

	int target,ret;
	int dataport;
	register int bytes,sockfd,sockdata;

    struct sockaddr_in scom,sdata;

	strcpy(file,argv[3]);

	system(command);
	sprintf(host_name,"%s","Server");

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
    printf("Server connnected, ready to establish service");
    int len;
    if (recv(sockfd, &dataport, sizeof(dataport),0)<=0) {
        printf("Failed to receive port number.\n");
        exit(3);
    }
    //向服务器发起数据连接的请求
    while(1)
    {
    	if((sockdata = socket(AF_INET,SOCK_STREAM,0))<0)
    	{
    		fprintf(stderr, "Failed to initial socket in service\n");
    		exit(2);
    	}

    	sdata.sin_family = AF_INET;
    	sdata.sin_port = htons(dataport);
 		inet_pton(AF_INET,argv[1],&sdata.sin_addr);

 		if((dat = connect(sockdata,(struct sockaddr*)&sdata,sizeof(sdata)))==-1)
 		{
 			fprintf(stderr, "Can't connect Server\n");
 			exit(3);
 		}
 		printf("Service established.");
/////////////////////GET/////////////////////////
		if(strcmp(cmd,"get")==0)
		{
			char* filename = argv[4];
		
//transfer the command through the command port
			send(sockfd,cmd,sizeof(cmd),0);//send the command
			send(sockfd,filename,sizeof(filename),0);
			int status;
			recv(sockfd,&status,sizeof(int),0);
			if(status == 450)
			{
				printf("File action not taken, file unavailible\n");
				exit(3);
			}

	 		FILE* file = fopen(filename,"w");
	 		int n;
	 		do
	 		{
	 			recv(sockdata,&buf,sizeof(buf),0);
	 			fwrite(buf,sizeof(char),n,file);
	 		}while(n<BIG_SIZE);

	 		printf("Closing data connection.\n");

	 		fclose(file);
	 		close(sockdata);
 		}

 		if(strcmp(cmd,"put")==0)
		{
			char* filename = argv[4];
		
//transfer the command through the command port
			send(sockfd,cmd,sizeof(cmd),0);//send the command
	 		if((send(sockfd,filename,sizeof(filename),0))==-1)
	 		{
	 			fprintf(stderr, "Command transfer failed\n");
	 			exit(3);
	 		}

	 		FILE* file = fopen(filename,"r");
	 		int n;
	 		if(!file)
	 		{
	 			printf("File not found!\n");
	 			exit(3);
	 		}
	 		int n;
	 		do
	 		{
	 			fread(buf,sizeof(char),n,file)
	 			send(sockdata,&buf,sizeof(buf),0);
	 			
	 		}while(n<BIG_SIZE);

	 		printf("Closing data connection.\n");

	 		fclose(file);
	 		close(sockdata);
 		}




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
	return 0;
}
