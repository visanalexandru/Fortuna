include_directories (${CMAKE_SOURCE_DIR}/src)
add_executable(Tests tests.cpp catch.hpp)
target_link_libraries(Tests Engine)