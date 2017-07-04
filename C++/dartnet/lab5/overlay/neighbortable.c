//FILE: overlay/neighbortable.c
//
//Description: this file the API for the neighbor table
//
//Date: May 03, 2010

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include "../common/constants.h"
#include "../topology/topology.h"
#include "neighbortable.h"

//This function first creates a neighbor table dynamically. It then parses the topology/topology.dat file and fill the nodeID and nodeIP fields in all the entries, initialize conn field as -1 .
//return the created neighbor table
nbr_entry_t* nt_create()
{
    int nodeNum = topology_getNbrNum();
    nbr_entry_t* entries = (nbr_entry_t*)malloc(sizeof(nbr_entry_t) * nodeNum);   

    memset(entries,0,sizeof(nbr_entry_t) * nodeNum);

    FILE* fh = fopen("../topology/topology.dat","r");
    if(fh == NULL){
        fprintf(stderr,"No such file.\n");
        return NULL;
    }

    int* nodeIds = topology_getNbrArray();
    char ipBuf[32];
    int i;
    for(i = 0; i < nodeNum;i++){
        entries[i].nodeID = nodeIds[i];
        sprintf(ipBuf,"192.168.44.%d",nodeIds[i]);
        inet_pton(AF_INET,ipBuf,(void*)&entries[i].nodeIP);
        entries[i].conn = -1;
    }
    free(nodeIds);

    return entries;
}

//This function destroys a neighbortable. It closes all the connections and frees all the dynamically allocated memory.
void nt_destroy(nbr_entry_t* nt)
{
    int nodeNum = topology_getNbrNum();
    int i;
    for(i = 0; i < nodeNum;i++){
        if(nt[i].conn > 0){
            close(nt[i].conn);
        }
    }
    free(nt);
}

//This function is used to assign a TCP connection to a neighbor table entry for a neighboring node. If the TCP connection is successfully assigned, return 1, otherwise return -1
int nt_addconn(nbr_entry_t* nt, int nodeID, int conn)
{
     return 0;
}
