cmake_minimum_required(VERSION 3.15)
project(Rasterizer CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Rasterizer
    draw.cpp
    main.cpp
    External/qdbmp.c
    External/stb_image.cpp
)

target_include_directories(Rasterizer PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/External
)

# The original project only defined WIN32 on Win32 builds, not x64 or Mac.
# Add it back only if some code actually branches on it and needs an equivalent.
# target_compile_definitions(Rasterizer PRIVATE SOME_DEFINE)

if(MSVC)
    target_compile_options(Rasterizer PRIVATE /W3)
else()
    target_compile_options(Rasterizer PRIVATE -Wall)
endif()