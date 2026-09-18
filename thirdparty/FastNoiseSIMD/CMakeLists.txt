cmake_minimum_required(VERSION 3.5...3.16)
project(fastNoiseSIMD)

file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseSIMD/*.cpp")

add_library(fastNoiseSIMD)

# Seperate the AVX2 file and give it special compile flag
foreach(SRC IN LISTS MY_SOURCE)
    if(SRC MATCHES "FastNoiseSIMD_avx2.cpp$")
        set(AVX2_SRC ${SRC})
        list(REMOVE_ITEM MY_SOURCE ${SRC})
    endif()    
endforeach()


target_sources(fastNoiseSIMD PRIVATE ${MY_SOURCE})

if(MSVC)
    set_source_files_properties(${AVX2_SRC} PROPERTIES COMPILE_FLAGS "/arch:AVX2")
else()
    set_source_files_properties(${AVX2_SRC} PROPERTIES COMPILE_FLAGS "-mavx2")
endif()
target_sources(fastNoiseSIMD PRIVATE ${AVX2_SRC})
target_include_directories(fastNoiseSIMD PUBLIC "FastNoiseSIMD")