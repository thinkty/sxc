#include <sxc/client.hpp>

/**
 * Initialize the TUI and read for user input
 */
client::client()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	m_ui.init();

}

