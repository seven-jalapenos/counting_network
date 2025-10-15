

set(Lib counting_network)
add_library(${Lib} STATIC
    balancer.cpp
    counting_network.cpp
    balancing_network.cpp
)

set(QLib hash_queue)
add_library(${QLib} STATIC
    hash.cpp
    hash_q.cpp
)

set(TTQLib ticket_queue)
add_library(${TTQLib} STATIC
    ring_array.cpp    
    ttq.cpp
    atq.cpp
)

target_include_directories(${Lib} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${QLib} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${TTQLib} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
