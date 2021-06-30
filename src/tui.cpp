#include <sxc/tui.hpp>

tui::tui(std::wstring * input)
	: m_input{input}
	, m_content{ftxui::text(L"Initializing UI...")}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 *
 * @param cb_on_enter Function object to trigger on enter pressed
 */
void tui::init(std::function<void()> cb_on_enter)
{
	ftxui::Component input = ftxui::Input(m_input, "");
	ftxui::InputBase::From(input)->on_enter = cb_on_enter;

	// UI elements
	auto renderer = ftxui::Renderer(input, [&]
		{
			return ftxui::vbox(
				{
					ftxui::vbox(
						{
							ftxui::vbox(
								{
									m_content,
								}
							),
							ftxui::text(L" ") | ftxui::select,
						}
					) | ftxui::frame | ftxui::flex,
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
void tui::print(std::wstring * message)
{
	m_content.push_back(ftxui::text(*message));
}

/**
 * Clear input
 */
void tui::clear_input()
{
	m_input->clear();
}

/**
 * Clear display panel that contains content
 */
void tui::clear_content()
{
	m_content.clear();
}
