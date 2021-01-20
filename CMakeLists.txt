cmake_minimum_required(VERSION 3.6)
project(QRCode)
find_package(OpenCV REQUIRED)
find_package(Sophus REQUIRED)

set(Sophus_LIBRARIES /usr/local/lib/libSophus.so)

add_subdirectory(
        solve
        serialPort
)
include_directories(
        serialPort/inc
        solve/inc
        ${OpenCV_INCLUDE_DIRS}
        /usr/include/eigen3
        ${Sophus_INCLUDE_DIRS}
)

set(SOURCE_FILES
        main.cpp
        S1AutoAttack.cpp
)
add_executable(QRCode
        ${SOURCE_FILES}
)

add_library(
        solve
        solve/src/solvePnP.cpp
        solve/src/transform.cpp
)

add_library(
        serialPort
        serialPort/src/serialPort.cpp
)

target_link_libraries(
        QRCode
        solve
        serialPort
        ${OpenCV_LIBS}
        ${Sophus_LIBRARIES}
)