//FILE: overlay/overlay.c
//
//Description: this file implements a ON process 
//A ON process first connects to all the neighbors and then starts listen_to_neighbor threads each of which keeps receiving the incoming packets from a neighbor and forwarding the received packets to the SNP process. Then ON process waits for the connection from SNP process. After a SNP process is connected, the ON process keeps receiving sendpkt_arg_t structures from the SNP process and sending the received packets out to the overlay network. 
//
//Date: April 28,2008


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <sys/utsname.h>
#include <assert.h>

#include "../common/constants.h"
#include "../common/pkt.h"
#include "overlay.h"
#include "../topology/topology.h"
#include "neighbortable.h"

//you should start the ON processes on all the overlay hosts within this period of time
#define OVERLAY_START_DELAY 60

/**************************************************************/
//declare global variables
/**************************************************************/

//declare the neighbor table as global variable 
nbr_entry_t* nt; 
//declare the TCP connection to SNP process as global variable
int network_conn; 


/**************************************************************/
//implementation overlay functions
/**************************************************************/

// This thread opens a TCP port on CONNECTION_PORT and waits for the incoming connection from all the neighbors that have a larger node ID than my nodeID,
// After all the incoming connections are established, this thread terminates 
void* waitNbrs(void* arg) {
    int neighborNum = topology_getNbrNum();
    int myNodeId;
    int i,sockfd,clientConn = 0;
    struct sockaddr_in serverAddr,clientAddr;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        fprintf(stderr,"Create socket failed.\n");
        return NULL;
    }
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,0,0);

    myNodeId = topology_getMyNodeID();
    for(i = 0; i < neighborNum;i++){
        if(nt[i].nodeID > myNodeId){
            clientConn++;       
        }
    }

    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(CONNECTION_PORT);

    if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
        fprintf(stderr,"Bind failed.\n");
        return NULL;
    }

    if(listen(sockfd,clientConn) < 0){
        fprintf(stderr,"Listen failed.\n");
        return NULL;
    }

    printf("Waiting for neighbor connections...\n");
    int j;
    for(i = 0; i < clientConn;i++){
        socklen_t sockLen = sizeof(clientAddr);
        int clientfd = accept(sockfd,(struct sockaddr*)&clientAddr,&sockLen);
        if(clientfd < 0){
            fprintf(stderr,"Accept failed.\n");  
            return NULL;
        }
        for(j = 0; j < neighborNum;j++){
            if(nt[j].nodeID == topology_getNodeIDfromip(&clientAddr.sin_addr)){
                nt[j].conn = clientfd;
                break;
            }        
        }
        printf("Node %d receive request from %d\n",myNodeId,nt[j].nodeID);
    }
    
    return 0;
}

// This function connects to all the neighbors that have a smaller node ID than my nodeID
// After all the outgoing connections are established, return 1, otherwise return -1
int connectNbrs() {
    int myNodeId = topology_getMyNodeID();
    int neighborNum = topology_getNbrNum();
    struct sockaddr_in serverAddr;
    int i,sockfd;
    
    for(i = 0; i < neighborNum;i++){
        if(nt[i].nodeID < myNodeId){
            sockfd = socket(AF_INET,SOCK_STREAM,0);
            if(sockfd < 0){
                fprintf(stderr,"Create socket failed.\n");
                return -1;
            }
            memset(&serverAddr,0,sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(CONNECTION_PORT);
            serverAddr.sin_addr.s_addr = nt[i].nodeIP;
            if(connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr)) < 0){
                perror("connect");
                continue;
            }
            nt[i].conn = sockfd;
        }
    }

    return 0;
}

//Each listen_to_neighbor thread keeps receiving packets from a neighbor. It handles the received packets by forwarding the packets to the SNP process.
//all listen_to_neighbor threads are started after all the TCP connections to the neighbors are established 
void* listen_to_neighbor(void* arg) {
    int idx = *(int*)arg;
    free(arg);
    int conn = nt[idx].conn;

    snp_pkt_t pkt;
    for(;;){
        printf("Receiving pakcet from neighbor,nodeId: %d,conn = %d ... ",nt[idx].nodeID,conn);
        int r = recvpkt(&pkt,conn);
        if(r == -1){
            printf("FAILED.\n");
        }
        else{
            printf("SUCCEEDED.\n");

            printf("Forward pkt to SNP process...");

            printf("network_conn: %d\n",network_conn);
            r = forwardpktToSNP(&pkt,network_conn);
            if(r == -1){
                printf("FAILED.\n");
            }
            else{
                printf("SUCCEEDED.\n");
            }
        }
    }
    return 0;
}

