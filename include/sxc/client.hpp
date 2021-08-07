#ifndef CLIENT_H
#define CLIENT_H

#include "tui.hpp"
#include "tls.hpp"
#include <string>
#include <thread>

class Client
{
private:
	std::string m_host;
	std::string m_port;
	std::wstring m_cmd;
	TUI m_ui;

public:
	Client();
	void InitClient(std::string host, std::string port);
	void InitUI();
};

#endif
