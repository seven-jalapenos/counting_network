set(Lib counting_network)

add_library(${Lib} STATIC
    balancer.cpp
    counting_network.cpp
    ordering_network.cpp

)

target_include_directories(${Lib} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)