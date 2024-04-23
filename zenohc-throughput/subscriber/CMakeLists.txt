cmake_minimum_required(VERSION 3.22)

project(
  "subscriber"
  LANGUAGES C CXX
  VERSION "0.10.1"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(zenohc 0.10.1 REQUIRED)

set(Boost_USE_STATIC_LIBS TRUE)
set(Boost_USE_STATIC_RUNTIME TRUE)

add_executable(subscriber)
target_sources(subscriber PRIVATE "src/main.cpp")
target_link_libraries(subscriber PRIVATE zenohc::static)

install(
  TARGETS subscriber
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
