#include <sxc/server.hpp>

/**
 * Initialize the TUI and parse user input
 */
Server::Server()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	m_ui.Init();
}

