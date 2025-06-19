

add_executable( test_ordering
    ordering_network_test.cpp
)


target_link_libraries(test_ordering
    ordering_network
)

set_target_properties(test_ordering PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

add_executable(test_counting
        counting_network_test.cpp
)

target_link_libraries(test_counting
        ordering_network
)

set_target_properties(test_counting PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)