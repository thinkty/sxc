cmake_minimum_required(VERSION 3.20)

project(sxc
	LANGUAGES CXX
	VERSION 1.0
)

# Import FTXUI (TUI lib)
include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG ba5826eab71461f5c4b8254d5efbcee74134af97
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# End Import

# Import OpenSSL (SSL lib)
find_package(OpenSSL REQUIRED)
# End Import

# Import Asio (Networking lib)
find_package(Boost REQUIRED COMPONENTS system)
# End Import

# Compiled library code
add_subdirectory(src)

# Executable code
add_subdirectory(apps)