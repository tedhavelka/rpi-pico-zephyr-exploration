/**
 *  Project:  RP2040 Exploration
 *     File:  thread-hello-dma.c
 */



// newlibc headers:
#include <stdint.h>                // to provide define of uint32_t
#include <stdio.h>                 // to provide puts() C function

// Zephyr RTOS headers:
#include <zephyr/kernel.h>

// Pico SDK headers:
#include <rp2_common/hardware_dma/include/hardware/dma.h>  // to provide dma_claim_unused_channel()
#include <platform.h>

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

#define SLEEP_TIME__THREAD_HELLO_DMA__MS (10000)



//----------------------------------------------------------------------
// - SECTION - file scoped
//----------------------------------------------------------------------

static uint32_t sleep_period__thread_hello_dma__fsv = SLEEP_TIME__THREAD_HELLO_DMA__MS;

bool demo_enabled_fsv = true;

void thread_hello_dma_entry_point(void* arg1, void* arg2, void* arg3);



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
                                            thread_hello_dma_entry_point,
                                            NULL, NULL, NULL,
                                            THREAD_HELLO_DMA__PRIORITY,
                                            0,
                                            K_MSEC(1000)); // K_NO_WAIT);

    rstatus = k_thread_name_set(thread_hello_dma__tid, MODULE_ID__THREAD_HELLO_DMA);
    if ( rstatus == 0 ) { } // avoid compiler warning about unused variable - TMH

    sleep_period__thread_hello_dma__fsv = SLEEP_TIME__THREAD_HELLO_DMA__MS;

    return (int)thread_hello_dma__tid;
}

void thread_hello_dma_set_unset_demo(bool option)
{
    demo_enabled_fsv = option;
}


//----------------------------------------------------------------------
// - SECTION - routines private
//----------------------------------------------------------------------

void thread_hello_dma_entry_point(void* arg1, void* arg2, void* arg3)
{
// Data will be copied from src to dst
    const char src[] = "Hello, world! (from DMA)";
    char dst[count_of(src)];

    uint32_t rstatus = ROUTINE_OK;

    if ( rstatus == 0 ) { }

// 2022-08-20 - First call to Pico SDK library:
//    stdio_init_all();

// From pico-examples/dma/hello_dma/hello_dma.c:
    // Get a free channel, panic() if there are none
    int chan = dma_claim_unused_channel(true);

    // 8 bit transfers. Both read and write address increment after each
    // transfer (each pointing to a location in src or dst respectively).
    // No DREQ is selected, so the DMA transfers as fast as it can.

    dma_channel_config c = dma_channel_get_default_config(chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, true);

    dma_channel_configure(
        chan,          // Channel to be configured
        &c,            // The configuration we just created
        dst,           // The initial write address
        src,           // The initial read address
        count_of(src), // Number of transfers; in this case each is 1 byte.
        true           // Start immediately.
    );

    while (1)
    {
        k_msleep(sleep_period__thread_hello_dma__fsv);

        if (demo_enabled_fsv)
        {
            puts(dst);
        }
    }
}




// --- EOF ---
