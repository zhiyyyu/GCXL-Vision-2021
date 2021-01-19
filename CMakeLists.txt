cmake_minimum_required(VERSION 3.6)
project(QRCode)
find_package(OpenCV REQUIRED)

add_subdirectory(solvePnP)
include_directories(
        solvePnP/inc
        ${OpenCV_INCLUDE_DIRS}
)

set(SOURCE_FILES
        main.cpp
)
add_executable(QRCode
        ${SOURCE_FILES}
)

add_library(
        solvePnP
        solvePnP/src/solvePnP.cpp
)
target_link_libraries(
        QRCode
        solvePnP
        ${OpenCV_LIBS}
)