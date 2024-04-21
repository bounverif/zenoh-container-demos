cmake_minimum_required(VERSION 3.22)

project(
  "reelayd"
  LANGUAGES C CXX
  VERSION "0.10.1"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.83 REQUIRED COMPONENTS program_options)
find_package(zenohc 0.10.1 REQUIRED)
find_package(reelay REQUIRED)
find_package(simdjson REQUIRED)

set(Boost_USE_STATIC_LIBS TRUE)
set(Boost_USE_STATIC_RUNTIME TRUE)

add_executable(reelayd)
target_sources(reelayd PRIVATE "src/main.cpp")
target_link_libraries(reelayd PRIVATE reelay::reelay simdjson::simdjson zenohc::static /usr/local/lib/libboost_program_options.a)

install(
  TARGETS reelayd
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
