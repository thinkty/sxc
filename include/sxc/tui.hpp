#ifndef TUI_HH
#define TUI_HH

#include <sxc/focusable.hpp>
#include <sxc/custom_input.hpp>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <functional>

class TUI
{
private:
	std::wstring * m_input;
	// ftxui::Elements m_container;
	ftxui::Component m_container;
	int m_msg_count;
	int m_index;

public:
	TUI(std::wstring * input);
	void Init(std::function<void()>, std::function<void()>, std::function<void()>);
	void Print(const std::wstring message);
	void ScrollUp();
	void ScrollDown();
	void ScrollToBottom();
	void ClearInput();
	void ClearContainer();
};

#endif
