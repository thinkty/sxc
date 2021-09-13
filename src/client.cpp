#include <sxc/client.hpp>

/**
 * Initialize the TUI and read for user input
 */
Client::Client()
	: m_cmd{}
	, m_ui{&m_cmd}
	, m_status{Status::init}
	, m_io_ctx{}
{
	// In a separate thread, start reading for new messages from the server
	std::thread message_handler_t([&]()
		{
			std::string message = m_inbound.Pop();
			// TODO: handle message from server
		}
	);
	message_handler_t.detach();

	// Parse input based on current status when the user presses enters
	auto on_enter = [this]()
	{
		ParseInput();
		m_ui.ClearInput();
	};

	// Print out the greetings and starter message on successful UI initialization
	auto on_success = [this]()
	{
		m_ui.Print(GREETINGS_MSG);
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

		UpdateStatus(Status::connecting);

		// Prompt for user-id to be used on the XMPP server
		m_ui.Print("Enter the id for the XMPP server");
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
	// Take user id if not occupied already
	if (m_id.empty())
	{
		m_ui.Print(L"> " + m_cmd);
		auto cmds = Util::Split(m_cmd);

		// TODO: ID format verification
		m_id = std::string(cmds[0].begin(), cmds[0].end());

		// Connect to xmpp server
		std::thread tls_client(& Client::InitTLSClient, this);
		tls_client.detach();
	}
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
	boost::asio::ip::tcp::resolver resolver(m_io_ctx);

	try
	{
		auto endpoints = resolver.resolve(m_host, m_port);
		m_ui.Print(Util::GetEndPointInfo(endpoints, m_port, true));

		boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::sslv23);
		// Use the certs in the ca-certificates package
		ssl_ctx.set_default_verify_paths();

		TLSClient client(
			m_io_ctx,
			ssl_ctx,
			endpoints,
			m_ui,
			m_inbound,
			m_outbound,
			m_id,
			[&]()
			{
				UpdateStatus(Status::connected);
			}
		);

		// The execution blocks the thread
		m_io_ctx.run();

		// Set status back to 'init' on disconnect
		m_ui.Print("Connection closed");
		UpdateStatus(Status::init);

		// Reset user id
		m_id.erase();
	}
	catch (const std::exception & e)
	{
		m_ui.Print(e.what());
		UpdateStatus(Status::init);
	}
}
