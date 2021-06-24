#include <sxc/client.hpp>

/**
 * Initialize the TUI and read for user input
 */
Client::Client()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	m_ui.Init();

	// TODO: read from the user input and parse on enter press
}

