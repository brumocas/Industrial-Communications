#include "ModbusTCP.h"

#define MBAP_SIZE 7     // Header size of MBAP in bytes
#define UNIT_ID 1      // Slave ID


uint16_t TI = 0; 

// Debug instructions
#define DEBUG
//#undef DEBUG_OFF

int send_modbus_request(char* server_addr, unsigned int port, uint8_t* APDU, uint16_t APDUlen, uint8_t* APDU_r){
    
    int sock, i;
    struct sockaddr_in server;
    char MBAP[MBAP_SIZE];

    // Create socket:
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("[TCP] Error: creating socket\n");
        return -1;
    }
    printf("[TCP] Socket created\n");
    
    // Set port and IP of the server
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(server_addr);

    // Try connecting with the server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {   
        printf("[TCP] Error: connecting to server\n");
        return -1;
    }
    printf("[TCP] Successful connection to the server\n");

    // Build MBAPDU header
    TI++;
    MBAP[0] = (uint8_t) (TI >> 8);              //  TI high byte
    MBAP[1] = (uint8_t) (TI & 0xFF);            //  TI low byte   
    MBAP[2] = (uint8_t) (0x00);                 //  Modbus protocol identifier
    MBAP[3] = (uint8_t) (0x00);                 //  Modbus protocol identifier
    MBAP[4] = (uint8_t) ((APDUlen + 1) >> 8);   //  APDUlen High byte
    MBAP[5] = (uint8_t) ((APDUlen + 1) & 0xFF); //  APDUlen Low byte
    MBAP[6] = (uint8_t) (UNIT_ID);              //  Slave ID

    #ifdef
    printf("[TCP] MBAP: ");
    for ( i = 0; i < MBAP_SIZE; i++) printf("%.2x ",MBAP[i]);
    printf("\n");
    #endif
    
    // Send request MBAPDU header (MBAP)
    if (send(sock, MBAP, MBAP_SIZE, 0) < 0)
    {
        printf("[TCP] Error: writing MBAP to socket\n");
        return -1;
    }

    // Send request MBAPDU payload (APDU)
    if (send(sock, APDU, APDUlen, 0) < 0)
    {
        printf("[TCP] Error: writing MBAP to socket\n");
        return -1;
    }

    #ifdef
    printf("[TCP] Message sent to Slave: ");
    for ( i = 0; i < MBAP_SIZE; i++) printf("%.2x ",MBAP[i]);
    for ( i = 0; i < APDUlen; i++) printf("%.2x ",APDU[i]);
    printf("\n");
    #endif

    // Receive response MBAPDU header (MBAP) - fixed size, reusing "MBAP"
    if (recv(sock, MBAP, MBAP_SIZE, 0) < 0)
    {
        printf("[TCP] Error: reading MBAP from socket\n");
        return -1;
    }

    // Receive response MBAPDU payload (APDU_R) - get size from "Lenght" field
    int len = (MBAP[4] << 8) + (MBAP[5]); // Recover lenght from field [4] and [5]
    printf("len: %d\n", len); 

    if (recv(sock, APDU_r, len - 1, 0) < 0)
    {
        printf("[TCP] Error: reading APDU_R from socket\n");
        return -1;
    }

    #ifdef
    printf("[TCP] Message received from slave :");
    for ( i = 0; i < MBAP_SIZE; i++) printf("%.2x ",MBAP[i]);
    for ( i = 0; i < len - 1; i++) printf("%.2x ",APDU[i]);
    printf("\n");
    #endif

    return 0;
}