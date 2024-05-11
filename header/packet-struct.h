#include<stdio.h>
#include<stdint.h>
#include <endian.h>
struct header{
    uint16_t id;//IDENTIFICATION
#   if __BYTE_ORDER == __BIG_ENDIAN        
        uint8_t qr: 1;//QUERY-RESPONSE
        uint8_t op: 4;//QUERY TYPE
        uint8_t aa: 1;//AUTHORITATIVE SERVER - ANSWER
        uint8_t tc: 1;//TRUNCATION
        uint8_t rd: 1;//RECURSION DESIRED
        uint8_t ra: 1;//RECURSION AVAILABLE
        uint8_t z:  3;//RESERVED-FUTURE USE- KEEP ZERO
        uint8_t rcode: 4;//RESPONSE CODE
#   elif __BYTE_ORDER == __LITTLE_ENDIAN 
        uint8_t rd: 1;//RECURSION DESIRED
        uint8_t tc: 1;//TRUNCATION
        uint8_t aa: 1;//AUTHORITATIVE SERVER - ANSWER
        uint8_t op: 4;//QUERY TYPE
        uint8_t qr: 1;//QUERY-RESPONSE
        uint8_t rcode: 4;//RESPONSE CODE
        uint8_t z:  3;//RESERVED-FUTURE USE- KEEP ZERO
        uint8_t ra: 1;//RECURSION AVAILABLE
#   else 
#       error "Byte ordering at fault"
#   endif
    uint16_t QDCOUNT;//NO. OF ENTRIES IN QUESTION SECTION
    uint16_t ANCOUNT;//NO. OF DNS RECORDS [RR]
    uint16_t NSCOUNT;//NO. OF NS-TYPE RR
    uint16_t ARCOUNT;//NO. OF ADDITIONAL RECORDS [RR]
};
struct question{
    char qName[256];
    uint16_t qType;
    uint16_t qClass;
};
struct RR{
    uint16_t type;
    uint16_t rrClass;
    uint32_t ttl;
    uint16_t rdLength;
    //RDATA is dependant on type, and rrClass
};
struct packet{
    struct header header;
};