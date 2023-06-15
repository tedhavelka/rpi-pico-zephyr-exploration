

//----------------------------------------------------------------------
// - SECTION - includes
//----------------------------------------------------------------------

//#include <string.h>
#include <stdint.h>                // to provide define of uint32_t

// Zephyr RTOS related:
#include <zephyr/kernel.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"



//----------------------------------------------------------------------
// - SECTION - defines
//----------------------------------------------------------------------

#define THREAD_UART_ADVANCED_PRIORITY 6
#define THREAD_UART_ADVANCED_STACK_SIZE 1024


//----------------------------------------------------------------------
// - SECTION - globals
//----------------------------------------------------------------------

struct k_thread thread_uart_advanced__thread_data;

void thread_uart_advanced_entry_point(void* arg1, void* arg2, void* arg3);



//----------------------------------------------------------------------
// - SECTION - code borrowed from RPi Pico 'advanced uart' sample
//----------------------------------------------------------------------

#define UART_ID uart1
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 4 // 0
#define UART_RX_PIN 5 // 1

static int chars_rxed = 0;

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // Can we send it back?
        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }   
}


//----------------------------------------------------------------------
// - SECTION - routines
//----------------------------------------------------------------------

K_THREAD_STACK_DEFINE(thread_uart_advanced_stack_area, THREAD_UART_ADVANCED_STACK_SIZE);

int initialize_thread_uart_advanced(void)
{
//    uint32_t rstatus = 0;

    k_tid_t thread_uart_advanced_tid = k_thread_create(&thread_uart_advanced__thread_data, thread_uart_advanced_stack_area,
                                         K_THREAD_STACK_SIZEOF(thread_uart_advanced_stack_area),
                                         thread_uart_advanced_entry_point,
                                         NULL, NULL, NULL,
                                         THREAD_UART_ADVANCED_PRIORITY,
                                         0,
                                         K_MSEC(1000)); // K_NO_WAIT);

//    rstatus = k_thread_name_set(thread_uart_advanced__tid, MODULE_ID__THREAD_LED);
//    if ( rstatus == 0 ) { } // avoid compiler warning about unused variable - TMH

    return (int)thread_uart_advanced_tid;
}



void thread_uart_advanced_entry_point(void* arg1, void* arg2, void* arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

#define THREAD_UART_ADV_SLEEP_IN_MS 6000
    unsigned int sleep_in_ms = THREAD_UART_ADV_SLEEP_IN_MS;

    printk("- thread_uart_advanced - entry point routine reached,\n");
    printk("( will print 'MARK' periodically )\n");


//**********************************************************************
//**********************************************************************
// Code borrowed from RPi Pico 'advanced UART' demo:

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "\nHello, uart interrupts\n");

//    while (1)
//        tight_loop_contents();

//**********************************************************************
//**********************************************************************

    while (1)
    {
        k_msleep(sleep_in_ms);

        printk("- MARK - Pico based UART thread running\n");
    }
}
