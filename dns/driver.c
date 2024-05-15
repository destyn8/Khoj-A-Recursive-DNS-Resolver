#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>//I HAVE ABSOLUTELY NO CLUE 
#include"../header/packet-struct.h"
#include"../header/root-servers.h"
#include<stdlib.h>
#define PORT 53
uint8_t* encodeDname(char domainName[]){
    char *label;
    label = malloc(64);
    int i,len = strlen(domainName);
    uint8_t* byteArr = malloc(len+2);
    if (byteArr == NULL) {
        printf("Memory allocation failed\n");
    }
    uint8_t zero = 0;
    uint8_t start_index = 0,k=0;
    size_t byteArrLen = 0;
    size_t totLen = 0;
    for(i =0;i<=len;i++){
        if(domainName[i]=='.' || domainName[i] =='\0' ){
            uint8_t labelLen = strlen(label);
            label = (uint8_t*) label;
            memcpy(byteArr+byteArrLen,&labelLen,1);
            byteArrLen+=sizeof(uint8_t);
            memcpy(byteArr+byteArrLen,label,labelLen);
            byteArrLen+=labelLen;
            start_index = i;
            memset(label,0,labelLen);
            k = 0;
        }       
        else{
            label[k] = domainName[i];
            k++;
        }
    }
    memcpy(byteArr+byteArrLen,&zero,1);
    i=0;
    return byteArr;
}
struct question buildQuery(char domainName[],enum RR_TYPE record){
    int id = (rand()%65683)+1;
    struct qPacket queryPacket;
    malloc(sizeof(queryPacket));
    queryPacket.header.id = id;
    queryPacket.header.rd = 1;
    queryPacket.header.tc = 0;
    queryPacket.header.aa = 0;
    queryPacket.header.op = 0;
    queryPacket.header.qr = 0;
    queryPacket.header.rcode = 0;
    queryPacket.header.z = 0;
    queryPacket.header.ra = 0;
    queryPacket.header.QDCOUNT = 1;
    queryPacket.header.ANCOUNT = 0;
    queryPacket.header.NSCOUNT = 0;
    queryPacket.header.ARCOUNT = 0;
    queryPacket.query.qName = encodeDname(domainName);
    queryPacket.query.question.qType = record;
    queryPacket.query.question.qClass = 1;//CLASS: IN
}//VERIFIED
int main(){
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0){
        perror("Socket creation failed");
        return 1;
    }
    printf("%s\n",ROOT_SERVER_A);//DEBUG CODE
    char domain[] = "www.google.ac.in" ; 
    buildQuery(domain,RR_A);
    return 0;
}
//uint8_t rd = 1<<8; //TRY IN CASE OF FALURE 
