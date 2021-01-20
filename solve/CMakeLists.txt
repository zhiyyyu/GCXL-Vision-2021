find_package(OpenCV REQUIRED)
find_package(Sophus REQUIRED)

set(Sophus_LIBRARIES /usr/local/lib/libSophus.so)

include_directories(
        inc
        ${Sophus_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        /usr/include/eigen3
)

add_library( solvepnp
        src/solvePnP.cpp
        src/transform.cpp
)

target_link_libraries(
        solvepnp
        ${Sophus_LIBRARIES}
        ${OpenCV_LIBS}
)