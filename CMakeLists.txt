cmake_minimum_required(VERSION 3.6)
project(QRCode)
find_package(OpenCV REQUIRED)
find_package(Sophus REQUIRED)

set(Sophus_LIBRARIES /usr/local/lib/libSophus.so)

add_subdirectory(solve)
add_subdirectory(thread)
add_subdirectory(serialPort)
add_subdirectory(detect)

include_directories(
        serialPort/inc
        solve/inc
        thread/inc
        detect/inc
        ${OpenCV_INCLUDE_DIRS}
        /usr/include/eigen3
        ${Sophus_INCLUDE_DIRS}
)

set(SOURCE_FILES
        main.cpp
        AutoAttack.cpp
)
add_executable(QRCode
        ${SOURCE_FILES}
)

target_link_libraries(
        QRCode
        thread
        solvepnp
        trans
        detect
        serialPort
        ${OpenCV_LIBS}
        ${Sophus_LIBRARIES}
)