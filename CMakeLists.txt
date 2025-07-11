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
        utils.h
        api.cpp
        api.h
        walltaker_response.h
        walltaker_opinion.h
        tray.cpp)

# Find Qt5 instead of Qt6
find_package(Qt5 COMPONENTS Widgets REQUIRED)

if (WIN32)
    target_link_libraries(walltaker_universal PRIVATE wininet Qt5::Widgets)
    target_include_directories(walltaker_universal PRIVATE ${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include)
elseif(APPLE)
    target_link_libraries(walltaker_universal PRIVATE Qt5::Widgets)
    target_include_directories(walltaker_universal PRIVATE ${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include)
else()
    find_package(CURL REQUIRED)
    target_link_libraries(walltaker_universal PRIVATE CURL::libcurl Qt5::Widgets)
    target_include_directories(walltaker_universal PRIVATE ${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include)
endif()

qt5_add_resources(walltaker_universal_resources resources.qrc)
target_sources(walltaker_universal PRIVATE ${walltaker_universal_resources})
