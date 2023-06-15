/*
 *
 *  Project:  RP2040 Exploration
 *
 *     File:  thread-led.c
 *
 */



//----------------------------------------------------------------------
// - SECTION - includes
//----------------------------------------------------------------------

//#include <string.h>
#include <stdint.h>                // to provide define of uint32_t

// Zephyr RTOS related:
#include <zephyr/kernel.h>

// Zephyr device and target processor peripheral API headers:
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#include "thread-led.h"
#include "app-return-values.h"



//----------------------------------------------------------------------
// - SECTION - stuff mostly to refactor to better file locations:
//----------------------------------------------------------------------

#define THREAD_LED__PRIORITY 8
#define MODULE_ID__THREAD_LED "thread-led"
#define SLEEP_TIME__THREAD_LED__MS 750

// - DEV 0419 ----------------------------------------------------------
#if 0

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#warning --- --- --- macro evaluating 'led0' dts alias returns true --- --- ---
#define LED0_LABEL   DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED0_PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define LED0_FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
#warning --- --- --- macro evaluating 'led0' dts alias returns false --- --- ---
#define LED0_LABEL   ""
#define LED0_PIN     0
#define LED0_FLAGS   0
#endif


#define LED1_NODE DT_ALIAS(led1)

#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
#warning --- --- --- macro evaluating 'led1' dts alias returns true --- --- ---
#define LED1_LABEL   DT_GPIO_LABEL(LED1_NODE, gpios)
#define LED1_PIN     DT_GPIO_PIN(LED1_NODE, gpios)
#define LED1_FLAGS   DT_GPIO_FLAGS(LED1_NODE, gpios)
#else
#warning --- --- --- macro evaluating 'led1' dts alias returns false --- --- ---
#define LED1_LABEL   ""
#define LED1_PIN     0
#define LED1_FLAGS   0
#endif

#endif // 0
// - DEV 0419 ----------------------------------------------------------

//----------------------------------------------------------------------
// - SECTION - file scoped
//----------------------------------------------------------------------

// Per zephyr/boards/arm/rpi_pico/rpi_pico.dts:
#define LED0_NODE DT_NODELABEL(led0)
// Per local dts overlay file:
#define LED1_NODE DT_ALIAS(red_led)

static const struct gpio_dt_spec dt_spec_red_led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec dt_spec_green_led = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

#define SLEEP_TIME_MS (5000)

static uint32_t sleep_period__thread_led__fsv;

#if 0
static const struct device *dev;

static const struct device *dev_led1;
#endif


//----------------------------------------------------------------------
// - SECTION - routines
//----------------------------------------------------------------------

#define THREAD_LED__STACK_SIZE 512

K_THREAD_STACK_DEFINE(thread_led__stack_area, THREAD_LED__STACK_SIZE);
struct k_thread thread_led__thread_data;

int thread_led__initialize(void)
{
    uint32_t rstatus = 0;

    k_tid_t thread_led__tid = k_thread_create(&thread_led__thread_data, thread_led__stack_area,
                                            K_THREAD_STACK_SIZEOF(thread_led__stack_area),
                                            thread_led__entry_point,
                                            NULL, NULL, NULL,
                                            THREAD_LED__PRIORITY,
                                            0,
                                            K_MSEC(1000)); // K_NO_WAIT);

    rstatus = k_thread_name_set(thread_led__tid, MODULE_ID__THREAD_LED);
    if ( rstatus == 0 ) { } // avoid compiler warning about unused variable - TMH

    sleep_period__thread_led__fsv = SLEEP_TIME__THREAD_LED__MS;

    return (int)thread_led__tid;
}

void thread_led__entry_point(void* arg1, void* arg2, void* arg3)
{
    bool led_is_on = true;
    int main_loop_count = 0;
    int rstatus = ROUTINE_OK;

    if (!device_is_ready(dt_spec_red_led.port) ||
        !device_is_ready(dt_spec_green_led.port))
    {
        printk("- thread_leds - one or more ports not ready to drive LEDS!  Exiting . . .\n");
        return;
    }

    int led_red_ok = gpio_pin_configure_dt(&dt_spec_red_led, GPIO_OUTPUT_ACTIVE);
    int led_grn_ok = gpio_pin_configure_dt(&dt_spec_green_led, GPIO_OUTPUT_INACTIVE);

    if ( led_red_ok || led_grn_ok )
    {
        printk("- thread_leds - trouble configuring GPIO ports for Nexus LEDs!  Thread finishing . . .\n");
        return;
    }

    rstatus = gpio_pin_toggle_dt(&dt_spec_red_led);

    while (1)
    {
#if 1
        rstatus = gpio_pin_toggle_dt(&dt_spec_red_led);
        rstatus = gpio_pin_toggle_dt(&dt_spec_green_led);
        led_is_on = !(led_is_on);
#endif

#if 0
        if ( led_is_on )
        {
            printk("- MARK - 0421 red led on\n\r");
        }
        else
        {
            printk("- MARK - 0421 red led off\n\r");
        }
#endif

#if 0
        switch(present_led_task)
        {
// Note - periodic blink case is the original Nordic Semi 'blinky'
//  sample app code, which lights LED at fifty percent duty cycle:
            case PP_THREAD_LED__PERIODIC_BLINK:
		gpio_pin_set(dev, PIN, (int)led_is_on);
		led_is_on = !led_is_on;
		k_msleep(SLEEP_TIME_MS);
                break;

            case PP_THREAD_LED__BLINK_ON_ALERT:
                rstatus = run_user_defined_finite_blink_pattern(PP_THREAD_LED__BLINK_PATTERN_CONTINUE);
                break;

            case PP_THREAD_LED__OFF:
		gpio_pin_set(dev, PIN, false);
                break;

            default:
                break;
        }

// The second call here to k_msleep() provides a minimal yet not
// zero sleep time in case / until we determine what impact is of
// blink patterns with zero or small period defined in them:

// *** DEBUG 0314 BEGIN *** QUESTION where is ten millisecond current spike originating? - TMH

//        if ( present_alert == PP_ALERT__NONE )
        {
            k_msleep(sleep_period__thread_led__fsv);
        }
//        else
//        {
//            k_msleep(SLEEP_PERIOD_OF_TEN_MS);
//        }

// *** DEBUG 0314 END ***
#endif

        k_msleep(sleep_period__thread_led__fsv);
        main_loop_count++;
    }
}



//----------------------------------------------------------------------
// - SECTION - public API
//----------------------------------------------------------------------

