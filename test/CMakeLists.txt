set(Test counting_network_test)

add_executable(${Test}
    counting_network_test.cpp
)

target_link_libraries(${Test}
    ordering_network
)

set_target_properties(${Test} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)
