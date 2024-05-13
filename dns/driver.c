#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>//I HAVE ABSOLUTELY NO CLUE 
#include"../header/packet-struct.h"
#include"../header/root-servers.h"
#define PORT 53
void encodeDname(char domainName[]){
    char *label;
    label = malloc(64);
    int i,len = strlen(domainName);
    uint8_t* byteArr =malloc(len+2);
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
    i=0;
    while(i<=strlen(domainName)){
        printf("%d:%c\n",byteArr[i],byteArr[i]);
        i++;
    }
}
int main(){
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0){
        perror("Socket creation failed");
        return 1;
    }
    printf("%s\n",ROOT_SERVER_A);//DEBUG CODE
    char domain[] = "google.ac.in" ; 
    encodeDname(domain);
    return 0;
}