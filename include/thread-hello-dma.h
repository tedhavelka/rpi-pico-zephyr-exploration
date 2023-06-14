#ifndef _RP2040_EXPLORATION__THREAD_HELLO_DMA
#define _RP2040_EXPLORATION__THREAD_HELLO_DMA


#include <stdbool.h>

int thread_hello_dma__initialize(void);

//void thread_hello_dma__entry_point(void* arg1, void* arg2, void* arg3);

void thread_hello_dma_set_unset_demo(bool option);


#endif // _RP2040_EXPLORATION__THREAD_HELLO_DMA
