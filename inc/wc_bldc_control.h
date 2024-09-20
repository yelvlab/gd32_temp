//
// Created by dell on 24-9-20.
//

#ifndef WC_BLDC_CONTROL_H
#define WC_BLDC_CONTROL_H

#include "gd32e23x.h"

#define RS485_GPIO_RCU RCU_GPIOA
#define RS485_COM_RCU  RCU_USART0
#define RS485_PORT     GPIOA
#define RS485_DI_PIN   GPIO_PIN_2
#define RS485_RO_PIN   GPIO_PIN_3
#define RS485_RE_PIN   GPIO_PIN_4
#define RS485_COM      USART0

#define GPIO_PORT_SPEED_CTRL     GPIOA
#define GPIO_PIN_SPEED_CTRL      GPIO_PIN_10

#define GPIO_PORT_SPEED_FB       GPIOA
#define GPIO_PIN_SPEED_FB        GPIO_PIN_9

#define GPIO_PORT_DRV_ENABLE     GPIOF
#define GPIO_PIN_DRV_ENABLE      GPIO_PIN_1

#define GPIO_PORT_MOTOR_DIR      GPIOF
#define GPIO_PIN_MOTOR_DIR       GPIO_PIN_0

#define RCU_SPEED_CTL            RCU_GPIOA
#define RCU_SPEED_FB             RCU_GPIOA
#define RCU_DRV_ENABLE           RCU_GPIOF
#define RCU_MOTOR_DIR            RCU_GPIOF

#define TIMER_SPEED_CTL          TIMER0
#define TIMER_CH_SPEED_CTL       TIMER_CH_2
#define RCU_TIMER_SPEED_CTL      RCU_TIMER0

void led_blink_config(void);



/* configure RS485 port & RE/DE Pin */
void rs485_com_config(void);
/* Set transmit enabel */
void rs485_transmit_enable(void);
/* Set receive enabel */
void rs485_receive_enable(void);

void bldc_set_pwm(uint8_t pwm);
void bldc_config(void);
void bldc_enable_set(bit_status status);


void timer2_config(void);

#endif //WC_BLDC_CONTROL_H
