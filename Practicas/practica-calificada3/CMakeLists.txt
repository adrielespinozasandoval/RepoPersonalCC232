cmake_minimum_required(VERSION 3.11)
project(PC3 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(pc3_headers INTERFACE)
target_include_directories(pc3_headers
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_features(pc3_headers INTERFACE cxx_std_17)

function(cc232_add_le_target target_name src_file)
    add_executable(${target_name} ${src_file})
    target_link_libraries(${target_name} PRIVATE pc3_headers)
endfunction()


cc232_add_le_target(pc3_demo_leastelements "demos/demo_leastelements.cpp")
cc232_add_le_target(pc3_demo_treap "demos/demo_treap.cpp")
cc232_add_le_target(pc3_demo_interactive "demos/demo_interactive.cpp")

cc232_add_le_target(pc3_test_public "public_tests/test_public.cpp")
cc232_add_le_target(pc3_test_internal_edge_cases "internal_tests/test_edge_cases.cpp")
cc232_add_le_target(pc3_test_internal_states "internal_tests/test_states.cpp")

cc232_add_le_target(pc3_bench_leastelements "benchmark/bench_leastelements.cpp")

add_test(NAME pc3_public COMMAND pc3_test_public)
add_test(NAME pc3_internal_edge_cases COMMAND pc3_test_internal_edge_cases)
add_test(NAME pc3_internal_states COMMAND pc3_test_internal_states)