/*
 * Copyright (c) 2021, 2023 Neela Nurseries
 *
 * SPDX-License-Identifier: Apache-2.0
 */


// #include <sys/printk.h>
#include <stdint.h>

#include "diagnostics.h"

#include "thread-simple-cli.h"



/*
 *----------------------------------------------------------------------
 * @brief     Wrapper function about Zephyr printk().
 * @purpose   To provide easy way to disable messages on chosen UART
 *            peripheral for Zephyr console, shell-uart, uart-mcumgr
 *            and similar.           
 *----------------------------------------------------------------------
 */

void dmsg(const char* message, int option)
{
    if (option == 0)
    {
        return;
    }

    if ( 1 )
    {
        printk_cli(message);
    }
}



// --- EOF ---
