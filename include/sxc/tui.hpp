#ifndef TUI_HH
#define TUI_HH

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/input.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>

class tui
{
private:
	std::wstring * m_input;
	ftxui::Elements m_content;

public:
	tui(std::wstring * input);
	void init();
	void print(std::wstring * message);
};

#endif
