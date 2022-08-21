# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(rpi-pico-exploration)


# 2022-08-20
zephyr_include_directories(../pico-sdk/src/rp2_common/pico_stdio/include)
zephyr_include_directories(../pico-sdk/src/rp2_common/pico_stdio_uart/include)
zephyr_include_directories(../pico-sdk/src/common/pico_sync/include)
zephyr_include_directories(../pico-sdk/src/common/pico_time/include)


target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/thread-led.c)
target_sources(app PRIVATE src/thread-hello-dma.c)

target_sources(app PRIVATE ../pico-sdk/src/rp2_common/pico_stdio/stdio.c)



