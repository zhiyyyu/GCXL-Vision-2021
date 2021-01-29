find_package(OpenCV REQUIRED)

include_directories(
        inc
        ${OpenCV_INCLUDE_DIRS}
)

add_library( detect
        src/detect.cpp
        src/score.cpp
        )

target_link_libraries(
        detect
        ${OpenCV_LIBS}
)