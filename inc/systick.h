/**
* ************************************************************************
 *
 * @file systick.h
 * @author GD32
 * @brief
 *
 * ************************************************************************
 * @copyright Copyright (c) 2024 GD32
 * ************************************************************************
 */
#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>

/* function declarations */
/* configure systick */
void systick_config(void);

/* delay a time in milliseconds */
void delay_ms(uint32_t count);

/* delay a time in microseconds */
void delay_us(uint32_t count);

#endif /* SYS_TICK_H */