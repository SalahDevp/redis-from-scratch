cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyProject VERSION 1.0 LANGUAGES CXX C)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler warnings and debug flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE CLIENT_SRC "src/client.c")

# Object files directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# Add the executable for the server
add_executable(server ${SRCS})
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add the executable for the client
add_executable(client ${CLIENT_SRC})
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


#fetch gtest lib
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
# Test files
file(GLOB_RECURSE TEST_SRCS "test/*.cpp")

add_library(src ${SRCS})
# Add the test executable
add_executable(runTests ${TEST_SRCS})
target_link_libraries(runTests src GTest::gtest_main)
set_target_properties(runTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(GoogleTest)
gtest_discover_tests(runTests)
