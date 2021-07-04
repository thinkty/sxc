#include <sxc/tui.hpp>

TUI::TUI(std::wstring * input)
	: m_input{input}
	, m_container{ftxui::Container::Vertical({})}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 *
 * @param cb_on_enter Function object to trigger on enter pressed
 */
void TUI::Init(std::function<void()> cb_on_enter)
{
	ftxui::Component input = ftxui::Make<CustomInput>(m_input);
	CustomInput::From(input)->on_enter = cb_on_enter;

	// UI elements
	auto renderer = ftxui::Renderer(input, [&]
		{
			return ftxui::vbox(
				{
					ftxui::vbox(
						{
							m_container->Render(),
						}
					) | ftxui::flex,
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

	auto screen = ftxui::ScreenInteractive::Fullscreen();
	screen.Loop(renderer);
}

/**
 * Print a given message to the display panel
 */
void TUI::Print(const std::wstring message)
{
	m_container->Add(ftxui::Make<Focusable>(message));
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
