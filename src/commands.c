#include <stdbool.h>               // to provide C 'bool' type definition
#include <stdint.h>                // to provide define of uint32_t

#include <zephyr/toolchain/gcc.h>  // to provide ARG_UNUSED() function like macro

#include "thread-hello-dma.h"


uint32_t cli_set_unset_dma_demo(const char* args)
{
    ARG_UNUSED(args);

    thread_hello_dma_set_unset_demo(false);

    return 0;
}
