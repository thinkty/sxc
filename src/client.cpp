#include <sxc/client.hpp>

/**
 * Initialize the TUI and read for user input
 */
Client::Client()
	: m_cmd{}
	, m_ui{&m_cmd}
{
	// TODO: Prompt the user for XMPP server ip and port
	// m_host, m_port

	std::thread tls_client(& Client::InitClient, this, m_host, m_port);
	tls_client.detach();

	InitUI();
}

/**
 * @brief Initialize the TLS server
 * @param host Host of the receiving XMPP server
 * @param port Port number of the receiving XMPP server
 * @ref https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/example/cpp11/ssl/client.cpp
 */
void Client::InitClient(std::string host, std::string port)
{
	boost::asio::io_context io_ctx;
	boost::asio::ip::tcp::resolver resolver(io_ctx);
	auto endpoints = resolver.resolve(host, port);

	boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::sslv23);
	ssl_ctx.load_verify_file("/home/thinkty/projects/sxc/rootca.crt"); // TODO: path to cert

	TLSClient client(io_ctx, ssl_ctx, endpoints, m_ui);

	// The execution blocks the thread
	io_ctx.run();

	m_ui.Print("Client closed...");
}

/**
 * @brief Initialize the UI
 */
void Client::InitUI()
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
