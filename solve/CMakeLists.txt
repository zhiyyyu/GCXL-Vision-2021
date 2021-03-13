find_package(OpenCV REQUIRED)
find_package(Sophus REQUIRED)

set(Sophus_LIBRARIES /usr/local/lib/libSophus.so)

include_directories(
        inc
        ../serialPort/inc
        ${Sophus_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        /usr/include/eigen3
)

add_library( solvepnp
        src/solvePnP.cpp
)
add_library( trans
        src/transform.cpp
)

target_link_libraries(
        solvepnp
        trans
        ${Sophus_LIBRARIES}
        ${OpenCV_LIBS}
)