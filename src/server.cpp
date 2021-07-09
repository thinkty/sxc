#include <sxc/server.hpp>

/**
 * @brief Initialize the TUI and the TCP server
 */
Server::Server()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	auto on_enter = [this]()
	{
		// TODO: Handle input
		m_ui.Print(m_cmd);
		m_ui.ClearInput();
	};

	m_ui.Init(on_enter);

}
