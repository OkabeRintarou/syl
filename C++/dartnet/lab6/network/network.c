//FILE: network/network.c
//
//Description: this file implements network layer process  
//
//Date: April 29,2008



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <assert.h>
#include <sys/utsname.h>
#include <pthread.h>
#include <unistd.h>

#include "../common/constants.h"
#include "../common/pkt.h"
#include "../common/seg.h"
#include "../topology/topology.h"
#include "network.h"
#include "nbrcosttable.h"
#include "dvtable.h"
#include "routingtable.h"

//network layer waits this time for establishing the routing paths 
#define NETWORK_WAITTIME 60

/**************************************************************/
//delare global variables
/**************************************************************/
int overlay_conn; 			//connection to the overlay
int transport_conn;			//connection to the transport
nbr_cost_entry_t* nct;			//neighbor cost table
dv_t* dv;				//distance vector table
pthread_mutex_t* dv_mutex;		//dvtable mutex
routingtable_t* routingtable;		//routing table
pthread_mutex_t* routingtable_mutex;	//routingtable mutex


/**************************************************************/
//implementation network layer functions
/**************************************************************/

//This function is used to for the SNP process to connect to the local ON process on port OVERLAY_PORT.
//TCP descriptor is returned if success, otherwise return -1.
int connectToOverlay() { 
	//put your code here
    int sock;
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        perror("socket");
        return -1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family  = AF_INET;
    if(inet_aton("127.0.0.1",(struct in_addr*)&serverAddr.sin_addr.s_addr) == 0){
        return -1;
    }
    serverAddr.sin_port = htons(OVERLAY_PORT);

    printf("Connect to the local ON process on port %d ... ",OVERLAY_PORT);
    if(connect(sock,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr)) == -1){
        printf("%s\n","FAILED");
        perror("connect");
        return -1;
    }
    printf("%s\n","SUCCEEDED");

    return sock;
}

//This thread sends out route update packets every ROUTEUPDATE_INTERVAL time
//The route update packet contains this node's distance vector. 
//Broadcasting is done by set the dest_nodeID in packet header as BROADCAST_NODEID
//and use overlay_sendpkt() to send the packet out using BROADCAST_NODEID address.
void* routeupdate_daemon(void* arg) {
	//put your code here
    snp_pkt_t pkt;

    int myNodeId = topology_getMyNodeID();
    int overlayNodeNum = topology_getNodeNum();
    int neighborNum = topology_getNbrNum();

    pkt_routeupdate_t* updatePkt;
    for(;;){
        printf("Send out route update pakcets\n");    
        pkt.header.src_nodeID = myNodeId;
        pkt.header.dest_nodeID = BROADCAST_NODEID;
        pkt.header.type = ROUTE_UPDATE;
        pkt.header.length = sizeof(unsigned int) + overlayNodeNum * sizeof(routeupdate_entry_t);

        updatePkt = (pkt_routeupdate_t*)pkt.data;
        updatePkt->entryNum = overlayNodeNum;

        pthread_mutex_lock(dv_mutex);
        memcpy(updatePkt->entry,dv[neighborNum].dvEntry,updatePkt->entryNum);
        pthread_mutex_unlock(dv_mutex);

        overlay_sendpkt(BROADCAST_NODEID,&pkt,overlay_conn);
        sleep(ROUTEUPDATE_INTERVAL);
    }
    return 0;
}

//This thread handles incoming packets from the ON process.
//It receives packets from the ON process by calling overlay_recvpkt().
//If the packet is a SNP packet and the destination node is this node, forward the packet to the SRT process.
//If the packet is a SNP packet and the destination node is not this node, forward the packet to the next hop according to the routing table.
//If this packet is an Route Update packet, update the distance vector table and the routing table. 
void* pkthandler(void* arg) {
    snp_pkt_t pkt;
    int myNodeId = topology_getMyNodeID();
    int neighborNum = topology_getNbrNum();
    int nodesNum = topology_getNodeNum();
    int* neighborArray = topology_getNbrArray();
    int* nodesArray = topology_getNodeArray();

    while(overlay_recvpkt(&pkt,overlay_conn) > 0){
        printf("Routing: received a packet from neighbor %d\n",pkt.header.src_nodeID);

        if(pkt.header.type == SNP){
            if(myNodeId == pkt.header.dest_nodeID){
                // forward the packet to the SRT process
                forwardsegToSRT(transport_conn,pkt.header.src_nodeID,(seg_t*)pkt.data);
            }
            else{
                // forward the packet to the next hop according to the routing table
                int nextHop = routingtable_getnextnode(routingtable,pkt.header.dest_nodeID);
                if(nextHop){
                    printf("[Error]: Routingtable Can't find next hop to forward segment.\n");
                }
                overlay_sendpkt(nextHop,&pkt,overlay_conn);
            }
        }
        else if(pkt.header.type == ROUTE_UPDATE){
            // update the distance vector table and the routing table

            pkt_routeupdate_t* updatePkt = (pkt_routeupdate_t*)pkt.data;
            printf("Receive route update packet.\n");

            pthread_mutex_lock(dv_mutex);
            pthread_mutex_lock(routingtable_mutex);

            int srcNodeId = pkt.header.src_nodeID;   
            for(unsigned int j = 0; j < updatePkt->entryNum;j++){
                dvtable_setcost(dv,srcNodeId,updatePkt->entry[j].nodeID,updatePkt->entry[j].cost);
            }
        
            for(int j = 0; j < nodesNum;j++){
                int destNodeId = nodesArray[j];
                for(int i = 0; i < neighborNum;i++){
                    int neighborNodeId = neighborArray[i];
                    int minCost = dvtable_getcost(dv,myNodeId,destNodeId);
                    int reCalcCost = nbrcosttable_getcost(nct,neighborNodeId) + dvtable_getcost(dv,neighborNodeId,destNodeId);
                    if(reCalcCost < minCost){
                        minCost = reCalcCost;
                        if(minCost < INFINITE_COST && srcNodeId != destNodeId){
                            dvtable_setcost(dv,srcNodeId,destNodeId,minCost);
                            routingtable_setnextnode(routingtable,destNodeId,neighborNodeId);
                        }
                    }
                }
            }
            
            pthread_mutex_unlock(dv_mutex);
            pthread_mutex_unlock(routingtable_mutex);
        }
        else{
            printf("[Error]: Unknow received packet type.\n]");
        }
    }

    close(overlay_conn);
    overlay_conn = -1;
    free(neighborArray);
    free(nodesArray);
    pthread_exit(NULL);
    return 0;
}

