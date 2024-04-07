add_library(queue  STATIC
    queue.cpp
    queue.hpp
)

target_link_libraries(queue PRIVATE pico_sync)
