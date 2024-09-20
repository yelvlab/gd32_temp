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

/*!
    \brief      configure RS485 port & RE/DE Pin
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rs485_com_config(void)
{
    rcu_periph_clock_enable(RS485_GPIO_RCU);
    rcu_periph_clock_enable(RS485_COM_RCU);

    gpio_af_set(RS485_PORT, GPIO_AF_1, RS485_RO_PIN | RS485_DI_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(RS485_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RS485_DI_PIN);
    gpio_output_options_set(RS485_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, RS485_DI_PIN);

    gpio_mode_set(RS485_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, RS485_RO_PIN);
    gpio_output_options_set(RS485_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, RS485_RO_PIN);

    /* USART configure */
    usart_deinit(RS485_COM);
    usart_baudrate_set(RS485_COM, 115200U);
    usart_receive_config(RS485_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(RS485_COM, USART_TRANSMIT_ENABLE);

    usart_enable(RS485_COM);

    gpio_mode_set(RS485_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RS485_RE_PIN);
    gpio_output_options_set(RS485_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RS485_RE_PIN);

    gpio_bit_write(RS485_PORT, RS485_RE_PIN, SET);
}

void rs485_transmit_enable(void)
{
    gpio_bit_write(RS485_PORT, RS485_RE_PIN, SET);
}

void rs485_receive_enable(void)
{
    gpio_bit_write(RS485_PORT, RS485_RE_PIN, RESET);
}

uint8_t speed_pwm = 0;   //bldc default pwm is 30

void bldc_set_pwm(uint8_t pwm)
{
    if (pwm > 0 && pwm <= 100)
    {
        speed_pwm = pwm;
        timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, 12*speed_pwm);
    }
}

void bldc_config(void)
{
    rcu_periph_clock_enable(RCU_SPEED_CTL | RCU_SPEED_FB);
    rcu_periph_clock_enable(RCU_DRV_ENABLE | RCU_MOTOR_DIR);

    /* ------------------------------
    -----BLDC speed control GPIO-----
    -----GPIOA_10               -- */
    gpio_mode_set(GPIO_PORT_SPEED_CTRL, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_SPEED_CTRL);
    gpio_output_options_set(GPIO_PORT_SPEED_CTRL, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_SPEED_CTRL);
    gpio_af_set(GPIO_PORT_SPEED_CTRL, GPIO_AF_2, GPIO_PIN_SPEED_CTRL);

    /* ------------------------------
    -----BLDC speed report GPIO------
    -----GPIOA_9               --- */
    // gpio_mode_set(GPIO_PORT_SPEED_FB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_SPEED_FB);
    // gpio_output_options_set(GPIO_PORT_SPEED_FB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_SPEED_FB);
    // gpio_af_set(GPIO_PORT_SPEED_FB, GPIO_AF_2, GPIO_PIN_SPEED_FB);

    /* ------------------------------
    -----BLDC enable GPIO------------
    -----GPIOF_1         --------- */
    gpio_mode_set(GPIO_PORT_DRV_ENABLE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_DRV_ENABLE);
    gpio_output_options_set(GPIO_PORT_DRV_ENABLE, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_DRV_ENABLE);
    gpio_bit_reset(GPIO_PORT_DRV_ENABLE, GPIO_PIN_DRV_ENABLE);

    /* ------------------------------
    -----BLDC forward/reverse GPIO---
    -----GPIOF_0                  ---
    -----no need control for      ---
    -----                WM7040-24V*/
    gpio_mode_set(GPIO_PORT_MOTOR_DIR, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_MOTOR_DIR);
    gpio_output_options_set(GPIO_PORT_MOTOR_DIR, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_MOTOR_DIR);
    gpio_bit_set(GPIO_PORT_MOTOR_DIR, GPIO_PIN_MOTOR_DIR);

    // gpio_bit_write(GPIOA, GPIO_PIN_10, RESET);

    timer_parameter_struct    timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;
    // timer_ic_parameter_struct timer_icinitpara;

    rcu_periph_clock_enable(RCU_TIMER_SPEED_CTL);
    timer_deinit(TIMER_SPEED_CTL);

    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler          =59;
    timer_initpara.alignedmode        =TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection   =TIMER_COUNTER_UP;
    timer_initpara.period             =1199;
    timer_initpara.clockdivision      =TIMER_CKDIV_DIV1;
    timer_init(TIMER_SPEED_CTL, &timer_initpara);

    timer_channel_output_struct_para_init(&timer_ocinitpara);
    timer_ocinitpara.outputstate  =TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate =TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   =TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  =TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  =TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate =TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER_SPEED_CTL, TIMER_CH_SPEED_CTL, &timer_ocinitpara);

    timer_channel_output_pulse_value_config(TIMER_SPEED_CTL, TIMER_CH_SPEED_CTL, 12* speed_pwm);
    timer_channel_output_mode_config(TIMER_SPEED_CTL, TIMER_CH_SPEED_CTL, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER_SPEED_CTL, TIMER_CH_SPEED_CTL, TIMER_OC_SHADOW_DISABLE);

    timer_primary_output_config(TIMER_SPEED_CTL, ENABLE);
    timer_auto_reload_shadow_enable(TIMER_SPEED_CTL);

    timer_enable(TIMER_SPEED_CTL);

    nvic_irq_enable(TIMER0_Channel_IRQn, 1);
}

void bldc_enable_set(bit_status status)
{
    gpio_bit_write(GPIOF, GPIO_PIN_1, status);
}

void timer2_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_CFGCMP);

    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_9);

    nvic_irq_enable(EXTI4_15_IRQn, 2U);

    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN9);
    exti_init(EXTI_9, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_9);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_6);

    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    /* enable the TIMER clock */
    rcu_periph_clock_enable(RCU_TIMER2);
    /* disable a TIMER */
    timer_deinit(TIMER2);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER2, &timer_initpara);

    /* TIMER2  configuration */
    /* initialize TIMER channel input parameter struct */
    timer_channel_input_struct_para_init(&timer_icinitpara);
    /* TIMER2 CH0 input capture configuration */
    timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter    = 0x0;
    timer_input_capture_config(TIMER2,TIMER_CH_0,&timer_icinitpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_CH0);
    /* channel 0 interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_CH0);

    /* TIMER2 counter enable */
    timer_enable(TIMER2);

    nvic_irq_enable(TIMER2_IRQn, 3U);
}
