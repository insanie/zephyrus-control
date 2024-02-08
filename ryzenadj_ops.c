#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "ryzenadj.h"

void set_ryzenadj_parameters(int apu_st, int dgpu_st, int co) {
    HMODULE ryzenadj_lib = LoadLibraryA("libryzenadj.dll");
    if (!ryzenadj_lib) {
        fprintf(stderr, "Failed to load libryzenadj.dll\n");
        exit(1);
    }
    ryzen_access ryzenadj = init_ryzenadj();
    while (!ryzenadj) {
        fprintf(stderr, "Failed to init ryzenadj\n");
        cleanup_ryzenadj(ryzenadj);
        Sleep(200);
        ryzenadj = init_ryzenadj();
    }
    init_table(ryzenadj);

    if (apu_st >= 15 && apu_st <= 50) {
        set_apu_skin_temp_limit(ryzenadj, (uint32_t)apu_st);
        printf("APU ST   -> %d\370C\n", apu_st);
    } else if (apu_st == 255) {
        printf("APU ST   -> [NOT SET]\n");
    } else {
        printf("APU ST   -> [OUT OF BOUNDS]\n");
    }

    if (dgpu_st >= 15 && dgpu_st <= 55) {
        set_dgpu_skin_temp_limit(ryzenadj, (uint32_t)dgpu_st);
        printf("DGPU ST  -> %d\370C\n", dgpu_st);
    } else if (dgpu_st == 255) {
        printf("DGPU ST  -> [NOT SET]\n");
    } else {
        printf("DGPU ST  -> [OUT OF BOUNDS]\n");
    }

    if (co <= 0 && co >= -50) {
        set_coall(ryzenadj, (uint32_t)co);
        printf("CO       -> %d\n", co);
    } else if (co == -255) {
        printf("CO       -> [NOT SET]\n");
    } else {
        printf("CO       -> [OUT OF BOUNDS]\n");
    }

    cleanup_ryzenadj(ryzenadj);
    FreeLibrary(ryzenadj_lib);
}
