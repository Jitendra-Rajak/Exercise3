#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc,char *argv[])
{
	int socket_fd,client_fd;/*These are the file descriptor for */
	int port_no;
	int pid;
	int client_count=0;
	int client_len;
	char buffer[256];
	struct sockaddr_in server_addr,client_addr;
	if(argc<2){
		error("Port number not provided \n");
	}

	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd<0)
		error("Error in creating socket \n");
	bzero((char*)&server_addr,sizeof(server_addr));
	port_no                     =atoi(argv[1]);
	server_addr.sin_family      =AF_INET;
	server_addr.sin_addr.s_addr =INADDR_ANY;
	server_addr.sin_port        =htons(port_no);

	if(bind(socket_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
		error("Error on binding the socket \n");
	listen(socket_fd,5);
	client_len=sizeof(client_addr);
	while(1){
		client_fd=accept(socket_fd,(struct sockaddr *)&client_addr,&client_len);
		if(client_fd<0)
			error("Error on accepting");
		client_count++;
		pid=fork();
		if(pid==0){
			printf("client %d is connected",client_count);
			while(1){
				bzero(buffer,sizeof(buffer));
				read(client_fd,buffer,256);
				if(strcmp(buffer,"bye\n")!=0){
					printf("Message from client %d is:%s",client_count,buffer);
					bzero(buffer,sizeof(buffer));
				}
				else{
					printf("Last message from client %d is:%s",client_count,buffer);
					bzero(buffer,sizeof(buffer));
					break;
				}

			}
		}
		close(client_fd);
	}

	return 0;

}

