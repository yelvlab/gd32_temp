//
// Created by dell on 24-9-20.
//

#include "wc_bldc_control.h"
#include "gd32e23x.h"

/*!
    \brief      led blink configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_blink_config(void){
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
