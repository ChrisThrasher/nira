include(FetchContent)
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(Catch2)
include(Catch)

option(RATIONAL_RUNTIME_TESTS "Run constexpr tests at runtime" OFF)

add_executable(test_rational test.cpp)
target_link_libraries(test_rational PRIVATE rational::rational Catch2::Catch2WithMain)
if(RATIONAL_RUNTIME_TESTS)
    target_compile_definitions(test_rational PRIVATE CATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
endif()
if(MSVC)
    target_compile_options(test_rational PRIVATE /W4)
    target_compile_definitions(test_rational PRIVATE _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    target_compile_options(test_rational PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
endif()
catch_discover_tests(test_rational)
