/**
 ******************************************************************************
 * @file       aKernel_def.h
 * @brief      aKernel 基础定义
 ******************************************************************************
 */
#ifndef AKERNEL_DEF_H
#define AKERNEL_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* 编译器相关宏定义 */
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARM 编译器 */
#define AKERNEL_SECTION(x) __attribute__((section(x)))
#define AKERNEL_USED __attribute__((used))
#define AKERNEL_ALIGN(n) __attribute__((aligned(n)))
#define AKERNEL_WEAK __attribute__((weak))
#define AKERNEL_INLINE static __inline
#elif defined(__IAR_SYSTEMS_ICC__) /* IAR 编译器 */
#define AKERNEL_SECTION(x) @x
#define AKERNEL_USED __root
#define AKERNEL_PRAGMA(x) _Pragma(#x)
#define AKERNEL_ALIGN(n) _Pragma(#x)
#define AKERNEL_WEAK __weak
#define AKERNEL_INLINE static inline
#elif defined(__GNUC__) /* GCC 编译器 */
#define AKERNEL_SECTION(x) __attribute__((section(x), used))
#define AKERNEL_USED __attribute__((used))
#define AKERNEL_ALIGN(n) __attribute__((aligned(n)))
#define AKERNEL_WEAK __attribute__((weak))
#define AKERNEL_INLINE static inline
#else
#error "不支持的编译器"
#endif

/**
 * @brief 断言宏
 * @note  DEBUG 模式下触发断言失败会进入死循环，Release 模式下为空操作
 */
#ifdef DEBUG
#define AKERNEL_ASSERT(expr)                                                   \
    do                                                                          \
    {                                                                           \
        if (!(expr))                                                            \
        {                                                                       \
            fprintf(stderr, "ASSERT FAILED: %s at %s:%d\n",                     \
                    #expr, __FILE__, __LINE__);                                 \
            while (1);                                                          \
        }                                                                       \
    } while (0)
#else
#define AKERNEL_ASSERT(expr) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* AKERNEL_DEF_H */
