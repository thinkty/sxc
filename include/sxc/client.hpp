#ifndef CLIENT_H
#define CLIENT_H

#include "tui.hpp"
#include "tls.hpp"
#include "util.hpp"

#include <string>
#include <thread>
#include <vector>
#include <functional>

#define GREETINGS_MSG "Welcome to SXC, enter 'help' to show a list of commands"

enum Status {
	init,
	connected,
	talking,
};

class Client
{
private:
	std::string m_host;
	std::string m_port;
	std::wstring m_cmd;
	Status m_status;
	TUI m_ui;

	void InitTLSClient(std::string host, std::string port);
	void InitUI(std::function<void()> on_success);
	void ParseInput();
	void UpdateStatus(const Status & status);

public:
	Client();
};

#endif
