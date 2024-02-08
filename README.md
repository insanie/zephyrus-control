# zephyrus-control

The application aims to replicate ASUS Armoury Crate functionality without all bells and whistles that sometimes actually
cripple user experience and/or even performance. It grants the end user granular control over Zephyrus G14 2022 (GA402) laptop
hardware in a lightweight, streamlined way with no strings attached. No random registry entries. No hidden configs. No
custom power plans. No weird business. The code is simple and de-obfuscated.

## Compatibility

Initially only GA402 is supported but this approach may be applied to other ROG/TUF devices using AMD APU+dGPU.
You can play around with different device codes from linux kernel in Credits section. While fan devices remain the same
for all ROG/TUF laptops, power limits devices can vary across AMD/Intel/Nvidia devices. And obviously, ryzenadj calls won't
work for Intel devices.

## Disclaimer

The application is shipped with config files that are 100% safe to use. However, even though there are some safety
mechanisms built into the application (that won't let you apply gibberish values) you can still harm your device, e.g.
though applying big power limits knowing your cooling system won't handle it well, and so on. Please be careful with
selecting values for parameters.

## Requirements
* Windows
* Disabled/deleted Armoury Crate app and services (it will try to override values)
* ROG/TUF (may be other ASUS laptops?) laptop for general fan curve functionality
* AMD APU for power limits, skin temp, mux and CO functionality

There's a chance some pieces of app will work for AMD APU + Nvidia dGPU. For Intel most probably you'll have to change
some power limit devices.

## Installation

1. Extract downloaded archive into any folder you see fit
2. Open `config.yaml` and `fan_curves.yaml` and verify all the parameters you want to set. Inside you'll find extensive
comments on how to select those
3. Run `zephyrus_control.exe` to see similar output:
```
Loading fan_curves.yaml...
Loading config.yaml...
PERFMODE -> MANUAL
MUX      -> [NOT SET]
FAST     -> 120 W
SLOW     -> 120 W
APU      -> 45 W
APU ST   -> 40°C
DGPU ST  -> 50°C
CO       -> -3
                                        ▄
CPU                           ▄    █    █
FAN                      █    █    █    █
                    █    █    █    █    █
     █    █    █    █    █    █    █    █
    30°C 40°C 50°C 60°C 70°C 80°C 90°C 100°C
    20 % 20 % 25 % 40 % 60 % 75 % 85 % 90 %
                                        ▄
GPU                           ▄    █    █
FAN                      █    █    █    █
                    █    █    █    █    █
     █    █    █    █    █    █    █    █
    30°C 40°C 50°C 60°C 70°C 80°C 90°C 100°C
    20 % 20 % 25 % 40 % 60 % 75 % 85 % 90 %
```
(Optional) you can schedule application execution through Task Scheduler. It is advised to disable all ASUS services.

## Reverting back

In case you applied some incorrect values and want them back, you can simply re-run the application with new values. If
re-run is not an option, keep in mind the following:
* Performance mode, power limits and mux status are persistent after laptop restart. Either re-run the app with new
  values or select a new performance mode
* Skin temperature limits and curve optimizer are to be written to SMU and will always be reverted back to default after restart

## Build

1. Clone the repo
2. Install `libyaml` and add it to CMake
3. Build

## Dependencies

* ATKACPI driver, installed by Windows Update automatically
* libyaml, has to be installed separately
* libryzenadj, shipped with the repo

## Credits

* https://github.com/FlyGoat/RyzenAdj
* Linux kernel, device codes specifically - https://github.com/torvalds/linux/blob/047371968ffc470769f541d6933e262dc7085456/include/linux/platform_data/x86/asus-wmi.h#L94
