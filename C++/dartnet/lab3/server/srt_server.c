#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include "srt_server.h"
#include "../common/constants.h"


/*interfaces to application layer*/

//
//
//  SRT socket API for the server side application.
//  ===================================
//
//  In what follows, we provide the prototype definition for each call and limited pseudo code representation
//  of the function. This is not meant to be comprehensive - more a guideline.
//
//  You are free to design the code as you wish.
//
//  NOTE: When designing all functions you should consider all possible states of the FSM using
//  a switch statement (see the Lab4 assignment for an example). Typically, the FSM has to be
// in a certain state determined by the design of the FSM to carry out a certain action.
//
//  GOAL: Your job is to design and implement the prototypes below - fill in the code.
//

// srt server initialization
//
// This function initializes the TCB table marking all entries NULL. It also initializes
// a global variable for the overlay TCP socket descriptor ‘‘conn’’ used as input parameter
// for snp_sendseg and snp_recvseg. Finally, the function starts the seghandler thread to
// handle the incoming segments. There is only one seghandler for the server side which
// handles call connections for the client.
//

svr_tcb_t** tcb;
int overlaySock;

static void* timeWaitout(void* arg);
static svr_tcb_t* gettcb(int sockfd)
{
    svr_tcb_t* pb = NULL;
    if(sockfd >= 0 && sockfd < MAX_TRANSPORT_CONNECTIONS){
        pb = tcb[sockfd];
    }

    return pb;
}

void srt_server_init(int conn) {
    pthread_t segThread;

    tcb = (svr_tcb_t**)malloc(sizeof(svr_tcb_t*) * MAX_TRANSPORT_CONNECTIONS);
    if(tcb == 0){
        perror("srt_server_init");
        return;
    }
    memset(tcb,0,sizeof(svr_tcb_t*) * MAX_TRANSPORT_CONNECTIONS);
    overlaySock = conn;

    pthread_create(&segThread,0,seghandler,0);
    return;
}

// Create a server sock
//
// This function looks up the client TCB table to find the first NULL entry, and creates
// a new TCB entry using malloc() for that entry. All fields in the TCB are initialized
// e.g., TCB state is set to CLOSED and the server port set to the function call parameter
// server port.  The TCB table entry index should be returned as the new socket ID to the server
// and be used to identify the connection on the server side. If no entry in the TCB table
// is available the function returns -1.

int srt_server_sock(unsigned int port) {
    int i;
    for(i = 0; i < MAX_TRANSPORT_CONNECTIONS;++i){
        if(tcb[i] == NULL){
            tcb[i] = (svr_tcb_t*)malloc(sizeof(svr_tcb_t));
            memset(tcb[i],0,sizeof(svr_tcb_t));
            tcb[i]->svr_portNum = port;
            tcb[i]->state = CLOSED;
            printf("Create server sock %d succeeded.\n",i);
            return i;
        }
    }

	return -1;
}

// Accept connection from srt client
//
// This function gets the TCB pointer using the sockfd and changes the state of the connetion to
// LISTENING. It then starts a timer to ‘‘busy wait’’ until the TCB’s state changes to CONNECTED
// (seghandler does this when a SYN is received). It waits in an infinite loop for the state
// transition before proceeding and to return 1 when the state change happens, dropping out of
// the busy wait loop. You can implement this blocking wait in different ways, if you wish.
//

int srt_server_accept(int sockfd) {
    svr_tcb_t* pb = gettcb(sockfd);
    if(pb == NULL){
        perror("no suck sockfd");
        return -1;
    }
    pb->state = LISTENING;

    while(pb->state == LISTENING){
        ;
    }

	return 1;
}

// Receive data from a srt client
//
// Receive data to a srt client. Recall this is a unidirectional transport
// where DATA flows from the client to the server. Signaling/control messages
// such as SYN, SYNACK, etc.flow in both directions. You do not need to implement
// this for Lab4. We will use this in Lab5 when we implement a Go-Back-N sliding window.
//
int srt_server_recv(int sockfd, void* buf, unsigned int length) {
	return 1;
}

// Close the srt server
//
// This function calls free() to free the TCB entry. It marks that entry in TCB as NULL
// and returns 1 if succeeded (i.e., was in the right state to complete a close) and -1
// if fails (i.e., in the wrong state).
//

int srt_server_close(int sockfd) {
    svr_tcb_t* pb = gettcb(sockfd);
    int state = -1;
    if(pb == NULL){
        perror("no suck sockfd");
        return -1;
    }
    state = pb->state;
    free(pb);
    tcb[sockfd] = NULL;
    
    switch(state){
        case CLOSED:
            printf("srt_server_close.\n");
            return 1;
        case LISTENING:
            printf("Can't close,LISTENING state.\n");
            return -1;
        case CONNECTED:
            printf("Can't close,CONNECTED state.\n");
            return -1;
        case CLOSEWAIT:
            printf("Can't close,CLOSEWAIT state.\n");
            return -1;
        default:
            printf("Can't close,Unknow state: %d\n",state);
            return -1;
    }
	return -1;
}

// Thread handles incoming segments
//
// This is a thread  started by srt_server_init(). It handles all the incoming
// segments from the client. The design of seghanlder is an infinite loop that calls snp_recvseg(). If
// snp_recvseg() fails then the overlay connection is closed and the thread is terminated. Depending
// on the state of the connection when a segment is received  (based on the incoming segment) various
// actions are taken. See the client FSM for more details.
//

