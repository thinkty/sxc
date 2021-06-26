#ifndef TUI_HH
#define TUI_HH

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/input.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>

class TUI
{
private:
	std::wstring * m_input;

public:
	TUI(std::wstring * input);
	void Init();
	void Print(std::wstring * message);
};

#endif
