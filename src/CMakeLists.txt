file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${sxc_SOURCE_DIR}/include/sxc/*.hpp")

add_library(sxc_lib STATIC
	client.cpp
	server.cpp
	${HEADER_LIST}
)

target_include_directories(sxc_lib PUBLIC ../include)

target_link_libraries(sxc_lib
	PRIVATE ftxui::screen
	PRIVATE ftxui::dom
	PRIVATE ftxui::component
)

set_target_properties(sxc_lib PROPERTIES CXX_STANDARD 17)
