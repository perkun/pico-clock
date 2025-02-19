cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(clock CXX C ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")

# file(GLOB_RECURSE SSD1306_OLED_PICO_SRC_FILES "vendor/SSD1306_OLED_PICO/src/ssd1306/*.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${SSD1306_OLED_PICO_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/SSD1306_OLED_PICO/include
    ${PROJECT_SOURCE_DIR}/vendor/ds1302-rtc-pico/include
)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/SSD1306_OLED_PICO)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/ds1302-rtc-pico)

# target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/vendor/SSD1306_OLED_PICO/include)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c ssd1306_oled_pico ds1302-rtc-pico)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
  file(
    COPY_FILE
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
    ONLY_IF_DIFFERENT
  )
endif()

