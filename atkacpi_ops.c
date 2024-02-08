/*

 ATKACPI receives a buffer of the following structure:
 |----| | |----| | |----| | |~
  CMD   |  SIZE  |  DEV   |  VALUE
  4B    |  4B    |  4B    |

 First dword is for ATKACPI command - read or write value of a device
 Second dword is for the size of the payload, which is third dword + the rest
 Third dword is ID of the device to control, e.g. fan, perfmode, power limit, mux...
 Fourth is the value that needs to be applied to the device set in third dword, or empty for read command

*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "yaml_parsers.h"

#define ATKACPI_CONTROL_CODE 2237452
#define BUFFER_SIZE 28
#define PAYLOAD_SIZE_INDEX 4

char *PERFMODES[] = {"BALANCED","TURBO","SILENT","PERFORMANCE","MANUAL"};
unsigned char ATKACPI_WRITE[] = {68,69,86,83};
unsigned char FAN_DEVICE_TEMPLATE[] = {0,0,17,0};
unsigned char FAN_DEVICE_ID[] = {36,37};
unsigned char PERFMODE_DEVICE[] = {117,0,18,0};
unsigned char FAST_LIMIT_DEVICE[] = {193,0,18,0};
unsigned char SLOW_LIMIT_DEVICE[] = {160,0,18,0};
unsigned char APU_LIMIT_DEVICE[] = {176,0,18,0};
unsigned char MUX_DEVICE[] = {22,0,9,0};

void write_atkacpi(unsigned char device[4], unsigned char *value, int value_length) {
    HANDLE atkacpi_h = CreateFile("\\\\.\\ATKACPI", GENERIC_READ | GENERIC_WRITE,
                                  0, NULL, OPEN_EXISTING,
                                  0, NULL);
    if (atkacpi_h == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "ATKACPI device opening error %lu\n", GetLastError());
        exit(EXIT_FAILURE);
    }
    unsigned char in_buffer[BUFFER_SIZE] = {0};
    memcpy(in_buffer + 0, ATKACPI_WRITE, 4);      // Setting command as 1st dword
    in_buffer[PAYLOAD_SIZE_INDEX] = value_length + 4;          // Setting length of payload as 2nd dword
    memcpy(in_buffer + 8, device, 4);             // Setting device as 3rd dword
    memcpy(in_buffer + 12, value, value_length);  // Setting value as 4+ dword
    DWORD returned_size;
    DeviceIoControl(atkacpi_h, ATKACPI_CONTROL_CODE, in_buffer, BUFFER_SIZE,
                    NULL, 0, &returned_size, NULL);
    CloseHandle(atkacpi_h);
}

void set_fan_curves(fan_curves_t fan_curves) {
    for (int i = 0; i < 2; i++) {
        int skip = 0;
        for (int j = 0; j < 16; j++) {
            int value = i == 0 ? fan_curves.cpu[j] : fan_curves.gpu[j];
            if (value < 0 || value > 100 ) {
                printf("%s fan curve not set or out of bounds!\n", i == 0 ? "CPU" : "GPU");
                skip = 1;
                break;
            }
        }
        if (skip == 1) { continue; }

        unsigned char fan_device[4];
        memcpy(fan_device + 0, FAN_DEVICE_TEMPLATE, 4);
        fan_device[0] = FAN_DEVICE_ID[i]; // CPU and GPU fan devices differ in this byte only
        write_atkacpi(fan_device, i == 0 ? fan_curves.cpu : fan_curves.gpu, 16);

        // Output curve graph...
        for (int j = 80; j >= 0; j -= 20) {
            if (j == 60) { printf("%s", i == 0 ? "CPU" : "GPU"); }
            else if (j == 40) { printf("FAN"); }
            else { printf("   "); }
            for (int k = 0; k < 8; k++) {
                unsigned char (*curve_p)[16] = i == 0 ? &fan_curves.cpu : &fan_curves.gpu;
                unsigned char symbol = (*curve_p)[k+8]-j > 15 ? '\333' : '\334';
                symbol = (*curve_p)[k+8]-j < 6 ? ' ' : symbol;
                printf("  %c  %s", symbol, k == 7 ? "\n" : "");
            }
        }
        // ...and curve values
        printf("   ");
        for (int j = 0; j < 16; j++) {
            printf(" %d%s%s", i == 0 ? fan_curves.cpu[j] : fan_curves.gpu[j], j < 8 ? "\370C" : " %", j == 7 ? "\n   " : "");
        }
        printf("\n");
    }
}

void set_atkacpi_parameters(unsigned char perfmode, unsigned char fast, unsigned char slow, unsigned char apu, unsigned char mux) {
    if (perfmode >= 0 && perfmode <= 4) {
        write_atkacpi(PERFMODE_DEVICE, &perfmode, 1);
        printf("PERFMODE -> %s\n", PERFMODES[perfmode]);
    } else if (perfmode == 255) {
        printf("PERFMODE -> [NOT SET]\n");
    } else {
        printf("PERFMODE -> [OUT OF BOUNDS]\n");
    }

    if (mux == 0 || mux == 1) {
        write_atkacpi(MUX_DEVICE, &mux, 1);
        printf("MUX      -> %s\n", mux == 0 ? "ENABLED" : "DISABLED");
    } else if (mux == 255) {
        printf("MUX      -> [NOT SET]\n");
    } else {
        printf("MUX      -> [OUT OF BOUNDS]\n");
    }

    if (fast >= 15 && fast <= 155) {
        write_atkacpi(FAST_LIMIT_DEVICE, &fast, 1);
        printf("FAST     -> %d W\n", fast);
    } else if (fast == 255) {
        printf("FAST     -> [NOT SET]\n");
    } else {
        printf("FAST     -> [OUT OF BOUNDS]\n");
    }

    if (slow >= 15 && slow <= 155) {
        write_atkacpi(SLOW_LIMIT_DEVICE, &slow, 1);
        printf("SLOW     -> %d W\n", slow);
    } else if (slow == 255) {
        printf("SLOW     -> [NOT SET]\n");
    } else {
        printf("SLOW     -> [OUT OF BOUNDS]\n");
    }

    if (apu >= 10 && apu <= 100) {
        write_atkacpi(APU_LIMIT_DEVICE, &apu, 1);
        printf("APU      -> %d W\n", apu);
    } else if (apu == 255) {
        printf("APU      -> [NOT SET]\n");
    } else {
        printf("APU      -> [OUT OF BOUNDS]\n");
    }
}
