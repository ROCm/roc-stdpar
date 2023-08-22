project(compile_test VERSION 0.3.14152 LANGUAGES CXX)

add_executable(algorithms_compile_test algorithms.cpp)
target_compile_options(
    algorithms_compile_test PRIVATE
        -hipstdpar
        --hipstdpar-path=${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(
    algorithms_compile_test PRIVATE
        -hipstdpar
        TBB::tbb
        Threads::Threads)

add_executable(interpose_compile_test interpose.cpp)
target_compile_options(
    interpose_compile_test PRIVATE
        -hipstdpar
        --hipstdpar-interpose-alloc
        --hipstdpar-path=${CMAKE_CURRENT_SOURCE_DIR}/../include
        -fsized-deallocation)
target_link_libraries(
    interpose_compile_test PRIVATE
        -hipstdpar
        TBB::tbb
        Threads::Threads)