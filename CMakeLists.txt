cmake_minimum_required(VERSION 3.6)
project(QRCode)
find_package(OpenCV REQUIRED)
find_package(Sophus REQUIRED)

set(Sophus_LIBRARIES /usr/local/lib/libSophus.so)

add_subdirectory(solve)
add_subdirectory(serialPort)
add_subdirectory(detect)

include_directories(
        serialPort/inc
        solve/inc
        detect/inc
        ${OpenCV_INCLUDE_DIRS}
        /usr/include/eigen3
        ${Sophus_INCLUDE_DIRS}
)

set(SOURCE_FILES
        main.cpp
        Robot.cpp
)
add_executable(QRCode
        ${SOURCE_FILES}
)

target_link_libraries( QRCode
        solvepnp
        trans
        detect
        serialPort
        ${OpenCV_LIBS}
        ${Sophus_LIBRARIES}
        -lboost_system
        -lboost_thread
)