cmake_minimum_required(VERSION 3.7)
project(loca)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_compile_options(-Wall -Werror -pedantic)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

include_directories(${PROJECT_NAME} include)
add_executable(${PROJECT_NAME} main.cpp ${all_SRCS})
target_link_libraries(${PROJECT_NAME} OpenGL::GL GLUT::GLUT GLU)

### Add Google test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

### Add tests target
enable_testing()
add_executable(
        ${PROJECT_NAME}_test
        ${all_SRCS}
)
target_include_directories(${PROJECT_NAME}_test PUBLIC include)
target_link_libraries(${PROJECT_NAME}_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)