#ifndef SERVER_H
#define SERVER_H

#include "tui.hpp"

#include <string>

class server
{
private:
	std::wstring m_cmd;
	tui m_ui;

public:
	server();
};

#endif
