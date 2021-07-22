#include <sxc/server.hpp>

/**
 * @brief Initialize the TUI and the TCP server
 */
Server::Server()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	// TODO: I'm not sure if putting these two in a separate thread is right
	// TODO: it's very weird. should look up multi threading asio and ftxui
}

/**
 * @brief Initialize the TLS server
 */
void Server::InitServer()
{
	boost::asio::io_context context;
	TLSServer server(context, m_ui);
	context.run();
}

/**
 * @brief Initialize the UI
 */
void Server::InitUI()
{
	auto on_enter = [this]()
	{
		// TODO: Handle input
		m_ui.Print(m_cmd);
		m_ui.ClearInput();
	};

	m_ui.Init(on_enter);
	m_ui.Print("Initializing UI...");
}