void *seghandler(void* arg) {

    seg_t segment;
    svr_tcb_t* pb = NULL;
    int ret = -1;
    int i;
    memset(&segment,0,sizeof(segment));

    for(;;){
        ret = recvseg(overlaySock,&segment);
        if(ret < 0){
            break;
        }

        // 根据收到的segment的目的端口找到相应的tcb
        for(i = 0; i < MAX_TRANSPORT_CONNECTIONS;++i){
            pb = gettcb(i);
            if(pb == NULL){
                continue;
            }
            else{
                if(segment.header.dest_port == pb->svr_portNum){
                    pb = gettcb(i);
                    break;
                }
            }
        }

        if(pb == NULL){
            printf("Unknow destination port: %d,type = %d\n",segment.header.dest_port,segment.header.type);
            continue;
        }

        switch(pb->state){
            case CLOSED:
                printf("tcb has closed.\n");
                break;
            case LISTENING:
                if(segment.header.type == SYN){
                    printf("Change state to CONNECTED,src_port:%u,dest_port:%u,seq_num:%u,check_sum:%u.\n",
                            segment.header.src_port,
                            segment.header.dest_port,
                            segment.header.seq_num,
                            segment.header.checksum);
                    pb->state = CONNECTED;
                    pb->client_portNum = segment.header.src_port;
                    pb->expect_seqNum = segment.header.seq_num;   

                    seg_t sendSegment;
                    sendSegment.header.src_port = pb->svr_portNum;
                    sendSegment.header.dest_port = segment.header.src_port;
                    sendSegment.header.type = SYNACK;

                    if(sendseg(overlaySock,&sendSegment) < 0){
                        printf("Error send SYNACK segment.\n");
                    }
                }
                else{
                    printf("Current state is LISTENING,receive invalid segment type: %d.\n",segment.header.type);
                }
                break;
            case CONNECTED:
                if(segment.header.type == SYN){
                    printf("Current state is CONNECTED,receive SYN segment,dest_port:%u,seq_num:%i,check_sum:%u.\n",
                            segment.header.dest_port,
                            segment.header.seq_num,
                            segment.header.checksum);
                    pb->client_portNum = segment.header.src_port;
                    pb->expect_seqNum = segment.header.seq_num;

                    seg_t sendSegment;
                    sendSegment.header.src_port = pb->svr_portNum;
                    sendSegment.header.dest_port = segment.header.src_port;
                    sendSegment.header.type = SYNACK;

                    if(sendseg(overlaySock,&sendSegment) < 0){
                        printf("Error send SYNACK segment.\n");
                    }
                }
                else if(segment.header.type == FIN){
                    printf("Change state to CLOSEWAIT.\n");
                    pb->state = CLOSEWAIT;

                    seg_t sendSegment;
                    sendSegment.header.src_port = pb->svr_portNum;
                    sendSegment.header.dest_port = segment.header.src_port;
                    sendSegment.header.type = FINACK;

                    if(sendseg(overlaySock,&sendSegment) < 0){
                        printf("Send FINACK failed.\n");
                    }

                    pthread_t t;                    
                    if(pthread_create(&t,0,timeWaitout,pb)){
                        printf("Create timeWaitout thread failed.\n");
                    }

                }
                else{
                    printf("Current state is CONNECTED,receive invalid segment type: %d.\n",segment.header.type);
                }
                break;
            case CLOSEWAIT:
                if(segment.header.type == FIN){
                    printf("Current state is CLOSEWAIT,receive FIN segment.\n");
                
                    seg_t sendSegment;
                    sendSegment.header.src_port = pb->svr_portNum;
                    sendSegment.header.dest_port = segment.header.src_port;
                    sendSegment.header.type = FINACK;

                    if(sendseg(overlaySock,&sendSegment) < 0){
                        printf("Send FINACK failed.\n");
                    }
                }
                else{
                    printf("Current state is CLOSEWAIT,receive invalid segment type: %d.\n",segment.header.type);
                }
                break;
            default:
                printf("Unknow state: %d.\n",pb->state);
                break;
        }
    }

    return 0;
}

int timespec_sub(struct timespec* x,struct timespec* y,struct timespec* result)
{
    int negative = 0;
    if(x->tv_sec > y->tv_sec){
        if(x->tv_nsec > y->tv_nsec){
            result->tv_sec = x->tv_sec - y->tv_sec;
            result->tv_nsec = x->tv_nsec - y->tv_nsec;
        }
        else{
            result->tv_sec = x->tv_sec - y->tv_sec - 1;
            result->tv_nsec = 1e9 - y->tv_nsec + x->tv_nsec;
        }
    }
    else{
        if(x->tv_sec ==  y->tv_sec){
            result->tv_sec = 0;
            if(x->tv_nsec > y->tv_nsec){
                result->tv_nsec = x->tv_nsec - y->tv_nsec;
            }
            else{
                negative = 1;
                result->tv_nsec = y->tv_nsec - x->tv_nsec;
            }       
        }   
        else{
            negative = 1;
            if(x->tv_nsec > y->tv_nsec){
                result->tv_sec = y->tv_sec - x->tv_sec - 1;
                result->tv_nsec = 1e9 - x->tv_nsec + y->tv_nsec;
            }
            else{
                result->tv_sec = y->tv_sec - x->tv_sec;
                result->tv_nsec = y->tv_nsec - x->tv_nsec;
            }
        }
    }

    return negative;
}

void* timeWaitout(void* arg)
{
    svr_tcb_t* pb = (svr_tcb_t*)(arg);
    struct timespec start,end,result;

    clock_gettime(CLOCK_REALTIME,&start);

    while(pb->state == CLOSEWAIT){
        clock_gettime(CLOCK_REALTIME,&end);
        timespec_sub(&end,&start,&result);
        if(result.tv_sec > CLOSEWAIT_TIME){
            pb->state = CLOSED;
            break;
        }
    }
    pthread_exit(NULL);
    return 0;
}
