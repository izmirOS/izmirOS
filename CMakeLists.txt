cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(izmirOS)

add_compile_options(-Wall -Wextra)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(kernel_obj OBJECT ${SRC_FILES})
target_include_directories(kernel_obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(kernel_obj PROPERTIES
    OUTPUT_NAME "kernel"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling kernel"
)

add_custom_command(
    OUTPUT boot.bin
    COMMAND ${CMAKE_ASM_COMPILER} -I${CMAKE_CURRENT_SOURCE_DIR} -f bin ${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm -o boot.bin
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm
    COMMENT "Assembling boot.asm to boot.bin"
)

add_custom_command(
    OUTPUT kernel_entry.o
    COMMAND ${CMAKE_ASM_COMPILER} -I${CMAKE_CURRENT_SOURCE_DIR} -f elf32 ${CMAKE_CURRENT_SOURCE_DIR}/boot/kernel_entry.asm -o kernel_entry.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot/kernel_entry.asm
    COMMENT "Assembling kernel_entry.asm to kernel_entry.o"
)

add_custom_target(boot_bin ALL DEPENDS boot.bin)
add_custom_target(kernel_entry ALL DEPENDS kernel_entry.o)

add_custom_command(
    OUTPUT kernel.bin
    COMMAND /opt/homebrew/bin/i386-elf-gcc
            -Ttext=0x1000
            -nostdlib
            -ffreestanding
            -m32
            -Wl,--oformat=binary
            kernel_entry.o
            $<TARGET_OBJECTS:kernel_obj>
            -o kernel.bin
    DEPENDS kernel_entry kernel_obj ${SRC_FILES}
    COMMAND_EXPAND_LISTS
)

add_custom_target(linked_bin ALL
    DEPENDS kernel.bin
)

add_custom_command(
    OUTPUT image.bin 
    COMMAND cat boot.bin kernel.bin > image.bin
    DEPENDS linked_bin boot_bin
    COMMAND_EXPAND_LISTS
)
add_custom_target(image_bin ALL
    DEPENDS image.bin
)
