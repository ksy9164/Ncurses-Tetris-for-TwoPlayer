/*
Student ID :"20133190"
Name : "Seongyoung Kang"
*/
#define ID "20133190"
#define NAME "Seongyoung Kang"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display();

int peertcpSocket = -1;	// peer socket

int main(int argc, char **argv)
{
	
  int tcpServ_sock;
  
  struct sockaddr_in tcpServer_addr;
  struct sockaddr_in tcpClient_addr;
  struct sockaddr_in newTcp_addr;
  
  int clnt_len;
    
  fd_set reads, temps;
  int fd_max;

  char command[1024];

  char *tcpport = NULL;
  char *userid = NULL;

// NEED TO ADD SOME VARIABLES 

  struct hostent *host_p;
  int msg_len;
  char send_msg[1024];
  
  char *inst = NULL;
  char *h_name = NULL;
  char *h_number = NULL;

  if(argc != 3){
    printf("Usage : %s <tcpport> <userid>\n", argv[0]);
    exit(1);
  }

  display();
    // NEED TO CREATE A SOCKET FOR TCP SERVER 
   if((tcpServ_sock = socket(PF_INET, SOCK_STREAM, 0)) <0 )
    {
        printf("socket( .. ) is Error!!");
        return 0;
	}
    memset(&tcpServer_addr, 0, sizeof(tcpServer_addr));
	tcpServer_addr.sin_family=AF_INET;
	tcpServer_addr.sin_addr.s_addr= INADDR_ANY;
    if(!isdigit(argv[1][0])) // check first number.. is it digit?
    {
        printf(" tcpport is not digit !! error !! \n");
        return 0;
    }
	tcpServer_addr.sin_port=htons(atoi(argv[1])); 
    // NEED TO bind
	if(bind(tcpServ_sock, (struct sockaddr *) &tcpServer_addr, sizeof(tcpServer_addr))==-1)
    {
		printf("bind( .. ) is Error!!");
        return 0;
    }       
    // NEED TO listen
	if(listen(tcpServ_sock,10)==-1)
    {
        printf("listen( .. ) is Error!!");
        return 0;
    }

    // initialize the select mask variables and set the
    // mask with stdin and the tcp server socket

    FD_ZERO(&reads);
	FD_SET(fileno(stdin), &reads);
	FD_SET(tcpServ_sock, &reads);
	
    tcpport = argv[1];
	userid = argv[2];
	fd_max=tcpServ_sock;
    printf("%s> \n", userid);

  while(1)
  {
    int nfound;

    temps = reads;
    
    nfound = select(fd_max+1, &temps, 0, 0, NULL);
	
	if(FD_ISSET(fileno(stdin), &temps))
    {
		// Input from the keyboard
		fgets(command, sizeof (command), stdin);
  		FD_CLR(fileno(stdin), &temps);

        if(command[0] == '@') // instruction !
        {
           inst = strtok(command," "); 
           h_name = strtok(NULL," ");
           h_number = strtok(NULL," ");

           if(!strcmp(inst ,"@talk")) // connect!
           {
               host_p =gethostbyname(h_name);
               
               memset((void *)&newTcp_addr, 0, sizeof(newTcp_addr));
               newTcp_addr.sin_family=AF_INET;
               newTcp_addr.sin_port=htons(atoi(h_number));
               memcpy((void *)&newTcp_addr.sin_addr,host_p->h_addr,host_p->h_length); 
               
               peertcpSocket = socket(PF_INET, SOCK_STREAM, 0);
               FD_SET(peertcpSocket, &reads);
               fd_max=peertcpSocket;
               if(connect(peertcpSocket,(struct sockaddr*)&newTcp_addr,sizeof newTcp_addr)==-1)
                    printf("connect(. . ) is Error");
           }
           else if(!strcmp(inst,"@quit\n"))
           {
               close(peertcpSocket);
               break;
           }
           else
           {
               printf("%s It's wrong instruction!! check plz!! \n",inst);
               continue;
           }
        }
        else // not instruction
        {
            strcat(command,"\n");
            
            strcpy(send_msg,userid);
            strcat(send_msg," : "); 
            strcat(send_msg,command);
		    write(peertcpSocket,send_msg,strlen(send_msg));
        }
	// NEED TO IMPLEMENT for input from keybord
  		printf("%s> \n", userid);
	}
	else if(FD_ISSET(tcpServ_sock, &temps))
	{
		//connect request from a peer
		clnt_len = sizeof(tcpClient_addr);
        if((peertcpSocket = accept(tcpServ_sock,(struct sockaddr*)&tcpClient_addr,&clnt_len))<0)
            printf("accept( .. ) is Error!!\n");
        
        FD_SET(peertcpSocket, &reads);
        fd_max=peertcpSocket;
        printf("Connection form host %s , port %d , socket %d \n",inet_ntoa(tcpClient_addr.sin_addr),ntohs(tcpClient_addr.sin_port),peertcpSocket);
	}
	else if(FD_ISSET(peertcpSocket, &temps))
	{
		// message from a peer
        if((msg_len=read(peertcpSocket, command, sizeof(command))) <0 )
            printf("read( .. ) is Error!!");
        
        if(msg_len == 0)
        {
            printf("Connection closed %d \n ", peertcpSocket); 
			FD_CLR(peertcpSocket, &reads);
            close(peertcpSocket);
        }else
            write(nfound,command,msg_len);
	}

  }//while End

}//main End

void display()
{
	printf("Student ID : %s \n",ID);
	printf("Name : %s  \n",NAME);
}
