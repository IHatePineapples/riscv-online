add_library(utils_threaded_queue
    queue.cpp
    queue.hpp
)

target_link_libraries(utils_threaded_queue pico_sync)
