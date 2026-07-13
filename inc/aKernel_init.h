/**
 ******************************************************************************
 * @file       aKernel_init.h
 * @brief      基于链接脚本段的模块化初始化框架
 * @note       支持：7级优先级 + 数据参数传递
 ******************************************************************************
 */
#ifndef AKERNEL_INIT_H
#define AKERNEL_INIT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

/* 编译器相关的 section 属性 */
#if defined(__GNUC__) || defined(__clang__)
#define AKERNEL_INIT_SECTION(x) __attribute__((section(x), used))
#elif defined(__CC_ARM) || defined(__CLANG_ARM) /* Keil MDK */
#define AKERNEL_INIT_SECTION(x) __attribute__((section(x)))
#elif defined(__IAR_SYSTEMS_ICC__)
#define AKERNEL_INIT_SECTION(x) @ x
#else
#error "不支持的编译器"
#endif

/* 初始化函数指针类型 - 返回 0 表示成功 */
typedef int (*akernel_init_fn_t)(const void *data);

/* 初始化函数条目 - 存储在链接脚本段中 */
struct akernel_init_entry
{
    akernel_init_fn_t fn;
    const void *data;
};

/**
 * @brief 初始化优先级级别（0~6，留给应用层使用）
 *
 * Platform 层函数（aDrvInit、FreeRTOS 启动）直接调用，不占用这些等级。
 */
#define AKERNEL_INIT_LEVEL_0    0
#define AKERNEL_INIT_LEVEL_1    1
#define AKERNEL_INIT_LEVEL_2    2
#define AKERNEL_INIT_LEVEL_3    3
#define AKERNEL_INIT_LEVEL_4    4
#define AKERNEL_INIT_LEVEL_5    5
#define AKERNEL_INIT_LEVEL_6    6

/* 段名构建宏 */
#define AKERNEL_INIT_SECTION_NAME(level) ".init_call_" #level

/**
 * @brief 注册初始化函数
 * @param func  初始化函数：int func(const void *data)
 * @param level 初始化级别
 * @param data  传递的数据（或 NULL）
 */
#define AKERNEL_INIT_REGISTER(func, level, data)                                \
    AKERNEL_INIT_SECTION(AKERNEL_INIT_SECTION_NAME(level))                     \
    static const struct akernel_init_entry _akernel_init_entry_##func =        \
        { (func), (data) }

/**
 * @brief 便捷宏：无数据参数
 */
#define AKERNEL_INIT_REGISTER_NODATA(func, level)                              \
    static int _akernel_init_wrapper_##func(const void *data)                  \
    {                                                                          \
        (void)data;                                                            \
        return func();                                                         \
    }                                                                          \
    AKERNEL_INIT_REGISTER(_akernel_init_wrapper_##func, level, NULL)

/**
 * @brief 快速注册宏
 */
#define AKERNEL_INIT_REGISTER_LEVEL0(func)  AKERNEL_INIT_REGISTER_NODATA(func, 0)
#define AKERNEL_INIT_REGISTER_LEVEL1(func)  AKERNEL_INIT_REGISTER_NODATA(func, 1)
#define AKERNEL_INIT_REGISTER_LEVEL2(func)  AKERNEL_INIT_REGISTER_NODATA(func, 2)
#define AKERNEL_INIT_REGISTER_LEVEL3(func)  AKERNEL_INIT_REGISTER_NODATA(func, 3)
#define AKERNEL_INIT_REGISTER_LEVEL4(func)  AKERNEL_INIT_REGISTER_NODATA(func, 4)
#define AKERNEL_INIT_REGISTER_LEVEL5(func)  AKERNEL_INIT_REGISTER_NODATA(func, 5)
#define AKERNEL_INIT_REGISTER_LEVEL6(func)  AKERNEL_INIT_REGISTER_NODATA(func, 6)

/**
 * @brief 执行所有初始化函数（按优先级顺序）
 */
void akernel_init_exec(void);

#ifdef __cplusplus
}
#endif

#endif /* AKERNEL_INIT_H */
