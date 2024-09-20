/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example
    
    \version 2024-02-22, V2.1.0, firmware for GD32E23x
*/

#include "main.h"
#include <stdio.h>
#include "gd32e23x.h"
#include "systick.h"

#include "gd32e23x_libopt.h"

#include "wc_bldc_control.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    /* configure systick */
    systick_config();

    // led_config();
    led_blink_config();

    rs485_com_config();

    bldc_config();
    bldc_set_pwm(80);
    delay_ms(5000);
    bldc_enable_set(SET);

    while(1){

    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}
