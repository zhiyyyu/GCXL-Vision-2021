

include_directories(
        inc
)

add_library( serialPort
        src/serialPort.cpp
        src/serialPortRead.cpp
        src/serialPortWrite.cpp
)

target_link_libraries(
        serialPort
)