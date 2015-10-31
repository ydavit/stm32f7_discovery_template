
=== Introduction ===
This template is for building STM32F746G-discovery applications under Linux
using gcc.

Main work is comming frmo https://github.com/jeremyherbert/stm32-templates

Instructions how to install openocd can be found here:
http://www.carminenoviello.com/en/2015/07/13/started-stm32f746g-disco/

GCC for Arm with Cortex-M7 support can be downloaded from here:
https://launchpad.net/gcc-arm-embedded

The official STM32CUBEF7 package can be downloaded from here:
http://www.st.com/web/en/catalog/tools/PF261909

The UDEV rules for the F7 discovery board is at utils/49-stlinkv2.rules.

=== Usage ===

arm-none-eabi-gcc along with other gcc tools should be in the PATH.

make - compiles the project
make flash - uploads the binary to the board

