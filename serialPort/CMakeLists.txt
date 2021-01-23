

include_directories(
        inc
)

add_library( serialPort
        src/serialPort.cpp
)

target_link_libraries(
        serialPort
)