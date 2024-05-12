#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>//I HAVE ABSOLUTELY NO CLUE 
#include"../header/packet-struct.h"
#include"../header/root-servers.h"
#define PORT 53
void encodeDname(char* domainName){
    unsigned char* byteArr;
    //byteArr = malloc(256);
    char *token = strtok(domainName,".");  
    while(token!=NULL){
        printf("\\x%02x%s",strlen(token),token);
        token = strtok(NULL,".");
    }
    printf("\\x%02x\n",0);
}
int main(){
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0){
        perror("Socket creation failed");
        return 1;
    }
    printf("%s\n",ROOT_SERVER_A);//DEBUG CODE
    char domain[] = "google.com" ; 
    encodeDname(domain);
    return 0;
}