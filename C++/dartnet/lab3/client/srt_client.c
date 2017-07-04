#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include "srt_client.h"


/*interfaces to application layer*/

//
//
//  SRT socket API for the client side application.
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
//  GOAL: Your goal for this assignment is to design and implement the 
//  protoypes below - fill the code.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// srt client initialization
//
// This function initializes the TCB table marking all entries NULL. It also initializes
// a global variable for the overlay TCP socket descriptor ‘‘conn’’ used as input parameter
// for snp_sendseg and snp_recvseg. Finally, the function starts the seghandler thread to
// handle the incoming segments. There is only one seghandler for the client side which
// handles call connections for the client.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//

client_tcb_t** tcb;
int overlaySock;

int timespec_sub(struct timespec* x,struct timespec* y,struct timespec* result);

void srt_client_init(int conn) {
    tcb = (client_tcb_t**)malloc(sizeof(client_tcb_t*) * MAX_TRANSPORT_CONNECTIONS);
    if(tcb == NULL){
        perror("malloc");
        return;
    }

    overlaySock = conn;

    memset(tcb,0,sizeof(client_tcb_t*) * MAX_TRANSPORT_CONNECTIONS);
    pthread_t segThread;
    pthread_create(&segThread,0,seghandler,0);

    return;
}

// Create a client tcb, return the sock
//
// This function looks up the client TCB table to find the first NULL entry, and creates
// a new TCB entry using malloc() for that entry. All fields in the TCB are initialized
// e.g., TCB state is set to CLOSED and the client port set to the function call parameter
// client port.  The TCB table entry index should be returned as the new socket ID to the client
// and be used to identify the connection on the client side. If no entry in the TC table
// is available the function returns -1.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//

int srt_client_sock(unsigned int client_port) {
    int i;
    for(i = 0; i < MAX_TRANSPORT_CONNECTIONS;++i){
        if(tcb[i] == NULL){
            tcb[i] = (client_tcb_t*)malloc(sizeof(client_tcb_t));
            memset(tcb[i],0,sizeof(client_tcb_t));
            tcb[i]->client_portNum  = client_port;
            tcb[i]->state = CLOSED;
            return i;
        }
    }
    return -1;
}

// Connect to a srt server
//
// This function is used to connect to the server. It takes the socket ID and the
// server’s port number as input parameters. The socket ID is used to find the TCB entry.
// This function sets up the TCB’s server port number and a SYN segment to send to
// the server using snp_sendseg(). After the SYN segment is sent, a timer is started.
// If no SYNACK is received after SYNSEG_TIMEOUT timeout, then the SYN is
// retransmitted. If SYNACK is received, return 1. Otherwise, if the number of SYNs
// sent > SYN_MAX_RETRY,  transition to CLOSED state and return -1.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//


int srt_client_connect(int sockfd, unsigned int server_port) {

    int i;
    for(i = 0; i < MAX_TRANSPORT_CONNECTIONS;++i){
        if(tcb[i] && i == sockfd){
            break;
        }
    }
    if( i == MAX_TRANSPORT_CONNECTIONS ){
        return -1;
    }

    client_tcb_t* pb = tcb[i];
    assert(pb);
    
    i = 0;
    seg_t segSend,segReceive;
    struct timespec start,end,result;

    memset(&segSend,0,sizeof(segSend));
    memset(&segReceive,0,sizeof(segReceive));

    switch(pb->state){
        case CLOSED:
            segSend.header.src_port = pb->client_portNum;
            segSend.header.dest_port = server_port;
            segSend.header.type = SYN;
            if(sendseg(overlaySock,&segSend) < 0){
                printf("sendseg failed.\n");
                return -1;
            }
            pb->state = SYNSENT;
            clock_gettime(CLOCK_REALTIME,&start);
            i = 1;

            while(pb->state == SYNSENT){
                if(i > SYN_MAX_RETRY){
                    printf("Retry sent SYN segment %d times and failed.\n",SYN_MAX_RETRY);
                    pb->state = CLOSED;
                    return -1;
                }
                clock_gettime(CLOCK_REALTIME,&end);
                int r = timespec_sub(&start,&end,&result);
                if(r > 0 && result.tv_sec * 1e9 + result.tv_nsec > SYNSEG_TIMEOUT_NS){
                    printf("超时: %d,%d\n",(int)(result.tv_sec * 1e9 + result.tv_nsec),SYNSEG_TIMEOUT_NS);
                     ++i;
                     if(sendseg(overlaySock,&segSend) < 0){
                        printf("sendseg failed.\n");
                        return -1;
                     }
                     clock_gettime(CLOCK_REALTIME,&start);
                }
            }
            pb->svr_portNum = server_port;
            return 1;
        case SYNSENT:
            printf("Cann't connect,current state SYNSENT.\n");
            break;
        case CONNECTED:
            printf("Cann't connect,current state CONNECTED.\n");
            break;
        case FINWAIT:
            printf("Cann't connect,current state FINWAIT.\n");
            break;
        default:
            printf("Unknoe state: %d.\n",pb->state);
            break; 
   }

    return -1;
}

// Send data to a srt server
//
// Send data to a srt server. You do not need to implement this for Lab4.
// We will use this in Lab5 when we implement a Go-Back-N sliding window.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//

int srt_client_send(int sockfd, void* data, unsigned int length) {
	return 1;
}

