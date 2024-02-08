#ifndef ZEPHYRUS_CONTROL_ATKACPI_OPS_H
#define ZEPHYRUS_CONTROL_ATKACPI_OPS_H

void set_fan_curves(fan_curves_t fan_curves);
void set_atkacpi_parameters(unsigned char perfmode, unsigned char fast, unsigned char slow, unsigned char apu, unsigned char mux);

#endif
