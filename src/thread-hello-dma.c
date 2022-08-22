/*
 *
 *  Project:  RP2040 Exploration
 *
 *     File:  thread-hello-dma.c
 *
 */



// newlibc headers:
#include <stdint.h>                // to provide define of uint32_t

// Zephyr RTOS headers:
#include <zephyr.h>

// RaspberryPi Pico SDK headers:  ( see ../notes/pico-stdio-h-include-notes.txt for reason to use pico/ relative path )
#include <pico/stdio.h>
#include <pico/stdio_uart.h>
#include <hardware/dma.h>          // CMakeLists.txt adds zephyr_include_directory:
                                   // <s> ../modules/hal/rpi_pico/src/rp2_common/hardware_dma/include/hardware </s>
                                   // ../modules/hal/rpi_pico/src/rp2_common/hardware_dma/include

// 2022-08-22 MON
#include <pico/platform.h> . . . are we including this file?  Build process reports no error
// finding it yet PICO_WEAK_FUNCTION_DEF seems not to expand per its def in platform.h - TMH
//#include <platform.h>

// app headers:
#include "thread-hello-dma.h"
#include "app-return-values.h"



//----------------------------------------------------------------------
// - SECTION - defines
//----------------------------------------------------------------------

// Defines for thread attributes required by Zephyr RTOS:
#define THREAD_HELLO_DMA__PRIORITY (7)
#define THREAD_HELLO_DMA__STACK_SIZE 512

// Defines to aid thread ID by app and periodic thread execution, not Zephyr requirements:
#define MODULE_ID__THREAD_HELLO_DMA "thread-hello-dma"

#define SLEEP_TIME__THREAD_HELLO_DMA__MS (1000)



//----------------------------------------------------------------------
// - SECTION - file scoped
//----------------------------------------------------------------------

static uint32_t sleep_period__thread_hello_dma__fsv = SLEEP_TIME__THREAD_HELLO_DMA__MS;



//----------------------------------------------------------------------
// - SECTION - routines
//----------------------------------------------------------------------

K_THREAD_STACK_DEFINE(thread_hello_dma__stack_area, THREAD_HELLO_DMA__STACK_SIZE);

struct k_thread thread_hello_dma__thread_data;

int thread_hello_dma__initialize(void)
{
    uint32_t rstatus = 0;

    k_tid_t thread_hello_dma__tid = k_thread_create(&thread_hello_dma__thread_data, thread_hello_dma__stack_area,
                                            K_THREAD_STACK_SIZEOF(thread_hello_dma__stack_area),
                                            thread_hello_dma__entry_point,
                                            NULL, NULL, NULL,
                                            THREAD_HELLO_DMA__PRIORITY,
                                            0,
                                            K_MSEC(1000)); // K_NO_WAIT);

    rstatus = k_thread_name_set(thread_hello_dma__tid, MODULE_ID__THREAD_HELLO_DMA);
    if ( rstatus == 0 ) { } // avoid compiler warning about unused variable - TMH

    sleep_period__thread_hello_dma__fsv = SLEEP_TIME__THREAD_HELLO_DMA__MS;

    return (int)thread_hello_dma__tid;
}



void thread_hello_dma__entry_point(void* arg1, void* arg2, void* arg3)
{
    uint32_t rstatus = ROUTINE_OK;

    if ( rstatus == 0 ) { }

// 2022-08-20 - First call to Pico SDK library:
//    stdio_init_all();

// From pico-examples/dma/hello_dma/hello_dma.c:
    // Get a free channel, panic() if there are none
    int chan = dma_claim_unused_channel(true);

    while ( 1 )
    {
        k_msleep(sleep_period__thread_hello_dma__fsv);
    }
}




// --- EOF ---
