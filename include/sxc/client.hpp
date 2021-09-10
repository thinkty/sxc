#ifndef CLIENT_H
#define CLIENT_H

#include <sxc/tui.hpp>
#include <sxc/tls.hpp>
#include <sxc/util.hpp>
#include <sxc/queue.hpp>

#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <functional>

#define GREETINGS_MSG "Welcome to SXC, enter 'help' to show a list of commands"
#define INIT_HELP_MSG "Connect to an XMPP server using the 'connect [host] [port]' command"
#define INIT_ERR_MSG1 "Unable to understand the given command."
#define INIT_ERR_MSG2 "Enter 'help' to show a list of commands available."
#define INIT_ERR_MSG3 "The host argument for the connect command is required."

enum class Status {
	init,
	connecting,
	connected,
	talking,
};

class Client
{
private:
	boost::asio::io_context m_io_ctx;
	std::mutex m_mutex;
	std::string m_host;
	std::string m_port;
	std::wstring m_cmd;
	Status m_status;
	TUI m_ui;
	Queue m_inbound;	// Inbound thread receive and display messages
	Queue m_outbound;	// Outbound thread send messages to the server

	void InitTLSClient();
	void ParseInput();
	void HandleInitStatus();
	void HandleConnectingStatus();
	void HandleConnectedStatus();
	void HandleTalkingStatus();
	void UpdateStatus(const Status & status);

public:
	Client();
};

#endif
