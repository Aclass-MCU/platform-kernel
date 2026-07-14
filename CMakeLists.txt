cmake_minimum_required(VERSION 3.23)

# ── aKernel — 内核框架库 ──────────────────────────────
set(KERNEL_SOURCES
    aKernel_init.c
    sysmem.c
    syscalls.c
)

set(KERNEL_PUBLIC_HEADERS
    inc/aKernel_def.h
    inc/aKernel_init.h
)

add_library(aKernel ${KERNEL_SOURCES})

target_sources(aKernel PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/inc
    FILES     ${KERNEL_PUBLIC_HEADERS}
)

target_link_libraries(aKernel PUBLIC
    aDrv
    freertos_kernel
)

install(TARGETS aKernel
    ARCHIVE   DESTINATION lib
    FILE_SET HEADERS DESTINATION include/aKernel
)
