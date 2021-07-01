#ifndef TUI_HH
#define TUI_HH

#include <sxc/focusable.hpp>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/input.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <functional>

class TUI
{
private:
	std::wstring * m_input;
	// ftxui::Elements m_container;
	ftxui::Component m_container;

public:
	TUI(std::wstring * input);
	void Init(std::function<void()> cb_on_enter);
	void Print(std::wstring * message);
	void ClearInput();
	void ClearContainer();
};

#endif
