#include <sxc/tui.hpp>

TUI::TUI(std::wstring * input)
	: m_input{input}
	, m_container{ftxui::Container::Vertical({})}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 *
 * @param on_enter			Function object to be called on enter pressed
 * @param on_arrow_up		Function object to be called on arrow up pressed
 * @param on_arrow_down	Function object to be called on arrow down pressed
 */
void TUI::Init(
	std::function<void()> on_enter,
	std::function<void()> on_arrow_up,
	std::function<void()> on_arrow_down
)
{
	ftxui::Component input = ftxui::Make<CustomInput>(m_input);
	CustomInput::From(input)->on_enter = on_enter;
	CustomInput::From(input)->on_arrow_up = on_arrow_up;
	CustomInput::From(input)->on_arrow_down = on_arrow_down;

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
