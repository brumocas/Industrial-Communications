/* Library for close */
#include <unistd.h>

#include "ModbusAP.h"

// 1 byte = 8 bits

#define IP1 "127.0.0.1"
#define IP2 "10.227.113.31"
#define PORT 502

// Debug instructions
//#define DEBUG

int main(){
    
    // Buffer to store the read registers
    // Registers have 16 bits each = 2 bytes
    int16_t A[4]; 
    int16_t B[1]; 
    int16_t C[1];
    
    int check_error;
    int16_t soma[1] = {0x41};

    check_error = write_multiple_regs(IP1, PORT, 121, 1, soma);
    printf("result = %d\n",check_error);
    if (check_error != 0)
    {   
        #ifdef DEBUG
        printf("Error: write_multiple_regs (%d)\n", check_error);
        #endif
        return check_error;
    }

    check_error = read_h_regs(IP1, PORT, 122, 4, A);
    printf("result = %d\n",check_error);
    if (check_error != 0)
    {   
        #ifdef DEBUG
        printf("Error: Reading_h_regs (%d)\n", check_error);
        #endif
        return check_error;
    }

    check_error = read_h_regs(IP1, PORT, 126, 1, B);
    printf("result = %d\n",check_error);
    if (check_error != 0)
    {   
        #ifdef DEBUG
        printf("Error: Reading_h_regs (%d)\n", check_error);
        #endif
        return check_error;
    }

    if (B[0] == 0)
    {
        C[0] = 9999;
    } else{
        C[0] = A[0] + A[3];
    }

    check_error = write_multiple_regs(IP1, PORT, 127, 1, C);
    printf("result = %d\n",check_error);
    if (check_error != 0)
    {   
        #ifdef DEBUG
        printf("Error: write_multiple_regs (%d)\n", check_error);
        #endif
        return check_error;
    }
    
    check_error = write_multiple_regs(IP2, PORT, 127, 1, C);
    printf("result = %d\n",check_error);
    if (check_error != 0)
    {   
        #ifdef DEBUG
        printf("Error: write_multiple_regs (%d)\n", check_error);
        #endif
        return check_error;
    }

    #ifdef DEBUG
    printf("Read Registers: \n");
    printf("SOMA[0] = %d\n", soma[0]);
    for (int i = 0; i < 4; i++)
    printf("A[%d] = %d\n",i , A[i]);
    printf("B[0] = %d\n", B[0]);
    printf("C = %ls\n", C);
    #endif
    
    return 0;
}