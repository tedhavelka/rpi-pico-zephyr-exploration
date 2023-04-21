/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdarg.h>
#include <stdio.h>

#include "pico.h"

// 2022-08-25 THU - amendment by Ted, testing to see whether resolves build error
//  seemingly related to symbol PICO_WEAK_FUNCTION_DEF:
//#include <platform.h>
#include "local_rpi_platform.h"
#include "local_rpi_platform_base.h"



// - DEV 0420 START ----------------------------------------------------
#if 0

PICO_WEAK_FUNCTION_DEF(tight_loop_contents)

void PICO_WEAK_FUNCTION_IMPL_NAME(tight_loop_contents)() {

}

#endif // 0
// - DEV 0420 END ------------------------------------------------------

void __noreturn panic_unsupported() {
    panic("not supported");
}

void hard_assertion_failure(void) {
    panic("Hard assert");
}

void panic(const char *fmt, ...) {
    va_list args;

    puts("*** PANIC ***\n");
    if (fmt) {
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }

    puts("\n");

    __breakpoint();
}

#if 0 // already defined somewhere else - TMH
void __breakpoint() {
    #ifdef _MSC_VER
        __debugbreak();
    #else
        __builtin_trap();
    #endif
}
#endif // already defined somewhere else
