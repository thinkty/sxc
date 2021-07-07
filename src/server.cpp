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

	auto on_arrow_up = [this]()
	{
		m_ui.ScrollUp();
	};

	auto on_arrow_down = [this]()
	{
		m_ui.ScrollDown();
	};

	m_ui.Init(on_enter, on_arrow_up, on_arrow_down);

}
