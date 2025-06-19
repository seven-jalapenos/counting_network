set(Lib ordering_network)

add_library(${Lib} STATIC 
    node.cpp
    counting_network.cpp
)

target_include_directories(${Lib} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)