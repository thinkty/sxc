cmake_minimum_required(VERSION 3.20)

project(sxc
	LANGUAGES CXX
	VERSION 1.0
)

add_library(client SHARED
	src/client.cpp
)

add_library(server SHARED
	src/server.cpp
)

# target_link_libraries(lib)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(sxc src/main.cpp)
target_link_libraries(sxc client server)

# FTXUI requires C++17
set_target_properties(sxc PROPERTIES CXX_STANDARD 17)

if (WIN32)
else()
	target_compile_options(sxc PRIVATE "-Wall")
	target_compile_options(sxc PRIVATE "-Wextra")
	target_compile_options(sxc PRIVATE "-pedantic")
	target_compile_options(sxc PRIVATE "-Werror")
	target_compile_options(sxc PRIVATE "-Wmissing-declarations")
	target_compile_options(sxc PRIVATE "-Wdeprecated")
	target_compile_options(sxc PRIVATE "-Wshadow")
endif()

