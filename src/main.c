/*
 *  Copyright (c) 2023 Neela Nurseries
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Project:  https://github.com/tedhavelka/rpi-pico-zephyr-exploration
 *
 *  File:  main.c
 */



//----------------------------------------------------------------------
// - SECTION - includes, NEED to review line by line which are needed - TMH
//----------------------------------------------------------------------

#include <errno.h>
#include <string.h>
#define LOG_LEVEL 4
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>

#include <zephyr/drivers/led_strip.h>   // to provide ...

// Pico SDK headers:
// Following header from ../modules/hal/rpi_pico/src/rp2040/hardware_regs/include/hardware/regs/intctrl.h
#include "hardware/regs/intctrl.h" // to provide UART0_IRQ

// Following Pico-SDK header provides several low level, static in-line functions:
#include "hardware/uart.h"         // to provide uart_is_readable, uart_getc, uart_is_writable, uart_putc

// 2022-08-04 added for simple factoring during early UART tests:
#include "main.h"
#include "development-defines.h"
#include "thread-led.h"
#include "thread-hello-dma.h"
#include "thread-simple-cli.h"
#include "thread-uart-advanced.h"
#include "uart-advanced.h"



//----------------------------------------------------------------------
// - SECTION - defines
//----------------------------------------------------------------------

// 1000 msec = 1 sec
#define SLEEP_TIME_MS 15000

// The devicetree node identifier for the "led0" alias.
#define LED0_NODE DT_ALIAS(led0)

// Following symbol sets a modest upper bound on periodic "mark" message, akin to heartbeat LED:
#define MARK_CYCLE_LENGTH 6

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */

#ifdef DEV_0805_KEEP_BLINKY_CODE_ENABLED
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#endif

#ifdef DEV_0805__WS2812_BRING_UP_WORK_ON_RP2040

// --- DEV BEGIN :: WS2812 test ---
//
// Source file:  ~/projects-sandbox/workspace-for-rpi/zephyr/samples/drivers/led_ws2812/src/main.c

#define STRIP_NODE              DT_ALIAS(led_strip)
#define STRIP_NUM_PIXELS        DT_PROP(DT_ALIAS(led_strip), chain_length)

#define DELAY_TIME K_MSEC(50)

#define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }

static const struct led_rgb colors[] = { 
        RGB(0x0f, 0x00, 0x00), /* red */
        RGB(0x00, 0x0f, 0x00), /* green */
        RGB(0x00, 0x00, 0x0f), /* blue */
};

struct led_rgb pixels[STRIP_NUM_PIXELS];

static const struct device *strip = DEVICE_DT_GET(STRIP_NODE);

// --- DEV END :: WS2812 test ---
#endif // DEV_0805__WS2812_BRING_UP_WORK_ON_RP2040

static bool global_flag_fsv = 0;


//----------------------------------------------------------------------
// - SECTION - routines
//----------------------------------------------------------------------

extern int main_for_advanced_uart_init(void);

// From Pico SDK sample:
static int chars_rxed = 0;

static void rpi_pico_uart1_isr(const struct device *dev)
{
    ARG_UNUSED(dev);

    global_flag_fsv = 1;

#define UART_ID uart1
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// From Pico SDK sample:
    while (uart_is_readable(UART_ID))
    {
        uint8_t ch = uart_getc(UART_ID);

        // Can we send it back?
        if (uart_is_writable(UART_ID))
        {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
}

void main(void)
{
    int rstatus = 0;

#if 1 //
    uint32_t count_for_mark_messages = 0;
    char lbuf[SIZE_OF_TEN_BYTES] = { 0 };

    if (!device_is_ready(led.port)) {
        return;
    }

    rstatus = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (rstatus < 0) {
        return;
    }

    memset(lbuf, 0, SIZE_OF_TEN_BYTES);

// NEED to capture return value from this routine call:
    rstatus = thread_led__initialize();

#ifdef DEV_0819_ENABLE_PICO_HELLO_DMA_CODE
    thread_hello_dma__initialize();
#endif

#if 1 // - DEV 0613 -
    rstatus |= initialize_thread_simple_cli();
    k_msleep(500);
#endif // - DEV 0613 -

#if 1
#define DEV_0615_DEFAULT_PRIORITY 1
#define UART_NODE DT_NODELABEL(uart_0)
    printk("- main.c - calling IRQ_CONNECT(), will send mark after call . . .\n");
    IRQ_CONNECT(UART1_IRQ, DEV_0615_DEFAULT_PRIORITY, rpi_pico_uart1_isr, DEVICE_DT_GET(UART_NODE), 0);
    printk("- MARK 0 -\n");
    k_msleep(1500);
#endif
#if 0
    printk("- DEV 0615 - symbols UART0_IRQ, UART1_IRQ assigned valueos of %u, %u\n",
      UART0_IRQ, UART1_IRQ);
#endif

// - DEV 0614 -
    printk("- main.c - calling Pico UART DMA sample code . . .\n");
    main_for_advanced_uart_init();
    k_msleep(500);
    printk("- MARK 1 -\n");
//    initialize_thread_uart_advanced();  . . . build time error regarding conflicting defines of arch_irq_is_enabled or similar
// - DEV 0614 -


    while (1)
    {
#ifdef DEV_0808__BLINK_FROM_MAIN_NOT_FROM_THREAD
        ret = gpio_pin_toggle_dt(&led);

        if (ret < 0) {
            return;
        }
#endif

        if ( count_for_mark_messages < MARK_CYCLE_LENGTH )
        {
            count_for_mark_messages++;
        }
        else
        {
            count_for_mark_messages = 1;
            memset(lbuf, 0, SIZE_OF_TEN_BYTES);
        }
         
        memset(lbuf, '.', count_for_mark_messages);

        printk("- MARK 0614 -");
        printk("%s", lbuf);
        printk("\n\r");

        k_msleep(SLEEP_TIME_MS);
    }
#endif

#ifdef DEV_0805__WS2812_BRING_UP_WORK_ON_RP2040
    size_t cursor = 0, color = 0;
    int rc;

    if (device_is_ready(strip)) {
        LOG_INF("Found LED strip device %s", strip->name);
    } else {
        LOG_ERR("LED strip device %s is not ready", strip->name);
        return;
    }

    LOG_INF("Displaying pattern on strip");
    while (1) {
        memset(&pixels, 0x00, sizeof(pixels));
        memcpy(&pixels[cursor], &colors[color], sizeof(struct led_rgb));
        rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);

        if (rc) {
            LOG_ERR("couldn't update strip: %d", rc);
        }

        cursor++;
        if (cursor >= STRIP_NUM_PIXELS) {
            cursor = 0;
            color++;
            if (color == ARRAY_SIZE(colors)) {
                color = 0;
            }
        }

        k_msleep(SLEEP_TIME_MS);
    }
#endif // DEV_0805__WS2812_BRING_UP_WORK_ON_RP2040

}



// --- EOF ---
