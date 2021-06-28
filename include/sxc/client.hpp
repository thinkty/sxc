#ifndef CLIENT_H
#define CLIENT_H

#include "tui.hpp"

#include <string>

class client
{
private:
	std::wstring m_cmd;
	tui m_ui;

public:
	client();
};

#endif
