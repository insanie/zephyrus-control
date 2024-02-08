#include "yaml_parsers.h"
#include "atkacpi_ops.h"
#include "ryzenadj_ops.h"

#define MAIN_CONFIG "config.yaml"
#define FAN_CURVES_CONFIG "fan_curves.yaml"

int main() {
    fan_curves_t fan_curves_config = parse_fan_curves_config(FAN_CURVES_CONFIG);
    config_t main_config = parse_main_config(MAIN_CONFIG);
    set_atkacpi_parameters(main_config.perfmode, main_config.fast, main_config.slow, main_config.apu, main_config.mux);
    set_ryzenadj_parameters(main_config.apu_st, main_config.dgpu_st, main_config.co - 256);
    set_fan_curves(fan_curves_config);
    return 0;
}
