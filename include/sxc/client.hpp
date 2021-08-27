#ifndef CLIENT_H
#define CLIENT_H

#include "tui.hpp"
#include "tls.hpp"
#include "util.hpp"

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

enum Status {
	init,
	connecting,
	connected,
	talking,
};

class Client
{
private:
	std::mutex m_mutex;
	std::string m_host;
	std::string m_port;
	std::wstring m_cmd;
	Status m_status;
	TUI m_ui;

	void InitTLSClient();
	void InitUI(std::function<void()> on_success);
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
