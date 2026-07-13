cmake_minimum_required(VERSION 3.20)

# aKernel - 内核框架库
add_library(aKernel
    aKernel_init.c
)

target_include_directories(aKernel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# main() 需要 aDrv 和 aOS 的头文件
target_include_directories(aKernel
    PRIVATE
        ${CMAKE_SOURCE_DIR}/platform/aDrv/inc
        ${CMAKE_SOURCE_DIR}/platform/aOS/include
)
