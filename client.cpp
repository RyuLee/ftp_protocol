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


using namespace std;

#define BIG_SIZE 1024
#define SMALL_SIZE 50

int main(int argc, char const *argv[])
{
	if(argc!=4)
	{
		cout<<"Usage: client<address><port><filename>"<<endl;
		return 0;
	}
	char buf[BIG_SIZE] = {0},file[SMALL_SIZE],host_name[SMALL_SIZE]={0};

	int target,ret;
	register int bytes,sockfd;

	struct sockaddr_in sin;

	strcpy(file,argv[3]);

	system(command);
	sprintf(host_name,"%s","Server");

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		fprintf(stderr, "File to initial socket\n");
		exit(2);
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[2]));

	inet_pton(AF_INET,argv[1],&sin.sin_addr);

	if((ret = connect(sockfd,(struct sockaddr*)&sin,sizeof(sin)))==-1)
	{
		fprintf(stderr, "Can't connect Server\n");
		exit(3);
	}

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
	}
	printf("Success\n");
	close(sockfd);
	close(target);
	return 0;
}