//This function opens a TCP port on OVERLAY_PORT, and waits for the incoming connection from local SNP process. After the local SNP process is connected, this function keeps getting sendpkt_arg_ts from SNP process, and sends the packets to the next hop in the overlay network. If the next hop's nodeID is BROADCAST_NODEID, the packet should be sent to all the neighboring nodes.
void waitNetwork() {
    int sock;
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        printf("Create socket failed.\n");
        exit(-1);
    }
    struct sockaddr_in serverAddr;

    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(OVERLAY_PORT);
    

    int r = 1;
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&r,sizeof(r)) < 0){
        printf("Setsockopt failed.\n");
        exit(-1);
    }

    if(bind(sock,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr)) < 0){
        printf("Bind failed.\n");
        exit(-1);
    }

    if(listen(sock,1) < 0){
        printf("Listen failed.\n");
        exit(-1);
    }

    struct sockaddr_in clientAddr;
    int neighborNum = topology_getNbrNum();
    int myNodeId = topology_getMyNodeID();
    int i;

    for(;;){
        socklen_t sockLen = sizeof(struct sockaddr);
        int clientfd = accept(sock,(struct sockaddr*)&clientAddr,&sockLen);
        network_conn = clientfd;
    
        for(;;){
            int nextNode;
            snp_pkt_t pkt;
            int r = getpktToSend(&pkt,&nextNode,clientfd);
            if(r == -1){
                printf("Close connecton between SON and SNP process.\n");
                break;
            }
            if(pkt.header.dest_nodeID == BROADCAST_NODEID){
                printf("SOP receive BOARDCAST PACKET.\n");
                for(i = 0; i < neighborNum;i++){
                    printf("Node %d send BOARDCAST PACKAET to node %d ",myNodeId,nt[i].nodeID);
                    if(sendpkt(&pkt,nt[i].conn) < 0){
                        printf("FAILED.\n");
                    }
                    else{
                        printf("SUCCEEDED.\n");
                    }
                }
            }

            else{
                for(i = 0; i < neighborNum; i++){
                    if(nextNode == nt[i].nodeID){
                        break;
                    }
                }
                if(i == neighborNum){
                    printf("Node %d has not neighbor node %d.\n",myNodeId,nextNode);
                }
                else{
                    int conn = nt[i].conn;
                    printf("Node %d send pkt to node %d ",myNodeId,nextNode);
                    if(sendpkt(&pkt,conn) < 0){
                        printf("FAILED.\n");
                    }
                    else{

                        printf("SUCCEEDED.\n");
                    }
                }
            }
        }       
    }
}

//this function stops the overlay
//it closes all the connections and frees all the dynamically allocated memory
//it is called when receiving a signal SIGINT
void overlay_stop() {
    if(network_conn != -1){
        close(network_conn);
        network_conn = -1;
    }
    int neighborNum = topology_getNbrNum();
    int i;
    for(i = 0; i < neighborNum;i++){
        close(nt[i].conn);
    }
}

int main() {
	//start overlay initialization
	printf("Overlay: Node %d initializing...\n",topology_getMyNodeID());	

	//create a neighbor table
	nt = nt_create();
	//initialize network_conn to -1, means no SNP process is connected yet
	network_conn = -1;
	
	//register a signal handler which is sued to terminate the process
	signal(SIGINT, overlay_stop);

	//print out all the neighbors
	int nbrNum = topology_getNbrNum();
	int i;
	for(i=0;i<nbrNum;i++) {
		printf("Overlay: neighbor %d:%d\n",i+1,nt[i].nodeID);
	}

	//start the waitNbrs thread to wait for incoming connections from neighbors with larger node IDs
	pthread_t waitNbrs_thread;
	pthread_create(&waitNbrs_thread,NULL,waitNbrs,(void*)0);

	//wait for other nodes to start
	sleep(OVERLAY_START_DELAY);
	
	//connect to neighbors with smaller node IDs
	connectNbrs();

	//wait for waitNbrs thread to return
	pthread_join(waitNbrs_thread,NULL);	

	//at this point, all connections to the neighbors are created
	
	//create threads listening to all the neighbors
	for(i=0;i<nbrNum;i++) {
		int* idx = (int*)malloc(sizeof(int));
		*idx = i;
		pthread_t nbr_listen_thread;
		pthread_create(&nbr_listen_thread,NULL,listen_to_neighbor,(void*)idx);
	}
	printf("Overlay: node initialized...\n");
	printf("Overlay: waiting for connection from SNP process...\n");

	//waiting for connection from  SNP process
	waitNetwork();
}
