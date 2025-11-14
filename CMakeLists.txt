cmake_minimum_required(VERSION 3.28)
project(nira
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "C++20 arithmetic library"
)

add_library(nira INTERFACE)
add_library(nira::nira ALIAS nira)
target_sources(nira INTERFACE FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/nira/fixed_point.hpp
        include/nira/rational.hpp
)
target_compile_features(nira INTERFACE cxx_std_20)

include(GNUInstallDirs)
install(TARGETS nira EXPORT nira-targets FILE_SET HEADERS)
install(
    EXPORT nira-targets
    NAMESPACE nira::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/nira
)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    USES_TERMINAL
)
add_custom_target(tidy
    COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    USES_TERMINAL
)
