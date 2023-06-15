# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

## Note some cmake functions must be called before cmake project() directive - TMH
project(rpi-pico-exploration)


##----------------------------------------------------------------------
## - SECTION - application files
##----------------------------------------------------------------------

# 2023-06-14 NOTE, following `modules/hal/rpi_pico` paths support header
#  file dependencies of pulled in `uart_advanced.c` source file:

target_include_directories(app PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../modules/hal/rpi_pico/src/rp2_common/hardware_irq/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../modules/hal/rpi_pico/src/common/pico_stdlib/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../modules/hal/rpi_pico/src/rp2_common/pico_stdio/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../modules/hal/rpi_pico/src/common/pico_time/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../modules/hal/rpi_pico/src/common/pico_sync/include"
)

# ./modules/hal/rpi_pico/src/rp2_common/hardware_irq/include/hardware/irq.h

target_sources(app PRIVATE
  src/main.c
  src/thread-led.c
  src/thread-hello-dma.c
  src/local_rpi_platform_base.c
  src/thread-simple-cli.c
  src/commands.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/hal/rpi_pico/src/rp2_common/hardware_irq/irq.c
#  src/uart_advanced.c
  src/thread-uart-advanced.c
)


##----------------------------------------------------------------------
## - SECTION - RPi Pico SDK dependencies
##----------------------------------------------------------------------

# ../modules/hal/rpi_pico/src/host/pico_platform/include/pico/platform.h

# 2023-04-19 Note second, third and fourth paths inform build system
#  where Pico SDK pound includes can find their expressed header files:

target_include_directories(app PRIVATE
  "../modules/hal/rpi_pico/src/"
  "../modules/hal/rpi_pico/src/rp2_common/hardware_dma/include/"
  "../modules/hal/rpi_pico/src/rp2_common/hardware_claim/include/"
  "../modules/hal/rpi_pico/src/host/pico_platform/include/pico/"
)

# ../modules/hal/rpi_pico/src/rp2_common/hardware_dma/dma.c

# Pico SDK source file platform_base.c provides:  void panic(const char *fmt, ...) {

cmake_path(SET RPI_HARDWARE_ABSTRACTION_LAYER_DIR "../modules/hal/rpi_pico")

target_sources(app PRIVATE
  "${RPI_HARDWARE_ABSTRACTION_LAYER_DIR}/src/rp2_common/hardware_dma/dma.c"
  "${RPI_HARDWARE_ABSTRACTION_LAYER_DIR}/src/rp2_common/hardware_claim/claim.c"
)

#  "${RPI_HARDWARE_ABSTRACTION_LAYER_DIR}/src/host/pico_platform/platform_base.c"




