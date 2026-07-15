/**
 ******************************************************************************
 * @file       aKernel_init.c
 * @brief      模块化初始化框架 + HAL 时基（TIM17） + main 入口
 ******************************************************************************
 */
#include "aKernel_init.h"
#include "aKernel_def.h"
#include "aDrv.h"
#include "aOS.h"
#include "stm32h7xx_hal.h"

static TIM_HandleTypeDef _tim_handle;

/* 链接脚本符号 */
extern struct akernel_init_entry __akernel_init_start[];
extern struct akernel_init_entry __akernel_init_end[];

/* ── HAL 时基（TIM17 代替 SysTick）────────────────────── */

void TIM17_IRQHandler(void)
{
    __HAL_TIM_CLEAR_IT(&_tim_handle, TIM_IT_UPDATE);
    HAL_IncTick();
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t timclk, pFLatency;

    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    timclk = HAL_RCC_GetPCLK2Freq();
    if (clkconfig.APB2CLKDivider != RCC_HCLK_DIV1)
        timclk *= 2U;

    __HAL_RCC_TIM17_CLK_ENABLE();

    _tim_handle.Instance               = TIM17;
    _tim_handle.Init.Prescaler         = (timclk / 1000000U) - 1U;
    _tim_handle.Init.Period            = (1000000U / 1000U) - 1U;
    _tim_handle.Init.ClockDivision     = 0U;
    _tim_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    _tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_Init(&_tim_handle);
    __HAL_TIM_CLEAR_FLAG(&_tim_handle, TIM_FLAG_UPDATE);

    HAL_TIM_Base_Start_IT(&_tim_handle);
    HAL_NVIC_SetPriority(TIM17_IRQn, TickPriority, 0U);
    HAL_NVIC_EnableIRQ(TIM17_IRQn);
    return HAL_OK;
}

void HAL_SuspendTick(void)
{
    __HAL_TIM_DISABLE_IT(&_tim_handle, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&_tim_handle, TIM_IT_UPDATE);
}

/* ── 内核初始化 ────────────────────────────────────────── */

void akernel_init_exec(void)
{
    struct akernel_init_entry *entry;

    for (entry = __akernel_init_start; entry < __akernel_init_end; entry++)
    {
        if (entry->fn)
        {
            int ret = entry->fn(entry->data);
            AKERNEL_ASSERT(ret == 0);
            (void)ret;
        }
    }
}

/* ── main 入口 ──────────────────────────────────────────── */

int main(void)
{
    aDrvInit();
    aOS_Create_Task();
    akernel_init_exec();
    aOS_Run();
}
