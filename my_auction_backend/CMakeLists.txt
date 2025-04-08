cmake_minimum_required(VERSION 3.10)
project(my_auction_app)

set(CMAKE_CXX_STANDARD 17)

# Use pkg-config to find libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Include directories:
include_directories(
    ${CMAKE_SOURCE_DIR}/third_party/crow/include
    ${CMAKE_SOURCE_DIR}/include
    ${PQXX_INCLUDE_DIRS}
    "/opt/homebrew/Cellar/asio/1.30.2/include"  # For Asio headers
)

# Add Homebrew's lib directory so the linker can find libpqxx.
link_directories("/opt/homebrew/lib")

add_executable(my_auction_backend
    src/main.cpp
    src/user_handler.cpp
)

target_link_libraries(my_auction_backend PRIVATE ${PQXX_LIBRARIES} pthread)
