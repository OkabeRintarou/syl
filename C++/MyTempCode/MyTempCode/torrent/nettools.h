#ifndef NETTOOLS_HPP
#define NETTOOLS_HPP

typedef struct sockaddr SA;
#define LISTENQ 512

#ifndef __cplusplus
extern "C" {
#endif
int open_clientfd(const char * hostname,int port);
int open_listenfd(int port);

#ifndef __cplusplus
}
#endif

#endif // NETTOOLS_HPP
