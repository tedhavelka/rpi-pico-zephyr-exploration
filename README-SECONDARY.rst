# rpi-pico-zephyr-exploration

Reference to Zephyr SDK set up instructions, as of 2022-07-31 for Zephyr SDK v0.14.2:

*  https://docs.zephyrproject.org/latest/develop/getting_started/index.html


- Overview -

This Zephyr RTOS based project targets the RaspberryPi RP2040 microcontroller, also known by 'MCU'.  The code is based on Zephyr RTOS release 3.1.0 and its "hello world" sample app.  While this project builds using primarily the rpi_pico board's device tree file set, as available in this later-in-2022 release of Zephyr, original project contributors flash and run this code on a Sparkfun DEV-18288 development kit.

No matter, what we learn early on is that while DEV-18288 comes with a WS2812 multi-color programmable LED, the device tree sources for the RP2040 in Zephyr's dts files is missing both SPI and DMA device nodes.  For this reason the primary goal of this project is to amend the file rp2040.dts with these nodes.  As of 2022-08-16 this device tree file is located in Zephyr RTOS project repo at:


   *  https://github.com/zephyrproject-rtos/zephyr/blob/main/dts/arm/rpi_pico/rp2040.dtsi


  Second goal is to craft wrapper functions about RPi Pico SDK SPI bus driver.



- EOF -
