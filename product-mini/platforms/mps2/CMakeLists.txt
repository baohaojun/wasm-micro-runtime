# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# from ESP-IDF 4.0 examples/build_system/cmake/idf_as_lib
cmake_minimum_required(VERSION 3.5)

project(wamr_mps2 C)

enable_language (ASM)


FREERTOS_DIR_REL := ../../../FreeRTOS
FREERTOS_DIR := $(abspath $(FREERTOS_DIR_REL))
KERNEL_DIR := $(FREERTOS_DIR)/Source

FREERTOS_PLUS_DIR_REL := ../../../FreeRTOS-Plus
FREERTOS_PLUS_DIR := $(abspath $(FREERTOS_PLUS_DIR_REL))

SOURCE_FILES += init/startup.c  syscall.c main.c
SOURCE_FILES += $(KERNEL_DIR)/portable/GCC/ARM_CM3/port.c
SOURCE_FILES += $(KERNEL_DIR)/tasks.c
SOURCE_FILES += $(KERNEL_DIR)/list.c
SOURCE_FILES += $(KERNEL_DIR)/queue.c
SOURCE_FILES += $(KERNEL_DIR)/timers.c
SOURCE_FILES += $(KERNEL_DIR)/event_groups.c
SOURCE_FILES += ${KERNEL_DIR}/portable/MemMang/heap_3.c


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
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_LIBC_BUILTIN 1)
set(WAMR_BUILD_LIBC_WASI 0)

set(WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

include(${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})

set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file} main.c iwasm_main.c
  init/startup.c  syscall.c main.c
  $ENV{FREERTOS_DIR}/Source/portable/GCC/ARM_CM3/port.c
  $ENV{FREERTOS_DIR}/Source/tasks.c
  $ENV{FREERTOS_DIR}/Source/list.c
  $ENV{FREERTOS_DIR}/Source/queue.c
  $ENV{FREERTOS_DIR}/Source/timers.c
  $ENV{FREERTOS_DIR}/Source/event_groups.c
  $ENV{FREERTOS_DIR}/Source/portable/MemMang/heap_3.c
  )

# Link the static libraries to the executable
target_link_libraries(${elf_file} idf::esp32 idf::freertos idf::spi_flash vmlib)
