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

option(NIRA_RUNTIME_TESTS "Run constexpr tests at runtime" OFF)

add_executable(nira_tests fixed_point.cpp rational.cpp)
target_link_libraries(nira_tests PRIVATE nira::nira Catch2::Catch2WithMain)
# target_compile_definitions(nira_tests PRIVATE CATCH_CONFIG_FALLBACK_STRINGIFIER=DoesNotExist)
if(NIRA_RUNTIME_TESTS)
    target_compile_definitions(nira_tests PRIVATE CATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
endif()
if(MSVC)
    target_compile_options(nira_tests PRIVATE /W4)
    target_compile_definitions(nira_tests PRIVATE _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    target_compile_options(nira_tests PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wdouble-promotion)
endif()
catch_discover_tests(nira_tests)
