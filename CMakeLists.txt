cmake_minimum_required(VERSION 3.10)
project(mykotlinc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# boost-program-options:x64-windows                 1.84.0              Boost program_options module
# replace with your own path
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")
find_package(Boost 1.84.0 REQUIRED COMPONENTS program_options)


add_executable(mykotlinc mykotlinc.cpp)


target_link_libraries(mykotlinc PRIVATE Boost::program_options)

#需要注意架构问题
