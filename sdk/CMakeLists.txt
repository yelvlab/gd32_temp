cmake_minimum_required(VERSION 3.10)

set(CMSIS_PATH ${CMAKE_CURRENT_LIST_DIR}/CMSIS)
set(PERIPHERAL_PATH ${CMAKE_CURRENT_LIST_DIR}/GD32E23x_standard_peripheral)

# Peripheral Driver
file(GLOB PERIPHERAL_SRCS ${PERIPHERAL_PATH}/src/*.c)

# CMSIS
set(TARGET_C_SOURCES
        ${CMSIS_PATH}/src/system_gd32e23x.c
        ${CMSIS_PATH}/src/syscalls.c
        ${PERIPHERAL_SRCS}
)

set(TARGET_C_INCLUDES
        ${CMSIS_PATH}/inc
        ${PERIPHERAL_PATH}/inc
)

# Shared library and linker script search paths
set(TARGET_LIB_DIRECTORIES
        ${PERIPHERAL_PATH}/inc
)

if (NOT TARGET_STARTUP_ASM)
    set(TARGET_STARTUP_ASM ${CMSIS_PATH}/Startup/startup_gd32e23x.S)
endif ()
message(STATUS "Use startup asm: " ${TARGET_STARTUP_ASM})

if (NOT TARGET_LD_SCRIPT)
    message(FATAL_ERROR "Must set TARGET_LD_SCRIPT to locate ld script")
endif ()
message(STATUS "Use LD script: " ${TARGET_LD_SCRIPT})

add_library(GD32E23X_SDK STATIC ${TARGET_C_SOURCES} ${TARGET_STARTUP_ASM})

# Shared sources, includes and definitions
target_compile_definitions(GD32E23X_SDK PUBLIC ${TARGET_C_DEFINES})
target_include_directories(GD32E23X_SDK
        PUBLIC ${TARGET_C_INCLUDES}
        INTERFACE ${TARGET_C_INCLUDES}
)

target_link_directories(GD32E23X_SDK PUBLIC ${TARGET_LIB_DIRECTORIES})
target_link_libraries(GD32E23X_SDK PUBLIC "c" "m" "nosys")
target_link_options(GD32E23X_SDK
        PUBLIC "-T${TARGET_LD_SCRIPT}"
        PUBLIC "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map,--cref"
)