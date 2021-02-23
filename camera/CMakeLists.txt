find_package(OpenCV REQUIRED)

include_directories(
        inc
		./lib/dahen_include
		./lib/dahen_lib
        ${OpenCV_INCLUDE_DIRS}

)

add_library(
        camera
        src/camera.cpp
        src/DAHEN_camera.cpp)

target_link_libraries(
        camera
        ${OpenCV_LIBS}
		/usr/lib/libgxiapi.so
)
