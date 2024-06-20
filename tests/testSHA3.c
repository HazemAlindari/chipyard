//All rights reserved by Hazem Al Indari
//FIXME: change the test to sha3
#include "mmio_FPGA.h"
#include "mmio.h"
#include <stdio.h>

#define REF0 0xe7952b64fb7cbeb0
#define REF1 0x2477f8f167faf6a0
#define REF2 0x65189a4adf50ce7e
#define REF3 0x9fa3c18e6ac0d64e
#define REF4 0x89aecc35f6fc676b
#define REF5 0x54b41685e882df5b
#define REF6 0x1e95ca18a902a48b
#define REF7 0xa6df70f930cf2683

//FPGA test
int main(void)
{
    uint64_t output[8], ref[8];
    reset_FPGA();

    uint8_t input[] = {0b01010100 ,0b01101000 ,0b01100101 ,0b00100000 ,0b01110001 ,0b01110101 ,0b01101001 ,0b01100011, 0b01101011, 0b00101110};
    int input0Size = sizeof(input) / sizeof(input[0]);

    reset_FPGA();
    
    // wait for peripheral to be ready
    wait_for_inputs_receive_ready_FPGA();

    //riting inputs to buffer
    if ((write_input0_FPGA(input, input0Size))) {
        printf("Error writing input0\n");
        return 1;
    }
    
    // wait for peripheral to complete
    wait_for_output_ready_FPGA();

    read_outputs_FPGA(output);
    ref[0] = REF0;
    ref[1] = REF1;
    ref[2] = REF2;
    ref[3] = REF3;
    ref[4] = REF4;
    ref[5] = REF5;
    ref[6] = REF6;
    ref[7] = REF7;

    int noMatch = 0;
    for (int i = 0; i < 8; i++) {
        if (output[i] != ref[i]) {
            printf("Mismatch at index %d\n", i);
            noMatch = 1;
            break;
        }
    }
    if (noMatch) {
        printf("Hardware result is incorrect for sha3\n");
        printf("Expected: %#lx %#lx %#lx %#lx", ref[7], ref[6], ref[5], ref[4]);
        printf(" %#lx %lx %lx %lx\n", ref[0], ref[1], ref[2], ref[3]);
        printf("Got back: %#lx %#lx %#lx %#lx", output[7], output[6], output[5], output[4]);
        printf(" %#lx %lx %lx %lx\n", output[0], output[1], output[2], output[3]);
    } else {
        printf("Hardware result is correct for sha3\n");
        printf("Got back: %#lx %#lx %#lx %#lx", output[7], output[6], output[5], output[4]);
        printf(" %#lx %lx %lx %lx\n", output[0], output[1], output[2], output[3]);
    }

    return 0;
}
