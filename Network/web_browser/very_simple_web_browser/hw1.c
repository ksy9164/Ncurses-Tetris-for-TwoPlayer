#define Name "SeongYoung_Kang"
#define StudentID 20133190

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
#include <regex.h>

int portnum = 0;

int main(int argc, char *argv[])
{
	struct sockaddr_in server, remote;
	int request_sock, new_sock;
	int bytesread;
	socklen_t addrlen;
    int len;

    char buf[BUFSIZ];
    
    if (argc != 2)
    {
		(void) fprintf(stderr,"usage: %s portnum \n",argv[0]);
		exit(1);
	}
	portnum = atoi(argv[1]);

	if ((request_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
		perror("socket");
		exit(1);
	}
	
	printf("Student ID : %d\n",StudentID);
	printf("Name : %s\n",Name);

	// Create a Server Socket

    //init socket
    server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons((u_short)atoi(argv[1]));
    
    //bind
	if (bind(request_sock, (struct sockaddr *)&server, sizeof (server)) < 0)
    {
		perror("bind");
		exit(1);
	}
    //listen
	if (listen(request_sock, SOMAXCONN) < 0)
    {
		perror("listen");
		exit(1);
	}


    //receive request
	while(1)
    {
		/* a new connection is available on the connetion socket */
		addrlen = sizeof(remote);
		new_sock = accept(request_sock,(struct sockaddr *)&remote, &addrlen);

        if(new_sock <0 )
        {
			perror("accept");
			exit(1);
        }
		
        printf("connection from host %s, port %d, socket %d\n",inet_ntoa(remote.sin_addr), ntohs(remote.sin_port), new_sock);

        
        //read message 
        bytesread = read(new_sock, buf, sizeof (buf) - 1);
        if (bytesread<=0)
        {
            printf("server: end of file on %d\n", new_sock);
            if (close(new_sock)) 
                perror("close");
            break;
        }

        buf[bytesread] = '\0';
        //print message
        printf("%s: %d bytes from %d: %s\n", argv[0], bytesread, new_sock, buf);
       
        
        //check the file name with regex
        char * pattern = "GET /biga.html";
        regex_t e_regex;
        int ret = regcomp(&e_regex,pattern,REG_EXTENDED);
        ret = regexec(&e_regex,buf,0,NULL,0);
        ret = !ret;

        //can't find file.. parse the file name for print error!
        char * temp = (char *)malloc(strlen(buf)+1);
        strcpy(temp,buf);
        char * ptr1 = strstr(temp,"GET");
        int remember_idx = 0;
        if(ret == 0)
        {
            while(1)
            {
                //cut str ptr1[0 ~ remember_idx]
                if(ptr1[remember_idx] == 'H' &&ptr1[remember_idx+1] == 'T'&& ptr1[remember_idx+2] == 'T' &&ptr1[remember_idx+3] == 'P')
                    break;
                remember_idx++;
            }
        }


        // read html file to html_buf
        FILE * in = fopen("biga.html","rb");
        fseek(in,0,SEEK_END);
        int html_len = ftell(in);
        char * html_buf = (char*)malloc(sizeof(char)*html_len+1 );
        fseek(in,0,0);
        
        // http protocol
        fread(html_buf,1,html_len,in);
        snprintf(buf,sizeof(buf),"HTTP/1.0 200 OK\r\n");
        write(new_sock, buf, strlen(buf));
		
        snprintf(buf,sizeof(buf),"Content-Length: %d\r\n",html_len);
        write(new_sock, buf, strlen(buf));
		
        snprintf(buf,sizeof(buf),"Content-Type: text/html\r\n");
        write(new_sock, buf, strlen(buf));
        
        snprintf(buf,sizeof(buf),"\r\n");
        write(new_sock, buf, strlen(buf));
        
        //Go http!
        if(ret != 0)
        {
            //send message
            write(new_sock,html_buf, strlen(html_buf));
            printf("finish %d %d \n\n\n", (int)strlen(html_buf),html_len);
        }
        else
        {
            printf("Server Error : No such file ");
            ptr1[3] = '.';
            for(int k =3 ; k<remember_idx ; k++)
                printf("%c",ptr1[k]);
            printf("\n\n");
        }
    }
} /* main - hw1.c */


