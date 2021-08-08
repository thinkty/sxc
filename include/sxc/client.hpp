#ifndef CLIENT_H
#define CLIENT_H

#include "tui.hpp"
#include "tls.hpp"

#include <string>
#include <thread>
#include <functional>

#define GREETINGS_MSG "Welcome to SXC, enter 'help' to show a list of commands"

class Client
{
private:
	std::string m_host;
	std::string m_port;
	std::wstring m_cmd;
	TUI m_ui;

public:
	Client();
	void InitTLSClient(std::string host, std::string port);
	void InitUI(std::function<void()> on_success);
};

#endif
