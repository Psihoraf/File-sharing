project(qr LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    libqrencode
    GIT_REPOSITORY https://github.com/fukuchi/libqrencode.git
    GIT_TAG v4.1.1
)

FetchContent_MakeAvailable(libqrencode)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


qt_add_qml_module(qr
    URI Qr
    VERSION 1.0
    SOURCES
        qrgenerator.cpp
        qrgenerator.h
)

target_include_directories(qr PUBLIC ${libqrencode_SOURCE_DIR})

target_link_libraries(qr
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        qrencode
)

