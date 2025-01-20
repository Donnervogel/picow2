# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picow2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(picow2 picow2.cpp )

pico_set_program_name(picow2 "picow2")
pico_set_program_version(picow2 "0.1")

# Generate PIO header
pico_generate_pio_header(picow2 ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picow2 0)
pico_enable_stdio_usb(picow2 0)


# Add the standard include files to the build
target_include_directories(picow2 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${PICO_SDK_PATH}/lib/btstack/src
  ${PICO_SDK_PATH}/lib/btstack/3rd-party/bluedroid/encoder/include
  ${PICO_SDK_PATH}/lib/btstack/3rd-party/bluedroid/include
  ${PICO_SDK_PATH}/lib/btstack/3rd-party/bluedroid/decoder/include
  ${PICO_SDK_PATH}/src/common/pico_base_headers/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_platform/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_pio/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_sync/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_timer/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_sync/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_time/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_util/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_sync/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_time/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_util/include
  ${PICO_SDK_PATH}/src/rp2_common/pico_stdio/include
  ${PICO_SDK_PATH}/src/rp2_common/hardware_uart/include
  ${PICO_SDK_PATH}/src/common/pico_stdlib_headers/include
  ${PICO_SDK_PATH}/lib/tinyusb/src
)

# Add any user requested libraries
target_link_libraries(picow2 
        hardware_pio
        
        )
# Add btstack and tinyusb libraries

# Link btstack and tinyusb libraries
target_link_libraries(picow2 
        hardware_pio
        btstack
        tinyusb
        )
pico_add_extra_outputs(picow2)