// Disconnect from a srt server
//
// This function is used to disconnect from the server. It takes the socket ID as
// an input parameter. The socket ID is used to find the TCB entry in the TCB table.
// This function sends a FIN segment to the server. After the FIN segment is sent
// the state should transition to FINWAIT and a timer started. If the
// state == CLOSED after the timeout the FINACK was successfully received. Else,
// if after a number of retries FIN_MAX_RETRY the state is still FINWAIT then
// the state transitions to CLOSED and -1 is returned.

int srt_client_disconnect(int sockfd) {

    int i;
    for(i = 0; i < MAX_TRANSPORT_CONNECTIONS;++i){
        if(tcb[i] && i == sockfd){
            break;
        }
    }

    if(i == MAX_TRANSPORT_CONNECTIONS){
        return -1;
    }

    client_tcb_t* pb = tcb[i];
    assert(pb);

    seg_t segSend,segReceive;
    struct timespec start,end,result;

    memset(&segSend,0,sizeof(segSend));
    memset(&segReceive,0,sizeof(segReceive));

    switch(pb->state){
        case CLOSED:
            printf("[Debug]:Current port: %d.\n",pb->client_portNum);
            printf("Current state CLOSED,do nothing.\n");
            return -1;
        case SYNSENT:
            printf("Current state SYSENT,do nothing.\n");
            return -1;
        case CONNECTED:
            printf("Current state CONNECTED or FINWAIT,send FIN segment.\n");
            segSend.header.src_port = pb->client_portNum;
            segSend.header.dest_port = pb->svr_portNum;
            segSend.header.type = FIN;
            if(sendseg(overlaySock,&segSend) < 0){
                printf("sendseg failed.\n");
                return -1;
            }
              
            printf("Client socket %d change state to FINWAIT.\n",pb->client_portNum);
            pb->state = FINWAIT;
            i = 1;

            clock_gettime(CLOCK_REALTIME,&start);
            while(pb->state == FINWAIT){
                if( i > FIN_MAX_RETRY){
                    return -1;
                }

                clock_gettime(CLOCK_REALTIME,&end);      
                timespec_sub(&start,&end,&result);
                if(result.tv_sec * 1e9 + result.tv_nsec > FINSEG_TIMEOUT_NS){
                    printf("没有收到FINACK，超时.\n");
                    ++i;
                    if(sendseg(overlaySock,&segSend) < 0){
                        printf("sendseg failed.\n");
                        return -1;
                    }
                    clock_gettime(CLOCK_REALTIME,&start);
                }
            }
            if(pb->state == CLOSED){
                return 1;
            }

            break;
        case FINWAIT:
            printf("Current state FINWAIT,do nothing.\n");
            return -1;
        default:
            assert(0); // shoule never arrive here
            return -1;
   }

    return -1;
}


// Close srt client

// This function calls free() to free the TCB entry. It marks that entry in TCB as NULL
// and returns 1 if succeeded (i.e., was in the right state to complete a close) and -1
// if fails (i.e., in the wrong state).
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//

int srt_client_close(int sockfd) {
    if(sockfd < 0 || sockfd >= MAX_TRANSPORT_CONNECTIONS){
        perror("no such sockfd");
        return -1;
    }
    client_tcb_t* pb = tcb[sockfd];
    int state = pb->state;
    free(pb);
    tcb[sockfd] = NULL;

    switch(state){
        case CLOSED:
            printf("client socket close.\n");
            return 1;
        case SYNSENT:
            printf("Can't close,SYNSENT state.\n");
            return -1;
        case CONNECTED:
            printf("Can't close,CONNECTED state.\n");
            return -1;
        case FINWAIT:
            printf("Can't close,FINWAIT state.\n");
            return -1;
        default:
            printf("Can't close,unknow state.\n");
            return -1;
    }
	return 0;
}

// The thread handles incoming segments
//
// This is a thread  started by srt_client_init(). It handles all the incoming
// segments from the server. The design of seghanlder is an infinite loop that calls snp_recvseg(). If
// snp_recvseg() fails then the overlay connection is closed and the thread is terminated. Depending
// on the state of the connection when a segment is received  (based on the incoming segment) various
// actions are taken. See the client FSM for more details.
//

void *seghandler(void* arg)
{
    client_tcb_t* pb = NULL;
    seg_t segment;
    int ret = -1;

    memset(&segment,0,sizeof(segment));

    for(;;){
        ret = recvseg(overlaySock,&segment);
        if(ret < 0){
            printf("recvseg failed.\n");
            break;
        }

        // 根据收到的segment目的端口找到相应的tcb
        int i;
        for(i = 0; i < MAX_TRANSPORT_CONNECTIONS;++i){
            pb = tcb[i];
            if(pb == NULL){
                continue;
            }
            else{
                if(segment.header.dest_port == pb->client_portNum){
                    break;
                }
            }
        }

        if(pb == NULL){
            printf("Unknow destination port: %d,type = %d.\n",segment.header.dest_port,segment.header.type);
            continue;
        }

        switch(pb->state){
            case CLOSED:
                printf("tcb has closed.\n");
                break;
            case SYNSENT:
                if(segment.header.type == SYNACK){
                    pb->state = CONNECTED;   
                }
                break;
            case CONNECTED:
                printf("Invalid state CONNECTED receive segment.\n");
                break;
            case FINWAIT:
                if(segment.header.type == FINACK){
                    pb->state = CLOSED;
                }
                break;
            default:
                printf("Unknow state:%d.\n",pb->state);
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

