/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example
    
    \version 2024-02-22, V2.1.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

#include "gd32e23x_libopt.h"

void led_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_bit_write(GPIOB, GPIO_PIN_1, SET);

    rcu_periph_clock_enable(RCU_TIMER13);
    timer_deinit(RCU_TIMER13);

    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler        =7199;
    timer_initpara.alignedmode      =TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection =TIMER_COUNTER_UP;
    timer_initpara.period           =999;
    timer_initpara.clockdivision    =TIMER_CKDIV_DIV1;
    timer_init(TIMER13, &timer_initpara);

    timer_auto_reload_shadow_enable(TIMER13);
    timer_interrupt_enable(TIMER13, TIMER_INT_UP);
    nvic_irq_enable(TIMER13_IRQn, 0);
    timer_enable(TIMER13);
}

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
    /* initilize the LEDs, USART and key */
    // gd_eval_led_init(LED1);
    // gd_eval_led_init(LED2);
    // gd_eval_led_init(LED3);
    // gd_eval_com_init(EVAL_COM);
    // gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);
    
    /* print out the clock frequency of system, AHB, APB1 and APB2 */
    // printf("\r\nCK_SYS is %lu", rcu_clock_freq_get(CK_SYS));
    // printf("\r\nCK_AHB is %lu", rcu_clock_freq_get(CK_AHB));
    // printf("\r\nCK_APB1 is %lu", rcu_clock_freq_get(CK_APB1));
    // printf("\r\nCK_APB2 is %lu", rcu_clock_freq_get(CK_APB2));

    led_config();

    // rcu_periph_clock_enable(RCU_GPIOB);
    //
    // gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    // gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    // gpio_bit_write(GPIOB, GPIO_PIN_1, RESET);
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
