file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${sxc_SOURCE_DIR}/include/sxc/*.hpp")

add_library(sxc_lib STATIC
	client.cpp
	server.cpp
	tui.cpp
	focusable.cpp
	custom_input.cpp
	tls.cpp
	util.cpp
	queue.cpp
	${HEADER_LIST}
)

target_include_directories(sxc_lib
	PUBLIC ../include
)

target_link_libraries(sxc_lib
	PUBLIC ftxui::screen
	PUBLIC ftxui::dom
	PUBLIC ftxui::component
	PUBLIC OpenSSL::SSL
	PUBLIC Boost::system
)

set_target_properties(sxc_lib PROPERTIES CXX_STANDARD 17)
