#include <sxc/server.hpp>

/**
 * @brief Initialize the TUI and the TCP server
 */
Server::Server()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	// Run the server in a separate thread
	// TODO: running the server on a separate thread works but i must
	// TODO: look for any problems, especially with ones accessing m_ui.
	// TODO: since it is using the same resource, look for race conditions
	// TODO: in addition, the incoming message does not trigger a re-render
	// TODO: hence, the screen only gets updated on mouse/keyboard event
	std::thread tls_server(& Server::InitServer, this);
	tls_server.detach();

	InitUI();
}

/**
 * @brief Initialize the TLS server
 */
void Server::InitServer()
{
	boost::asio::io_context context;
	TLSServer server(context, m_ui);

	// The execution blocks the thread
	context.run();

	m_ui.Print("Server closed...");
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
