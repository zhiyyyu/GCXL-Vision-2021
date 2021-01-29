include_directories(
        inc
)

add_library( thread
        src/thread.cpp
        src/thread1.cpp
        src/thread2.cpp
        )

target_link_libraries(
        thread
        -lpthread
)