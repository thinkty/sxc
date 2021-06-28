#include <sxc/server.hpp>

/**
 * Initialize the TUI and parse user input
 */
server::server()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	m_ui.init();
}

