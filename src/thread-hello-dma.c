/*
 *
 *  Project:  RP2040 Exploration
 *
 *     File:  thread-hello-dma.c
 *
 */



#include <stdint.h>                // to provide define of uint32_t

#include <zephyr.h>

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

    while ( 1 )
    {
        k_msleep(sleep_period__thread_hello_dma__fsv);
    }
}




// --- EOF ---
