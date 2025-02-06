cmake_minimum_required(VERSION 3.30)
project(HiEasyX)

include_directories(./)

set(CMAKE_CXX_STANDARD 20)

add_executable(HiEasyX main.cpp
        include/impl/hex_impl.h
        include/hex_geo.h
        include/font/hex_font.h
        include/impl/EasyX/hex_impl_easyx.h
        source/impl/EasyX/hex_impl_easyx.cpp
        include/hex_string.h
        include/hex.h
        source/hex.cpp
        source/hex_window.cpp
        include/hex_window.h
        include/hex_button.h
        source/hex_button.cpp
        include/hex_text.h
        source/hex_text.cpp
)