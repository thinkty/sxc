#ifndef CLIENT_H
#define CLIENT_H

#include "tui.hpp"

#include <string>

class Client
{
private:
	std::wstring m_cmd;
	TUI m_ui;

public:
	Client();
};

#endif
