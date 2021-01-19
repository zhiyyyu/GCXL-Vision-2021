find_package(OpenCV REQUIRED)

include_directories(
        inc
        ${OpenCV_INCLUDE_DIRS}
)

add_library( solvepnp
        src/solvePnP.cpp
)

target_link_libraries(
        solvepnp
        ${OpenCV_LIBS}
)