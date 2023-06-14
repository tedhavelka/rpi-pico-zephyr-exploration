#ifndef _THREAD_SIMPLE_CLI
#define _THREAD_SIMPLE_CLI


int initialize_thread_simple_cli(void);

// routine to send character stream to arbitrary, non-Zephyr-chosen UART:
uint32_t printk_cli(const char* output);

// routine to share arguments counted in latest command input:
uint32_t argument_count_from_cli_module(void);

uint32_t arg_n(const uint32_t requested_arg, char* return_arg);

uint32_t arg_is_decimal(const uint32_t index_to_arg, int* value_to_return);

uint32_t dec_value_at_arg_index(const uint32_t index_to_arg);


// 2021-11-18 - command factoring work:
#define SIZE_COMMAND_TOKEN   (128)
#define SUPPORTED_ARG_LENGTH (16)

// Note, space permitting we'll store up to ten user commands in a ring buffer:
#define SIZE_COMMAND_HISTORY (10)

// Note, for now we support command line up to 256 characters:
#define SIZE_COMMAND_INPUT_SUPPORTED (256)

// Note, we start with support for passing up to ten args to a CLI command herein:
#define MAX_COUNT_SUPPORTED_ARGS (10)


#endif // _THREAD_SIMPLE_CLI
