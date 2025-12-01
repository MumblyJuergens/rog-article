cmake_minimum_required(VERSION 3.18)

project(rog_article)

message("Using ${CMAKE_CXX_COMPILER} in ${CMAKE_BUILD_TYPE}")

# if you are using Windows or cannot install packages, consider using CPM.
# include(cmake/CPM.cmake)
# CPMAddPackage("gh:libsdl-org/SDL#release-3.2.x")
# CPMAddPackage("gh:libsdl-org/SDL_image#release-3.2.x")
find_package(SDL3)
find_package(SDL3_image)

# For FullCompilerWarnings
include(cmake/helpers.cmake)

add_executable(rog_article src/main.cpp)
target_compile_features(rog_article PRIVATE cxx_std_23)
target_link_libraries(rog_article PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
FullCompilerWarnings(rog_article PRIVATE)
