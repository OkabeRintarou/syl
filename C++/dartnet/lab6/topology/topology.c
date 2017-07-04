//FILE: topology/topology.c
//
//Description: this file implements some helper functions used to parse 
//the topology file 
//
//Date: May 3,2010

#define _BSD_SOURCE

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "topology.h"
#include "../common/constants.h"

char* getLocalIp()
{
    int MAXINTERFACES = 16;
    char* ip = NULL;
    int fd,intrface;
    struct ifreq buf[MAXINTERFACES];
    struct ifconf ifc;

    if((fd = socket(AF_INET,SOCK_DGRAM,0)) >= 0){
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t)buf;
        if(!ioctl(fd,SIOCGIFCONF,(char*)&ifc)){
            intrface = ifc.ifc_len / sizeof(struct ifreq);
            
            while(intrface-- > 0){
                if(!(ioctl(fd,SIOCGIFADDR,(char*)&buf[intrface]))){
                    ip = (inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                    break;
                }
            }
        }
        close(fd);
        return ip;
    }
    return NULL;
}

//this function returns node ID of the given hostname
//the node ID is an integer of the last 8 digit of the node's IP address
//for example, a node with IP address 202.120.92.3 will have node ID 3
//if the node ID can't be retrieved, return -1
int topology_getNodeIDfromname(char* hostname) 
{
    char localname[32];
    gethostname(localname,sizeof(localname));
    if(strcmp(hostname,localname) == 0){
        return topology_getMyNodeID();
    }

    struct hostent* entry = gethostbyname(hostname);
    if(entry == NULL){
        return -1;
    }

    char buf[64];
    inet_ntop(entry->h_addrtype,entry->h_addr,buf,sizeof(buf));
    int a,b,c;
    int nodeId = 0x100;
    sscanf(buf,"%d.%d.%d.%d",&a,&b,&c,&nodeId);
    if(nodeId < 0 || nodeId > 0xff){
        return -1;
    }
    return nodeId;
}

//this function returns node ID from the given IP address
//if the node ID can't be retrieved, return -1
int topology_getNodeIDfromip(struct in_addr* addr)
{
    char* ip = inet_ntoa(*addr);
    int a,b,c,nodeId = 0x100;
    sscanf(ip,"%d.%d.%d.%d",&a,&b,&c,&nodeId);
    if(nodeId < 0 || nodeId > 0xff){
        return -1;
    }

    return nodeId;;
}

//this function returns my node ID
//if my node ID can't be retrieved, return -1
int topology_getMyNodeID()
{
    char* ip = getLocalIp();
    int a,b,c,nodeId = 0x100;
    sscanf(ip,"%d.%d.%d.%d",&a,&b,&c,&nodeId);
    if(nodeId < 0 || nodeId > 0xff){
        return -1;
    }
    return nodeId;
}

//this functions parses the topology information stored in topology.dat
//returns the number of neighbors
int topology_getNbrNum()
{
    FILE* fh;
    fh = fopen("../topology/topology.dat","r");
    if(fh == NULL){
        fprintf(stderr,"Cannot open topology.dat.\n");
        return -1;
    }

    int count = 0;
    int i;
    char hostname1[32],hostname2[32];
    int cost;
    char localname[32];
    gethostname(localname,sizeof(localname));
    
    for(i = 0; !feof(fh);++i){
        strcpy(hostname1,"vacant line");
        strcpy(hostname2,"vacant line"); // to prevent the vacant line
        fscanf(fh,"%s %s %d",hostname1,hostname2,&cost);
        if(strcmp(hostname1,localname) == 0 || strcmp(hostname2,localname) == 0 ){
            ++count;
        }
    }
    fclose(fh);

    return count;
}

//this functions parses the topology information stored in topology.dat
//returns the number of total nodes in the overlay 
int topology_getNodeNum()
{ 
    return 4;
}

