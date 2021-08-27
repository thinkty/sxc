#include <sxc/client.hpp>

/**
 * Initialize the TUI and read for user input
 */
Client::Client()
	: m_cmd{}
	, m_ui{&m_cmd}
	, m_status{Status::init}
{
	// Print out the greetings and starter message on successful UI initialization
	InitUI([this]()
		{
			m_ui.Print(GREETINGS_MSG);
		}
	);
}

/**
 * @brief Initialize the UI and call the callback function on completion
 * @param on_success Callback function to be called on initialization success
 */
void Client::InitUI(std::function<void()> on_success)
{
	auto on_enter = [this]()
	{
		ParseInput();
		m_ui.ClearInput();
	};

	m_ui.Init(on_enter, on_success);
}

/**
 * @brief Parse user input based on current status. If the user just initialized
 * the program and is in `init` status, listen for `connect` command and
 * arguments regarding host and port.
 *
 * If in `connecting` status, listen for different commands such as
 * - list              : show a list of contacts
 * - add [recipient]   : create a new contact
 * - enter [recipient] : send messages to the specified recipient
 * - disconnect        : disconnect from the current XMPP server
 *
 * In `talking` status, send everything that is typed in to the recipient and
 * listen for commands such as
 * - quit       : leave the current conversation
 * - disconnect : disconnect from the current XMPP server
 */
void Client::ParseInput()
{
	if (m_cmd.empty())
	{
		return;
	}

	switch (m_status)
	{
	case Status::init:
		HandleInitStatus();
		break;

	case Status::connecting:
		HandleConnectingStatus();
		break;
	
	case Status::connected:
		HandleConnectedStatus();
		break;

	case Status::talking:
		HandleTalkingStatus();
		break;
	
	default:
		// Unexpected status
		m_ui.Print("Error: Unexpected Status Code");
		break;
	}
}

/**
 * @brief Handle user commands while the user is in the `init` status
 */
void Client::HandleInitStatus()
{
	m_ui.Print(L"> " + m_cmd);
	auto cmds = Util::Split(m_cmd);

	if (cmds[0] == L"help")
	{
		m_ui.Print(INIT_HELP_MSG);
	}
	// Host argument is required
	else if (cmds.size() == 1 && cmds[0] == L"connect")
	{
		m_ui.Print(INIT_ERR_MSG3);
	}
	else if (cmds.size() <= 3 && cmds[0] == L"connect")
	{
		// Parse host (required) and port (optional)
		m_host = std::string(cmds[1].begin(), cmds[1].end());
		if (cmds.size() == 2)
		{
			m_port = "5222";
		}
		else
		{
			m_port = std::string(cmds[2].begin(), cmds[2].end());
		}

		// Connect to xmpp server
		std::thread tls_client(& Client::InitTLSClient, this);
		tls_client.detach();
	}
	else
	{
		m_ui.Print(INIT_ERR_MSG1);
		m_ui.Print(INIT_ERR_MSG2);
	}
}

/**
 * @brief Handle user commands while the user is in the `connecting` status
 */
void Client::HandleConnectingStatus()
{
	m_ui.Print("Please wait while connecting");
}

/**
 * @brief Handle user commands while the user is in the `connected` status
 */
void Client::HandleConnectedStatus()
{
	m_ui.Print(L"> " + m_cmd);
}

/**
 * @brief Handle user commands while the user is in the `talking` status
 */
void Client::HandleTalkingStatus()
{
	m_ui.Print(L"> " + m_cmd);
}

/**
 * @brief Update current status of the client
 * @param status New status
 */
void Client::UpdateStatus(const Status & status)
{
	std::scoped_lock lock(m_mutex);
	m_status = status;
	std::string status_str;
	switch (m_status)
	{
	case Status::init:
		status_str = "<INIT>";
		break;
	
	case Status::connecting:
		status_str = "<CONNECTING>";
		break;

	case Status::connected:
		status_str = "<CONNECTED>";
		break;

	case Status::talking:
		status_str = "<TALKING>";
		break;

	default:
		status_str = "<UNRECOGNIZED>";
		break;
	}
	m_ui.Print("Updated status to " + status_str);
}

/**
 * @brief Initialize the TLS server
 * @ref https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/example/cpp11/ssl/client.cpp
 */
void Client::InitTLSClient()
{
	UpdateStatus(Status::connecting);
	boost::asio::io_context io_ctx;
	boost::asio::ip::tcp::resolver resolver(io_ctx);

	try
	{
		auto endpoints = resolver.resolve(m_host, m_port);
		m_ui.Print(Util::GetEndPointInfo(endpoints, m_port, true));

		boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::sslv23);
		ssl_ctx.load_verify_file("/home/thinkty/projects/sxc/rootca.crt"); // TODO: path to cert

		TLSClient client(io_ctx, ssl_ctx, endpoints, m_ui, [&]()
			{
				UpdateStatus(Status::connected);
			}
		);

		// The execution blocks the thread
		io_ctx.run();

		// Set status back to 'init' on disconnect
		m_ui.Print("Connection closed");
		UpdateStatus(Status::init);
	}
	catch (const std::exception & e)
	{
		m_ui.Print(e.what());
		UpdateStatus(Status::init);
	}
}
