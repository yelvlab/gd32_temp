/*!
    \file    systick.c
    \brief   the systick configuration file

    \version 2024-02-22, V2.1.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor inc.

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

volatile static float count_1us = 0;
volatile static float count_1ms = 0;

/**
 * ************************************************************************
 * @brief 配置 SysTick 定时器
 *
 *
 * ************************************************************************
 */
void systick_config(void)
{
    //设置了 SysTick 定时器的时钟源为 HCLK/8
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);
    //计算了每微秒所需的 SysTick 计数值
    count_1us = (float)SystemCoreClock/8000000;
    //计算了每毫秒所需的 SysTick 计数值
    count_1ms = (float)count_1us * 1000;
}


/**
 * ************************************************************************
 * @brief delay_us  微秒延时函数
 *
 * @param[in] count  微秒值
 *
 * ************************************************************************
 */
void delay_us(uint32_t count)
{
    uint32_t ctl;

    //设置 SysTick 计数器的装载值
    SysTick->LOAD = (uint32_t)(count * count_1us);
    //清零 SysTick 计数器，以确保计数器从零开始计数
    SysTick->VAL = 0x0000U;
    //使能 SysTick 定时器，开始进行计数
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    //等待 SysTick 计数器的计数值达到装载值时退出
    do
    {
        ctl = SysTick->CTRL;    //读取 CTRL 寄存器的值
    }while((ctl & SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    //循环退出，禁用 SysTick 定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    //将 SysTick 计数器的当前值清零，以便下次使用
    SysTick->VAL = 0x0000U;

}


/**
 * ************************************************************************
 * @brief delay_ms 毫秒延时函数
 *
 * @param[in] count  毫秒值
 *
 * ************************************************************************
 */
void delay_ms(uint32_t count)
{
    uint32_t ctl;

    //设置 SysTick 计数器的装载值
    SysTick->LOAD = (uint32_t)(count * count_1ms);
    //清零 SysTick 计数器，以确保计数器从零开始计数
    SysTick->VAL = 0x0000U;
    //使能 SysTick 定时器，开始进行计数
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    //等待 SysTick 计数器的计数值达到装载值时退出
    do
    {
        ctl = SysTick->CTRL;    //读取 CTRL 寄存器的值
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    //循环退出，禁用 SysTick 定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    //将 SysTick 计数器的当前值清零，以便下次使用
    SysTick->VAL = 0x0000U;
}
