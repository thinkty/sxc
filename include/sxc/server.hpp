#ifndef SERVER_H
#define SERVER_H

#include "tui.hpp"
#include "tls.hpp"
#include <string>
#include <thread>

class Server
{
private:
	std::wstring m_cmd;
	TUI m_ui;

public:
	Server();
	void InitServer();
	void InitUI();
};

#endif
