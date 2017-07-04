/*
 * proxy.c - CS:APP Web proxy
 *
 * TEAM MEMBERS:
 *     Andrew Carnegie, ac00@cs.cmu.edu 
 *     Harry Q. Bovik, bovik@cs.cmu.edu
 * 
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */ 
#include "csapp.h"


typedef struct {
	int ThreadIdx;
	int Connfd;
	struct sockaddr_in ClientAddr;
}Dummy;


FILE * g_LogFile;
sem_t g_Mutex;
/*
 * Function prototypes
 */
int parse_uri(char *uri, char *target_addr, char *path, int  *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);
void * ThreadFunc(void*);

void fprintf_ts(FILE* f,const char * fmt,...)
{
	va_list args;
	va_start(args,fmt);
	P(&g_Mutex);
	vfprintf(f,fmt,args);
	fflush(f);
	V(&g_Mutex);
	va_end(args);
}

/* 
 * main - Main routine for the proxy program 
 */
int main(int argc, char **argv)
{

	int listenfd,port;
	int threadIdx = 0;

    /* Check arguments */
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
	exit(0);
    }
	
	if((port = atoi(argv[1])) == 0){
		fprintf(stderr,"atoi argument(%s) error\n",argv[1]);
		exit(1);
	}

	Signal(SIGPIPE,SIG_IGN);
	Sem_init(&g_Mutex,0,1);
	
	if((g_LogFile = fopen("proxy.log","a")) == NULL){
		fprintf(stderr,"fopen error\n");
		exit(1);
	}

	listenfd = Open_listenfd(port);

	for(;;){
		struct sockaddr_in clientaddr;
		socklen_t clientlen = sizeof(struct sockaddr_in);
		pthread_t tid;

		Dummy * data = (Dummy*)malloc(sizeof(Dummy));
		if(NULL == data){
			fprintf(stderr,"malloc error\n");
			continue;
		}
		if((data->Connfd = accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen)) < 0){
			perror("accept error");
			free(data);
			continue;
		}
		data->ThreadIdx = threadIdx++;
		data->ClientAddr = clientaddr;		
		pthread_create(&tid,NULL,ThreadFunc,data);
	}
	
    exit(0);
}

void * ThreadFunc(void * vargp)
{
	Dummy data = *(Dummy*)vargp;
	Free(vargp);
	if(pthread_detach(pthread_self()) < 0){
		perror("pthread_detach error");
		goto _cleanup;
	}

	rio_t riobuf;
	rio_readinitb(&riobuf,data.Connfd);
	
	char buf[MAXLINE] = "";
	char uri[MAXLINE] = "";
	char hostname[MAXLINE] = "";
	char pathname[MAXLINE] = "";
	int port;
	int nread;
	int clientfd = -1;

	if((nread = Rio_readlineb_w(&riobuf,buf,MAXLINE)) == 0){
		goto _cleanup;
	}
	{
		char method[35];
		char version[35];
		if(sscanf(buf,"%s %s %s",method,uri,version) != 3){
			goto _cleanup;
		}
	}
	
	if(parse_uri(uri,hostname,pathname,&port) == -1){
		printf("parse_uri failed\n");
		goto _cleanup;
	}
	
	// connect to server
	clientfd = open_clientfd_ts(hostname,port);
	if(clientfd < 0){
		goto _cleanup;
	}
	
	// forward request line to real server
	if(Rio_writen_w(clientfd,buf,nread) != nread){
		goto _cleanup;
	}

	// forward request head to real server
	for(;;){

		ssize_t wc;
		while((wc = Rio_readlineb_w(&riobuf,buf,sizeof(buf))) < 0){
			if(EINTR == errno)continue;

		} 
		if(wc == 0)break;

		if(Rio_writen_w(clientfd,buf,wc) != wc){
			goto _cleanup;
		}
		if(strcmp(buf,"\n") == 0 || strcmp(buf,"\r\n") == 0)break;
	}

	// forward response to client
	for(;;){

		ssize_t  wc;
		while((wc = read(clientfd,buf,sizeof(buf))) < 0){
			if(EINTR == errno){
				continue;
			}
			else{
				perror("read error");
				break;
			}
		}
		if(wc < 0)goto _cleanup;
		if(wc == 0)break;
		
		nread += wc;

		if(Rio_writen_w(data.Connfd,buf,wc) != wc){
			goto _cleanup;
		}
	}
	
	format_log_entry(buf,&data.ClientAddr,uri,nread);
	fprintf_ts(g_LogFile,"%s\n",buf);

_cleanup:
	close(data.Connfd);
	if(clientfd > 0){
		close(clientfd);
	}

	return NULL;
}
/*
 * parse_uri - URI parser
 * 
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, int *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
	hostname[0] = '\0';
	return -1;
    }
       
    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';
    
    /* Extract the port number */
    *port = 80; /* default */
    if (*hostend == ':')   
	*port = atoi(hostend + 1);
    
    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
	pathname[0] = '\0';
    }
    else {
	pathbegin++;	
	strcpy(pathname, pathbegin);
    }


    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring. 
 * 
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, 
		      char *uri, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /* 
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s", time_str, a, b, c, d, uri);
}


