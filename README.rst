RPi Pico Zephyr Explorations
#############################

Overview
********

Project to explore capabilities of RPi Pico development board, a readily available and inexpensive board featuring the RP2040 dual-core Cortex-M0 processor.  The first two non-trivial efforts to exercise Pico-SDK functionality within a Zephyr based app involve bringing up Pico examples for `DMA <https://github.com/raspberrypi/pico-examples/tree/master/dma/hello_dma>`_, and for `DMA driven UART <https://github.com/raspberrypi/pico-examples/tree/master/uart/uart_advanced>`_.

The work in this repository is minimal and something of a code chimera.  Only about a year ago around 2022 June did full device tree support come into the Zephyr RTOS source tree.  RPi Pico SDK has since been integrated under Zephyr Project modules directory `hal_rpi_pico <https://github.com/zephyrproject-rtos/hal_rpi_pico>`_.  Pico SDK examples however are not part of `Zephyr v3.3.0 manifest included projects <https://github.com/zephyrproject-rtos/hal_rpi_pico>`_.

Specific Works Within This Project
**********************************

Work in this tiny project to bring up RPi Pico advanced UART code is an effort to enable interrupt driven or DMA driver UART functions, plus a second UART beyond the default UART called out implicitly by RPi Pico hello_world sample app.  Project need for multiple UARTs goes to the goal to implement a simple command line interface, whose body of code can be instantiated to support multiple CLI sessions in a given firmware application.

A further goal is to decouple this CLI from specific targeted processors, through wrapper API functions which connect to UART or other I/O device to provide read and write byte streams.

A third goal is to design CLI to be scheduler and RTOS agnostic, again decoupled.  In summary the three starting goals for the CLI, which this project aims to test on an RPi Pico dev board first, are:

1.  command line interface (CLI) supporting multiple sessions
2.  CLI decoupled from target processor hardware details
3.  CLI decoupled from presence of RTOS and from any given RTOS

Specific Issues 2023 Q2
************************

To enable RP2040 DMA functionality, and further use of DMA in a UART sample code has required some messy hacking in this project's top level CMakeLists.txt file.  Further, when Pico SDK calls enable a hardware interrupt in the RP2040 Zephyr does not automatically know about these interrupts.  We see this as our Zephyr app crashes early on, complaining of an undefined or unknown interrupt.

This interrupts problem can be solved by use of Zephyr's `IRQ_CONNECT()` macro, but this arrangement of Pico SDK calls and Zephyr macros and calls is far from ideal.  At this early stage we are focused on getting functionalities to work.  As this effort progresses we want to review and improve the hacked code whereever we can find good ways to do so.

Requirements
************

Your board must:

#. Have an LED connected via a GPIO pin (these are called "User LEDs" on many of
   `Zephyr's boards <https://github.com/zephyrproject-rtos/zephyr/tree/main/boards>`_).
#. Have the LED configured using the ``led0`` devicetree alias.

To exercise and work with the command line interface (CLI) you must have some
type of serial interface cable to a computer or terminal device.  An example
cable is an FTDI 3.3 volt USB-to-serial adapter.  On Linux based workstations
FTDI cables when attached to a USB port appear in /dev as devices with names
of the form `ttyUSBn`, where `n` is zero or a positive small integer.

You'll need a terminal emulation program such as `minicom` or similar when
using a laptop or PC to communicate serially with your target board.

Building and Running
********************

Typical build invocation assumes Zephyr toolchain properly installed, and interaction via `west` command line Python based utility:

.. code-block::  shell

    $ west build -b rpi_pico -p always

The board name `rpi_pico` may be substituted with the name of another Zephyr supported target board.

Build errors
************

You will see a build error at the source code line defining the ``struct
gpio_dt_spec led`` variable if you try to build Blinky for an unsupported
board.

On GCC-based toolchains, the error looks like this:

.. code-block:: none

   error: '__device_dts_ord_DT_N_ALIAS_led_P_gpios_IDX_0_PH_ORD' undeclared here (not in a function)

Adding board support
********************

To add support for your board, add something like this to your devicetree:

.. code-block:: DTS

   / {
   	aliases {
   		led0 = &myled0;
   	};

   	leds {
   		compatible = "gpio-leds";
   		myled0: led_0 {
   			gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;
                };
   	};
   };

The above sets your board's ``led0`` alias to use pin 13 on GPIO controller
``gpio0``. The pin flags `GPIO_ACTIVE_HIGH` mean the LED is on when
the pin is set to its high state, and off when the pin is in its low state.

Tips and General References:
****************************

- To learn how to set up Zephyr's toolchain visit https://docs.zephyrproject.org/latest/develop/getting_started/index.html.

- To see what some developers are building using Zephyr and some open hardware dev
  boards, visit Jared Wolff's community forum at https://community.circuitdojo.com/.

- To see which development boards and targeted hardwares are supported by Zephyr
  RTOS, navigate to the boards directory and choose your processor architecture.
  For example when building projects to run on ARM 32-bit microcontrollers the
  boards which current Zephyr RTOS supports appear in https://github.com/zephyrproject-rtos/zephyr/tree/main/boards/arm.

- See https://github.com/zephyrproject-rtos/zephyr/blob/main/dts/bindings/gpio/gpio-controller.yaml
  for the flags you can use in devicetree.

- If the LED is built in to your board hardware, the alias should be defined among
  your boards device tree source files.  Typical file to define all or most board
  features is a file named [board_name].dtsi.  A few examples:

  *  Sparkfun Thing Plus nRF9160 . . . `zephyr/boards/arm/sparkfun_thing_plus_nrf9160/sparkfun_thing_plus_nrf9160_common.dtsi`

  *  Nordic Semi nRF9160         . . . `zephyr/blob/main/boards/arm/nrf9160dk_nrf9160/nrf9160dk_nrf9160_common.dtsi`

  *  RPi Pico board              . . . `https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/rpi_pico/rpi_pico-common.dtsi`

  For a Zephyr based app an LED may be added to a board overlay file, written
  in device tree source as in the code excerpt just before this "Tips" section.
