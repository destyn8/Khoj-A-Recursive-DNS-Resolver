#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"../header/packet-struct.h"
#include"../header/root-servers.h"
#include<stdlib.h>
#define PORT 53
size_t packetLength = 0;
uint8_t* encodeDname(char domainName[]){
    char *label;
    label = malloc(64);
    int i,len = strlen(domainName);
    uint8_t* byteArr = malloc(len+2);
    if (byteArr == NULL) {
        printf("Memory allocation failed\n");
    }
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
    return byteArr;
}
uint8_t* buildQuery(char domainName[],enum RR_TYPE record){
    uint8_t zero = 0;
    int id = (rand()%65682)+1;
    struct header header;
    struct query query;
    uint8_t* packetBuffer;
    packetBuffer = malloc(512);
    memset(packetBuffer, 0, sizeof(packetBuffer));
    header.id = id;
    header.rd = 1;
    header.tc = 0;
    header.aa = 0;
    header.op = 0;
    header.qr = 0;
    header.rcode = 0;
    header.z = 0;
    header.ra = 0;
    header.QDCOUNT = htons(1);
    header.ANCOUNT = 0;
    header.NSCOUNT = 0;
    header.ARCOUNT = 0;
    memcpy(packetBuffer,&header,sizeof(header));
    packetLength+=sizeof(header);
    query.qName = encodeDname(domainName);
    memcpy(packetBuffer+packetLength,query.qName,strlen(query.qName));
    packetLength+=strlen(query.qName);
    memcpy(packetBuffer+packetLength,&zero,1);
    packetLength+=1;
    query.question.qType  = htons(record);
    query.question.qClass = htons(1);//CLASS: IN
    memcpy(packetBuffer+packetLength,&query.question.qType,2);
    packetLength+=2;
    memcpy(packetBuffer+packetLength,&query.question.qClass,sizeof(query.question.qClass));
    return packetBuffer;
}
int main(){
    uint8_t* packetBuffer;
    packetBuffer = malloc(512);
    char domain[] = "www.google.in";
    int sockfd = socket(AF_INET,SOCK_DGRAM,0); 
    struct sockaddr_in socket;
    if (sockfd < 0){
        perror("Socket creation failed");
        return 1;
    }
    socket.sin_family = AF_INET;
    socket.sin_port = htons(53);
    if (inet_pton(AF_INET, "8.8.8.8", &socket.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    packetBuffer = buildQuery(domain,RR_A);
    printf("\npacketLength:%d\n",packetLength);
    size_t sent = sendto(sockfd,packetBuffer,packetLength+2,0,(struct sockaddr *)&socket,sizeof(socket));
    int j;
    for(j=0;j<33;j++){
        printf("%2.2x",packetBuffer[j]);
    }

    printf("\n");
    return 0;
}
//uint8_t rd = 1<<8; //TRY IN CASE OF FALURE 