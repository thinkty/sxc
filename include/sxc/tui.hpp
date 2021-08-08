#ifndef TUI_HH
#define TUI_HH

#include <sxc/focusable.hpp>
#include <sxc/custom_input.hpp>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <mutex>
#include <string>
#include <functional>

class TUI
{
private:
	std::wstring * m_input;
	ftxui::Component m_container;
	ftxui::ScreenInteractive m_screen;
	int m_msg_count;
	int m_index;
	std::mutex m_mutex;

	void ScrollUp();
	void ScrollDown();

public:
	TUI(std::wstring * input);
	void Init(
		std::function<void()> on_enter,
		std::function<void()> on_success = ([]{})
	);
	void Print(const std::wstring message);
	void Print(const std::string message);
	void ClearInput();
	void ClearContainer();
};

#endif
