
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "nbrcosttable.h"
#include "../common/constants.h"
#include "../topology/topology.h"

//This function creates a neighbor cost table dynamically 
//and initialize the table with all its neighbors' node IDs and direct link costs.
//The neighbors' node IDs and direct link costs are retrieved from topology.dat file. 
nbr_cost_entry_t* nbrcosttable_create()
{
    int neighborNum = topology_getNbrNum();
    int myNodeId = topology_getMyNodeID();
    nbr_cost_entry_t* entry = (nbr_cost_entry_t*)malloc(sizeof(nbr_cost_entry_t) * neighborNum);
    int* neighborArray = topology_getNbrArray();
    int i;
    for(i = 0; i < neighborNum;i++){
        entry[i].nodeID = neighborArray[i];
        entry[i].cost = topology_getCost(myNodeId,neighborArray[i]);
    }

    free(neighborArray);

    return entry;
}

//This function destroys a neighbor cost table. 
//It frees all the dynamically allocated memory for the neighbor cost table.
void nbrcosttable_destroy(nbr_cost_entry_t* nct)
{
    if(nct){
        free(nct);
    }
}

//This function is used to get the direct link cost from neighbor.
//The direct link cost is returned if the neighbor is found in the table.
//INFINITE_COST is returned if the node is not found in the table.
unsigned int nbrcosttable_getcost(nbr_cost_entry_t* nct, int nodeID)
{
    int neighborNum = topology_getNbrNum();
    int i;
    for(i = 0; i < neighborNum;i++){
        if(nct[i].nodeID == (unsigned)nodeID){
            return nct[i].cost;
        }
    }
    return INFINITE_COST;
}

//This function prints out the contents of a neighbor cost table.
void nbrcosttable_print(nbr_cost_entry_t* nct)
{
    int neighborNum = topology_getNbrNum();
    int myNodeId = topology_getMyNodeID();
    
    int i;
    for(i = 0; i < neighborNum;i++){
        printf("%d -> %d ,cost %d\n",myNodeId,nct[i].nodeID,nct[i].cost);
    }
}