//this functions parses the topology information stored in topology.dat
//returns a dynamically allocated array which contains all the nodes' IDs in the overlay network  
int* topology_getNodeArray()
{
    int* nodeId = (int*)malloc(sizeof(int) * 4);
    nodeId[0] = 129;
    nodeId[1] = 130;
    nodeId[2] = 199;
    nodeId[3] = 200;
    return nodeId;
}

//this functions parses the topology information stored in topology.dat
//returns a dynamically allocated array which contains all the neighbors'IDs  
int* topology_getNbrArray()
{
    int myNodeId = topology_getMyNodeID();
    int* nodeId = NULL;
    if(myNodeId == 129){
        nodeId = (int*)malloc(sizeof(int) * 3);
        nodeId[0] = 130;
        nodeId[1] = 199;
        nodeId[2] = 200;
    }
    else if(myNodeId == 130){
        nodeId = (int*)malloc(sizeof(int) * 2);
        nodeId[0] = 129;
        nodeId[1] = 200;
    }
    else if(myNodeId == 200){
        nodeId = (int*)malloc(sizeof(int) * 3);
        nodeId[0] = 129;
        nodeId[1] = 130;
        nodeId[2] = 199;
    }
    else if(myNodeId == 199){
        nodeId = (int*)malloc(sizeof(int) * 2);
        nodeId[0] = 129;
        nodeId[1] = 200;
    }
    return nodeId;


    return 0;
}

//this functions parses the topology information stored in topology.dat
//returns the cost of the direct link between the two given nodes 
//if no direct link between the two given nodes, INFINITE_COST is returned
unsigned int topology_getCost(int fromNodeID, int toNodeID)
{
    if(fromNodeID == toNodeID){
        return 0;
    }
    FILE* fh;
    fh = fopen("../topology/topology.dat","r");
    if(fh == NULL){
        fprintf(stderr,"Cannot open topology.dat\n");
        exit(-1);
    }

    int nodeId1,nodeId2;
    int i;
    char hostname1[32],hostname2[32];
    int cost;
    char localname[32];
    gethostname(localname,sizeof(localname));
    for(i = 0; !feof(fh);i++){
        strcpy(hostname1,"vacant line");
        strcpy(hostname2,"vacant line");
        fscanf(fh,"%s %s %d",hostname1,hostname2,&cost);
        nodeId1 = topology_getNodeIDfromname(hostname1);
        nodeId2 = topology_getNodeIDfromname(hostname2);


        if((nodeId1 == fromNodeID && nodeId2 == toNodeID) ||
                (nodeId1 == toNodeID && nodeId2 == fromNodeID)){
            fclose(fh);
            return cost;
        }
    }
    fclose(fh);
    return INFINITE_COST;
}

/*
int main()
{
    printf("yuliang:%d\n",topology_getNodeIDfromname("yuliang"));
    printf("yuliang64:%d\n",topology_getNodeIDfromname("yuliang64"));
    printf("deeplinux:%d\n",topology_getNodeIDfromname("deeplinux"));
    printf("pcbsd:%d\n",topology_getNodeIDfromname("mint32"));
    int fromNodeID = 129;
    int toNodeID = 200;
    printf("The cost between %d and %d is %d\n",
            fromNodeID,
            toNodeID,
            topology_getCost(fromNodeID,toNodeID));
    fromNodeID = 129;
    toNodeID = 130;
    printf("The cost between %d and %d is %d\n",
            fromNodeID,
            toNodeID,
            topology_getCost(fromNodeID,toNodeID));
    fromNodeID =  129;
    toNodeID = 199;
    printf("The cost between %d and %d is %d\n",
            fromNodeID,
            toNodeID,
            topology_getCost(fromNodeID,toNodeID));
    fromNodeID = 130;
    toNodeID = 200;
    printf("The cost between %d and %d is %d\n",
            fromNodeID,
            toNodeID,
            topology_getCost(fromNodeID,toNodeID));
    fromNodeID = 199;
    toNodeID = 200;
    printf("The cost between %d and %d is %d\n",
            fromNodeID,
            toNodeID,
            topology_getCost(fromNodeID,toNodeID));
    return 0;
}
*/
