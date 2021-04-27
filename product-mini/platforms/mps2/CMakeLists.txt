# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# from ESP-IDF 4.0 examples/build_system/cmake/idf_as_lib
cmake_minimum_required(VERSION 3.5)

project(wamr_mps2 C)

enable_language (ASM)

include_directories(
  $ENV{FREERTOS_DIR}/Demo/CORTEX_M3_MPS2_QEMU_GCC
  $ENV{FREERTOS_DIR}/Demo/CORTEX_M3_MPS2_QEMU_GCC/CMSIS
  $ENV{FREERTOS_DIR}/Source/include
  $ENV{FREERTOS_DIR}/Source/portable/GCC/ARM_CM3
  )

set(WAMR_BUILD_PLATFORM "mps2")
set(WAMR_BUILD_TARGET "MPS2")
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_AOT 0)
set(WAMR_BUILD_LIBC_BUILTIN 1)
set(WAMR_BUILD_LIBC_WASI 0)

set(WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

include(${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})

set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file}
  init/startup.c  syscall.c main.c main_blinky.c # iwasm_main.c
  $ENV{FREERTOS_DIR}/Source/portable/GCC/ARM_CM3/port.c
  $ENV{FREERTOS_DIR}/Source/tasks.c
  $ENV{FREERTOS_DIR}/Source/list.c
  $ENV{FREERTOS_DIR}/Source/queue.c
  $ENV{FREERTOS_DIR}/Source/timers.c
  $ENV{FREERTOS_DIR}/Source/event_groups.c
  $ENV{FREERTOS_DIR}/Source/portable/MemMang/heap_3.c
  )

set(CMAKE_C_FLAGS "-DQEMU_SOC_MPS2 -DHEAP3 -nostartfiles -mthumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -Wno-error=implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-declaration-mismatch -Werror -ggdb3 -Og")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -Wstrict-aliasing" )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=1" )
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

set(LDFLAGS "-T ${CMAKE_SOURCE_DIR}/scripts/mps2_m3.ld -specs=nano.specs --specs=rdimon.specs -lc -lrdimon -Xlinker -Map=${CMAKE_BINARY_DIR}/output.map")
set_property(TARGET ${elf_file} PROPERTY LINK_OPTIONS -T ${CMAKE_SOURCE_DIR}/scripts/mps2_m3.ld -specs=nano.specs --specs=rdimon.specs -lc -lrdimon -Xlinker -Map=${CMAKE_BINARY_DIR}/output.map)

# Link the static libraries to the executable
target_link_libraries(${elf_file} vmlib)
