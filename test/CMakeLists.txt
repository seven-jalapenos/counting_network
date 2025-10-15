

# add_executable( test_ordering
#     ordering_network_test.cpp
# )
# target_link_libraries(test_ordering
#     counting_network
# )
# set_target_properties(test_ordering PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
# )


# add_executable(test_counting
#         counting_network_test.cpp
# )
# target_link_libraries(test_counting
#         counting_network
# )
# set_target_properties(test_counting PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
# )


add_executable(test_queue
        nq_dq_test.cpp
)
target_link_libraries(test_queue
        counting_network
        hash_queue
        ticket_queue
)
set_target_properties(test_queue PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# add_executable(produce_consume
#         producer_consumer_test.cpp
# )
# target_link_libraries(produce_consume
#         counting_network
#         hash_queue
# )
# set_target_properties(produce_consume PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
# )

# add_executable(op_pair
#         op_pair.cpp
# )
# target_link_libraries(op_pair
#         counting_network
#         hash_queue
# )
# set_target_properties(op_pair PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
# )

# add_executable(million
#         million.cpp
# )
# target_link_libraries(million
#         ticket_queue
#         counting_network
# )
# set_target_properties(million PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
# )

add_executable(counting_bm
        counting_bm.cpp
)
target_link_libraries(counting_bm
        counting_network
)
set_target_properties(counting_bm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# add_executable(atomic_op
#         atomic_op.cpp
# )
# target_link_libraries(atomic_op
#         counting_network
#         ticket_queue
# )
# set_target_properties(atomic_op PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
# )