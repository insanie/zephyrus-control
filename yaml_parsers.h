#ifndef ZEPHYRUS_CONTROL_YAML_PARSERS_H
#define ZEPHYRUS_CONTROL_YAML_PARSERS_H

typedef struct {
    unsigned char cpu[16];
    unsigned char gpu[16];
} fan_curves_t;

typedef struct {
    unsigned char perfmode;
    unsigned char fast;
    unsigned char slow;
    unsigned char apu;
    unsigned char apu_st;
    unsigned char dgpu_st;
    unsigned char co;
    unsigned char mux;
} config_t;

fan_curves_t parse_fan_curves_config(char* filename);
config_t parse_main_config(char* filename);

#endif
