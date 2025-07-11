cmake_minimum_required(VERSION 3.16)
project(walltaker_universal)

set(CMAKE_CXX_STANDARD 23)

add_executable(walltaker_universal main.cpp
        config.cpp
        config.h
        network.cpp
        network.h
        wallpaper.cpp
        wallpaper.h
        utils.cpp
        utils.h)

if (WIN32)
    target_link_libraries(walltaker_universal PRIVATE wininet)
    target_include_directories(walltaker_universal PRIVATE ${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include)
elseif(APPLE)
    # macOS specific settings (add frameworks/libs as needed)
    target_include_directories(walltaker_universal PRIVATE ${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include)
else()
    find_package(CURL REQUIRED)
    target_link_libraries(walltaker_universal PRIVATE CURL::libcurl)
    target_include_directories(walltaker_universal PRIVATE ${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include)
endif()
