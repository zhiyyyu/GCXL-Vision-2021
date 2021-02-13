find_package(OpenCV REQUIRED)

include_directories(
        inc
        ${OpenCV_INCLUDE_DIRS}
)

add_library( locate
        src/locate.cpp
        )

target_link_libraries(
        locate
        ${OpenCV_LIBS}
        -lboost_system
        -lboost_filesystem
)