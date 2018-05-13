// Student ID : 20133190
// Name : 강성영

#define ID 20133190
#define NAME "SeongyoungKang"

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

#define PROMPT() {printf("\n> ");fflush(stdout);}
#define GETCMD "get"
#define QUITCMD "quit"

int check_down[11];
char write_f_name[BUFSIZ];
int main(int argc, char *argv[])
{
	int socktoserver = -1;
	char buf[BUFSIZ];
	char fname[BUFSIZ];	

    char request[BUFSIZ];

    struct hostent *hostp;
	struct sockaddr_in server;
    int sock;
	
    printf("Student ID : %d\n",ID);
	printf("Name : %s\n",NAME);
	PROMPT(); 

	for (;;)
    {
        // 사용자 부터 입력 받은 내용을 buf 에 담는다.
		if (!fgets(buf, BUFSIZ - 1, stdin))
        {
			if (ferror(stdin))
            {
				perror("stdin");
				exit(1);
			}
			exit(0);
		}

        // buf를 \t \n \r 공백 단위로 분할 합니다.
		char *cmd = strtok(buf, " \t\n\r");

        //문자열 끝이라면..?
		if((cmd == NULL) || (strcmp(cmd, "") == 0))
        {
			PROMPT(); 
			continue;
		}
        // 종료 문자열 호출시!
        else if(strcasecmp(cmd, QUITCMD) == 0)
        {
				exit(0);
		}
		//get 명령이나 공백,quite 가 아닌 다른 것 입력시.
        if(!strcasecmp(cmd, GETCMD) == 0)
        {
			printf("Wrong command %s\n", cmd);
			PROMPT(); 
			continue;
		}

        //여기까지 통과 했다면 재대로 된 format (get) 을 사용해서 명령한 것 입니다.
        // strtok 함수는 이전에 잘랐던 buf 주소를 기억하고 있습니다.
		char *hostname = strtok(NULL, " \t\n\r");
		char *pnum = strtok(NULL, " ");
		char *filename = strtok(NULL, " \t\n\r");
//        printf("host name is %s\n",hostname);
//        printf("port number is %s\n",pnum);
//        printf("file adress is %s \n",filename);
//        just for Debugging, print the URL

		// connect to a server
        if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
            perror("socket");
            exit(1);
        }
        if ((hostp = gethostbyname(hostname)) == 0)
        {
            fprintf(stderr,"%s: unknown host\n",hostname);
            exit(1);
        }
        memset((void *) &server, 0, sizeof (server));
        server.sin_family = AF_INET;
        memcpy((void *) &server.sin_addr, hostp->h_addr, hostp->h_length);
        server.sin_port = htons((u_short)atoi(pnum));

        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            (void) close(sock);
            fprintf(stderr, "connect");
            exit(1);
        }

        //write to array http request format
        sprintf(request,"GET %s HTTP/1.0\r\nHost:%s\r\nUser-agent:HW2/1.0\r\nConnection:close\r\n\r\n",filename,hostname);
        //send MSG
        if (write(sock, request, strlen(request)) < 0)
        {
                perror("write");
                exit(1);
        }
        char * write_temp;
        int bytesread;
        char * find_cont_len = NULL;
        char * find_last_header = NULL;
        char * c_flag = "Content-Length: ";
//        char * h_flag = "\r\n\r\n";
        int cnt =0;
        int first_flag =0;
        int filelen;
        int last_header_idx;
        int current_size =0;
        int rate;
        //find file name
        for(int i =0;i<strlen(filename);i++)
        {
            if(filename[i] == '/')
            {
                write_temp = &filename[i+1];
            }
        }
        int temp_cal = 0;
        while(write_temp[temp_cal] != '\0')
        {
            write_f_name[temp_cal] = write_temp[temp_cal];
            temp_cal++;
        }
        write_f_name[temp_cal] = '\0';
        FILE * in = fopen(write_f_name,"w+");
        if(in == NULL)
        {
            printf("file open error\n");
            continue;
        }
        
        for(;;)
        {

            bytesread = read(sock, buf, sizeof buf);
            
            // first, find content length & find last_idx of header
            if(cnt == 0)
            {
                find_cont_len = strstr(buf,c_flag);
                for(int i=0;i<strlen(c_flag);i++)
                    find_cont_len++;
                
                while(('0'<=find_cont_len[cnt]) && (find_cont_len[cnt]<='9'))
                {cnt++; }

                find_cont_len[cnt] = '\0';

                filelen = atoi(find_cont_len);
                printf("Total Size %d bytes\n",filelen);
                
                //find last index of header
                for(int i=0;i<bytesread;i++)
                {
                    if(buf[i]=='\r'&&buf[i+1]=='\n'&&buf[i+2]=='\r'&&buf[i+3]=='\n')
                    {
                       last_header_idx = i+4;
                       find_last_header = &buf[i+4];
                       break;

                    }
                }
            }
            
            //write to file
            //at first, use different pointer to ignore header
            if(first_flag == 0)
            {
                first_flag = 1;
                fwrite(find_last_header,bytesread-last_header_idx,1,in);
                current_size += bytesread-last_header_idx;
            }
            else
            {
                fwrite(buf,bytesread,1,in);
                current_size += bytesread;;
            }

            //check down loading size 
            rate = (int)(((double)current_size)/((double)filelen)*((double)10));
            if(rate >= 10)
                rate = 10;
            for(int i=1;i<=rate;i++)
            {
                if(check_down[i] == 0)
                {
                    check_down[i] = 1;
                    printf("Current Downloading %d/%d(bytes) %d%% \n ",current_size,filelen,i*10);
                }
            }
            if(current_size >= filelen)
            {
                printf("Download Complete: %s %d/%d ",write_f_name, current_size, filelen);
                break;
            } 
        }
        for(int i=0;i<=10;i++)
            check_down[i] = 0;
        fclose(in);
        PROMPT();
	}
} 
