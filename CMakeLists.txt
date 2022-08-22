# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(rpi-pico-exploration)



# Copied block from https://github.com/raspberrypi/pico-sdk/blob/master/cmake/pico_pre_load_platform.cmake
# --- COPY BLOCK BEGIN ---

# PICO_CMAKE_CONFIG: PICO_PLATFORM, platform to build for e.g. rp2040/host, default=rp2040 or environment value, group=build
if (DEFINED ENV{PICO_PLATFORM} AND (NOT PICO_PLATFORM))
    set(PICO_PLATFORM $ENV{PICO_PLATFORM})
    message("Using PICO_PLATFORM from environment ('${PICO_PLATFORM}')")
else()
    if (NOT PICO_PLATFORM)
        set(PICO_PLATFORM "rp2040")
        pico_message("Defaulting PICO_PLATFORM to ${PICO_PLATFORM} since not specified.")
    else()
        message("PICO platform is ${PICO_PLATFORM}.")
    endif()
endif ()

# --- COPY BLOCK END ---


# 2022-08-20
zephyr_include_directories(../pico-sdk/src/rp2_common/pico_stdio/include)
zephyr_include_directories(../pico-sdk/src/rp2_common/pico_stdio_uart/include)
zephyr_include_directories(../pico-sdk/src/common/pico_sync/include)
zephyr_include_directories(../pico-sdk/src/common/pico_time/include)

zephyr_include_directories(../modules/hal/rpi_pico/src/host/pico_platform/include)
zephyr_include_directories(../modules/hal/rpi_pico/src/rp2_common/hardware_claim/include)
zephyr_include_directories(../modules/hal/rpi_pico/src/rp2_common/hardware_dma/include) # /hardware/dma.h


target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/thread-led.c)
target_sources(app PRIVATE src/thread-hello-dma.c)

# 2022-08-21 - searching for solution to __printflike(1, 0) error therefore commenting out following target sources stanza:
#  See https://forums.raspberrypi.com/viewtopic.php?t=315944#p1890253

# target_sources(app PRIVATE ../pico-sdk/src/rp2_common/pico_stdio/stdio.c)

## ./src/host/pico_stdio/include/pico/stdio.h


## 2022-08-22 Monday work:

# To provide dma_claim_unused_channel():
zephyr_include_directories(../modules/hal/rpi_pico/src/rp2_common/hardware_dma/include) # /hardware/dma.h
target_sources(app PRIVATE ../modules/hal/rpi_pico/src/rp2_common/hardware_dma/dma.c)

# To provide hardware_claim.h and claim.c respectively:
zephyr_include_directories(../modules/hal/rpi_pico/src/rp2_common/hardware_claim/include)
target_sources(app PRIVATE ../modules/hal/rpi_pico/src/rp2_common/hardware_claim/claim.c)

# To provide pico-sdk panic() routine:
#zephyr_include_directories(../modules/hal/rpi_pico/src/host/pico_platform/include/pico)
#zephyr_include_directories(../modules/hal/rpi_pico/src/host/pico_platform/include)
target_sources(app PRIVATE ../modules/hal/rpi_pico/src/host/pico_platform/platform_base.c)

# Symbol 'PICO_WEAK_FUNCTION_DEF' does not seem to be expanded when gcc compiling code - TMH
# [workspace]/modules/hal/rpi_pico/src/host/pico_platform/include/pico/platform.h




