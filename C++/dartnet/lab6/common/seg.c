#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "seg.h"
#include "pkt.h"


//SRT process uses this function to send a segment and its destination node ID in a sendseg_arg_t structure to SNP process to send out. 
//Parameter network_conn is the TCP descriptor of the connection between the SRT process and the SNP process. 
//Return 1 if a sendseg_arg_t is succefully sent, otherwise return -1.
int snp_sendseg(int network_conn, int dest_nodeID, seg_t* segPtr)
{
    char bufstart[2];
    char bufend[2];
    bufstart[0] = '!';
    bufstart[1] = '&';
    bufend[0] = '!';
    bufend[1] = '#';
    if(send(network_conn,bufstart,2,0) < 0){
        return -1;
    }
    sendseg_arg_t sendArg = {dest_nodeID,*segPtr};
    if(send(network_conn,&sendArg,sizeof(sendseg_arg_t),0) < 0){
        return -1;
    }
    if(send(network_conn,bufend,2,0) < 0){
        return -1;
    }
    return 1;
}

//SRT process uses this function to receive a  sendseg_arg_t structure which contains a segment and its src node ID from the SNP process. 
//Parameter network_conn is the TCP descriptor of the connection between the SRT process and the SNP process. 
//When a segment is received, use seglost to determine if the segment should be discarded, also check the checksum.  
//Return 1 if a sendseg_arg_t is succefully received, otherwise return -1.
int snp_recvseg(int network_conn, int* src_nodeID, seg_t* segPtr)
{
    char buf[sizeof(sendseg_arg_t)+2]; 
    char c;
    int idx = 0;
    // state can be 0,1,2,3; 
    // 0 starting point 
    // 1 '!' received
    // 2 '&' received, start receiving segment
    // 3 '!' received,
    // 4 '#' received, finish receiving segment 
    int state = 0; 
    while(recv(network_conn,&c,1,0)>0) {
        if (state == 0) {
            if(c=='!')
                state = 1;
        }
        else if(state == 1) {
            if(c=='&') 
                state = 2;
            else
                state = 0;
        }
        else if(state == 2) {
            if(c=='!') {
                buf[idx]=c;
                idx++;
                state = 3;
            }
            else {
                buf[idx]=c;
                idx++;
            }
        }
        else if(state == 3) {
            if(c=='#') {
                buf[idx]=c;
                idx++;
                state = 0;
                idx = 0;
                if(seglost(segPtr)>0) {
                    printf("seg lost!!!\n");
                    continue;
                }
                memcpy(src_nodeID,buf,sizeof(*src_nodeID));
                memcpy(segPtr,buf + sizeof(*src_nodeID),sizeof(seg_t));
                return 1;
            }
            else if(c=='!') {
                buf[idx]=c;
                idx++;
            }
            else {
                buf[idx]=c;
                idx++;
                state = 2;
            }
        }
    }
    return 0;
}

#define SEGSTART1 1
#define SEGSTART2 2
#define SEGRECV   3
#define SEGSTOP1  4
//SNP process uses this function to receive a sendseg_arg_t structure which contains a segment and its destination node ID from the SRT process.
//Parameter tran_conn is the TCP descriptor of the connection between the SRT process and the SNP process. 
//Return 1 if a sendseg_arg_t is succefully received, otherwise return -1.
int getsegToSend(int tran_conn, int* dest_nodeID, seg_t* segPtr)
{
    char buf[sizeof(sendseg_arg_t) + 2];
    char c;
    int idx = 0;
    
    int state = SEGSTART1;
    while(recv(tran_conn,&c,1,0) > 0){
        if(state == SEGSTART1){
            if(c == '!'){
                state = SEGSTART2;
            }
        }
        else if(state == SEGSTART2){
            if(c == '&'){
                state = SEGRECV;
            }
            else{
                state = SEGSTART1;
            }
        }
        else if(state == SEGRECV){
            if(c == '!'){
                buf[idx] = c;
                idx++;
                state = SEGSTOP1;
            }
            else{
                buf[idx] = c;
                idx++;
            }
        }
        else if(state == SEGSTOP1){
            if(c == '#'){
                buf[idx] = c;
                idx++;
                state = SEGSTART1;
                memcpy(dest_nodeID,buf,sizeof(int));
                memcpy(segPtr,buf + sizeof(int),sizeof(*segPtr));
                return 1;
            }
            else if(c == '!'){
                buf[idx] = c;
                idx++;
            }
            else{
                buf[idx] = c;
                idx++;
                state = SEGRECV;
            }
        }
    }

    return 0;
}

//SNP process uses this function to send a sendseg_arg_t structure which contains a segment and its src node ID to the SRT process.
//Parameter tran_conn is the TCP descriptor of the connection between the SRT process and the SNP process. 
//Return 1 if a sendseg_arg_t is succefully sent, otherwise return -1.
int forwardsegToSRT(int tran_conn, int src_nodeID, seg_t* segPtr)
{
    sendseg_arg_t seg = {src_nodeID,*segPtr};

    char sendbuf[2];
    sendbuf[0] = '!';
    sendbuf[1] = '&';

    if(send(tran_conn,sendbuf,2,0) < 0){
        printf("send failure!\n");
        return -1;
    }

    if(send(tran_conn,&seg,sizeof(sendseg_arg_t),0) < 0){
        printf("send failure!\n");
        return -1;
    }

    sendbuf[0] = '!';
    sendbuf[1] = '#';
    if(send(tran_conn,sendbuf,2,0) < 0){
        printf("send failure!\n");
        return -1;
    }

    return 1;
}

// for seglost(seg_t* segment):
// a segment has PKT_LOST_RATE probability to be lost or invalid checksum
// with PKT_LOST_RATE/2 probability, the segment is lost, this function returns 1
// If the segment is not lost, return 0. 
// Even the segment is not lost, the packet has PKT_LOST_RATE/2 probability to have invalid checksum
// We flip  a random bit in the segment to create invalid checksum
int seglost(seg_t* segPtr)
{
    int random  = rand() % 100;
    if(random < PKT_LOSS_RATE * 100){
        if(rand() % 2 == 0){
            printf("seg lost!!!\n");
            return 1;
        }
    }
    else{
        int len = sizeof(srt_hdr_t) + segPtr->header.length;
        int errorbit = rand() % (len * 8);
        char* temp = (char*)segPtr;
        temp = temp + errorbit / 8;
        *temp = *temp ^ (1 << (errorbit % 8));
        return 0;
    }
    return 0;
}
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//This function calculates checksum over the given segment.
//The checksum is calculated over the segment header and segment data.
//You should first clear the checksum field in segment header to be 0.
//If the data has odd number of octets, add an 0 octets to calculate checksum.
//Use 1s complement for checksum calculation.
unsigned short checksum(seg_t* segment)
{
    unsigned short* buf = (unsigned short*)segment;
    int size = segment->header.length + sizeof(segment->header);
    unsigned long cksum = 0;
    while(size > 1){
        cksum += *buf++;
        size -= sizeof(unsigned short);
    }
    if(size){
        unsigned short t = *((char*)buf);
        cksum += (t << 8);
    }
    while((cksum >> 16) & 0x0000ffff){
        cksum = ((cksum >> 16) & 0x0000ffff) + (cksum & 0xffff);
    }

    return (unsigned short)(~cksum);
}

//Check the checksum in the segment,
//return 1 if the checksum is valid,
//return -1 if the checksum is invalid
int checkchecksum(seg_t* segment)
{
    if(segment->header.length > MAX_SEG_LEN){
        return -1;
    }
    if(checksum(segment) == 0){
        return 1;
    }
    return -1;
}
