#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "yaml_parsers.h"

fan_curves_t parse_fan_curves_config(char* filename) {
    printf("Loading %s...\n", filename);
    FILE* config_fd = fopen(filename, "r");
    if (!config_fd) {
        perror("Failed to open config");
        exit(EXIT_FAILURE);
    }
    yaml_parser_t parser;
    yaml_event_t event;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize YAML parser\n");
        exit(EXIT_FAILURE);
    }
    yaml_parser_set_input_file(&parser, config_fd);
    fan_curves_t parsed_config;
    for (int i = 0; i < 16; i++) {
        parsed_config.cpu[i] = 255;
        parsed_config.gpu[i] = 255;
    }
    unsigned char (*member_p)[16] = NULL;
    int value_index = 0;
    while (1) {
        if (!yaml_parser_parse(&parser, &event)) {
            fprintf(stderr, "Parser error %d\n", parser.error);
            exit(EXIT_FAILURE);
        }
        if (event.type == YAML_SCALAR_EVENT) {
            char *current_value = (char*)event.data.scalar.value;
            if (strcmp(current_value, "cpu") == 0) {
                member_p = &parsed_config.cpu;
                value_index = 0;
            } else if (strcmp(current_value, "gpu") == 0) {
                member_p = &parsed_config.gpu;
                value_index = 0;
            } else if (member_p != NULL) {
                (*member_p)[value_index] = atoi(current_value);
                value_index++;
            }
        }
        if (event.type == YAML_STREAM_END_EVENT) {
            break;
        }
        yaml_event_delete(&event);
    }
    yaml_parser_delete(&parser);
    fclose(config_fd);
    return parsed_config;
}

config_t parse_main_config(char* filename) {
    printf("Loading %s...\n", filename);
    FILE *config_fd = fopen(filename, "r");
    if (!config_fd) {
        perror("Failed to open config");
        exit(EXIT_FAILURE);
    }
    yaml_parser_t parser;
    yaml_event_t event;
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize YAML parser\n");
        exit(EXIT_FAILURE);
    }
    yaml_parser_set_input_file(&parser, config_fd);
    config_t parsed_config = {
            .perfmode = 255,
            .fast = 255,
            .slow = 255,
            .apu = 255,
            .apu_st = 255,
            .dgpu_st = 255,
            .co = 1,
            .mux = 255
    };
    unsigned char *member_p = NULL;
    while (1) {
        if (!yaml_parser_parse(&parser, &event)) {
            fprintf(stderr, "Parser error %d\n", parser.error);
            exit(EXIT_FAILURE);
        }
        if (event.type == YAML_SCALAR_EVENT) {
            char *current_value = (char*)event.data.scalar.value;
            if (strcmp(current_value, "perfmode") == 0)     { member_p = &parsed_config.perfmode; }
            else if (strcmp(current_value, "fast") == 0)    { member_p = &parsed_config.fast; }
            else if (strcmp(current_value, "slow") == 0)    { member_p = &parsed_config.slow; }
            else if (strcmp(current_value, "apu") == 0)     { member_p = &parsed_config.apu; }
            else if (strcmp(current_value, "apu_st") == 0)  { member_p = &parsed_config.apu_st; }
            else if (strcmp(current_value, "dgpu_st") == 0) { member_p = &parsed_config.dgpu_st; }
            else if (strcmp(current_value, "co") == 0)      { member_p = &parsed_config.co; }
            else if (strcmp(current_value, "mux") == 0)     { member_p = &parsed_config.mux; }
            else if (member_p != NULL)                      { *member_p = atoi(current_value); }
        }
        if (event.type == YAML_STREAM_END_EVENT) {
            break;
        }
        yaml_event_delete(&event);
    }
    yaml_parser_delete(&parser);
    fclose(config_fd);
    return parsed_config;
}
