#include <sxc/tui.hpp>

TUI::TUI(std::wstring * input)
	: m_input{input}
	, m_container{ftxui::Container::Vertical({})}
	, m_msg_count{0}
	, m_index{-1}
	, m_screen{ftxui::ScreenInteractive::Fullscreen()}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 *
 * @param on_enter Callback on enter pressed
 */
void TUI::Init(std::function<void()> on_enter)
{
	ftxui::Component input = ftxui::Make<CustomInput>(m_input);

	// Adding callbacks to each key input event
	CustomInput::From(input)->on_enter = on_enter;
	CustomInput::From(input)->on_arrow_up = [this]()
	{
		ScrollUp();
	};
	CustomInput::From(input)->on_arrow_down = [this]()
	{
		ScrollDown();
	};

	// Layout the UI elements
	auto renderer = ftxui::Renderer(input, [&]
		{
			return ftxui::vbox(
				{
					ftxui::vbox(
						{
							m_container->Render(),
						}
					) | ftxui::flex | ftxui::frame,
					ftxui::separator(),
					ftxui::hbox(
						{
							ftxui::text(L" > "),
							input->Render(),
						}
					),
				}
			) | ftxui::border;
		}
	);

	// Render UI on loop
	m_screen.Loop(renderer);
}

/**
 * @brief Print a given message to the display panel
 * @param message A standard wstring object to be displayed
 */
void TUI::Print(const std::wstring message)
{
	m_container->Add(ftxui::Make<Focusable>(message));

	if (m_index == m_msg_count - 1)
	{
		// If already at bottom, scroll to down
		ScrollDown();
		m_index++;
	}

	m_msg_count++;

	// Trigger a re-render to update the view with new message
	// Thread-safe (https://github.com/ArthurSonzogni/FTXUI/issues/41#issuecomment-671832846)
	m_screen.PostEvent(ftxui::Event::Custom);
}

/**
 * @brief Print a given message to the display panel
 * @param message A standard string object to be converted into wstring before
 * being displayed
 * @warning This will fail if a message contains non-ASCII characters
 */
void TUI::Print(const std::string message)
{
	Print(std::wstring(message.begin(), message.end()));
}

/**
 * Scroll up in the container
 */
void TUI::ScrollUp()
{
	m_container->OnEvent(ftxui::Event::ArrowUp);
	if (m_index > -1)
	{
		m_index--;
	}
}

/**
 * Scroll down in the container
 */
void TUI::ScrollDown()
{
	m_container->OnEvent(ftxui::Event::ArrowDown);
	if (m_index < m_msg_count - 1)
	{
		m_index++;
	}
}

/**
 * Clear input
 */
void TUI::ClearInput()
{
	m_input->clear();
}

/**
 * Clear display panel that contains content
 */
void TUI::ClearContainer()
{
	m_container->~ComponentBase();
	m_container = ftxui::Container::Vertical({});
}
