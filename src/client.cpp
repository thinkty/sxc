#include <sxc/client.hpp>

/**
 * Initialize the TUI and read for user input
 */
Client::Client()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	m_ui.Init([this]()
		{
			// TODO: Handle input
			m_ui.Print(m_cmd);
			m_ui.ClearInput();
		}
	);

}

