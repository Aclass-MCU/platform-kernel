/**
 ******************************************************************************
 * @file       aKernel_init.c
 * @brief      模块化初始化框架实现 + main 入口
 ******************************************************************************
 */
#include "aKernel_init.h"
#include "aKernel_def.h"
#include "aDrv.h"
#include "aOS.h"

/* 链接脚本符号 */
extern struct akernel_init_entry __akernel_init_start[];
extern struct akernel_init_entry __akernel_init_end[];

void akernel_init_exec(void)
{
    struct akernel_init_entry *entry;

    for (entry = __akernel_init_start; entry < __akernel_init_end; entry++)
    {
        if (entry->fn)
        {
            int ret = entry->fn(entry->data);
            AKERNEL_ASSERT(ret == 0);
        }
    }
}

int main(void)
{
    aDrvInit();              /* 平台初始化（直接调用） */
    akernel_init_exec();    /* 应用层初始化（遍历 init sections） */
    aOS_Run();
}