//This function stops the SNP process. 
//It closes all the connections and frees all the dynamically allocated memory.
//It is called when the SNP process receives a signal SIGINT.
void network_stop() {
    if(overlay_conn != -1){
        close(overlay_conn);
        overlay_conn = -1;
    }
    if(transport_conn != -1){
        close(transport_conn);
        transport_conn = -1;
    }

    nbrcosttable_destroy(nct);
    dvtable_destroy(dv);
    routingtable_destroy(routingtable);

    return;
}

//This function opens a port on NETWORK_PORT and waits for the TCP connection from local SRT process.
//After the local SRT process is connected, this function keeps receiving sendseg_arg_ts which contains the segments and their destination node addresses from the SRT process. The received segments are then encapsulated into packets (one segment in one packet), and sent to the next hop using overlay_sendpkt. The next hop is retrieved from routing table.
//When a local SRT process is disconnected, this function waits for the next SRT process to connect.
void waitTransport() {
    int sock;
    int myNodeId = topology_getMyNodeID();

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        printf("Create socket failed.\n");
        exit(-1);
    }
    struct sockaddr_in serverAddr;

    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(NETWORK_PORT);

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
    for(;;){
        socklen_t sockLen = sizeof(struct sockaddr);
        int clientfd = accept(sock,(struct sockaddr*)&clientAddr,&sockLen);
        transport_conn = clientfd;

        for(;;){
            int destNodeId;
            seg_t segment;
            int r = getsegToSend(transport_conn,&destNodeId,&segment);
            if(r == -1){
                printf("Close connection between SNP and SRT process.\n");
                break;
            }
        
            int nextHop = routingtable_getnextnode(routingtable,destNodeId);
            if(nextHop == -1){
                printf("Routing table cannot find next hop for destination node %d.\n",destNodeId);
            }
            else{
                snp_pkt_t pkt;
                pkt.header.src_nodeID = myNodeId;
                pkt.header.dest_nodeID = destNodeId;
                pkt.header.type = SNP;
                pkt.header.length = segment.header.length + 24;
                memcpy(pkt.data,&segment,segment.header.length);

                r = overlay_sendpkt(nextHop,&pkt,overlay_conn);
                if(r == -1){
                    printf("Overlay send packet failed.\n");
                }
                else{
                    printf("Overlay send packet to next hop,src nodeId: %d,dest nodeId: %d.\n",
                            pkt.header.src_nodeID,pkt.header.dest_nodeID);
                }
            }
        }
    
        transport_conn = -1;
    }
}

int main(int argc, char *argv[]) {
	printf("network layer is starting, pls wait...\n");

	//initialize global variables
	nct = nbrcosttable_create();
	dv = dvtable_create();
	dv_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(dv_mutex,NULL);
	routingtable = routingtable_create();
	routingtable_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(routingtable_mutex,NULL);
	overlay_conn = -1;
	transport_conn = -1;

    printf("nbrcosttable:\n");
	nbrcosttable_print(nct);
    printf("dvtable:\n");
	dvtable_print(dv);
    printf("routingtable:\n");
	routingtable_print(routingtable);

	//register a signal handler which is used to terminate the process
	signal(SIGINT, network_stop);

	//connect to local ON process 
	overlay_conn = connectToOverlay();
	if(overlay_conn<0) {
		printf("can't connect to overlay process\n");
		exit(1);		
	}
	
	//start a thread that handles incoming packets from ON process 
	pthread_t pkt_handler_thread; 
	pthread_create(&pkt_handler_thread,NULL,pkthandler,(void*)0);

	//start a route update thread 
	pthread_t routeupdate_thread;
	pthread_create(&routeupdate_thread,NULL,routeupdate_daemon,(void*)0);	

	printf("network layer is started...\n");
	printf("waiting for routes to be established\n");
	sleep(NETWORK_WAITTIME);
	routingtable_print(routingtable);

	//wait connection from SRT process
	printf("waiting for connection from SRT process\n");
	waitTransport(); 
}

