#include <sxc/tui.hpp>

tui::tui(std::wstring * input)
	: m_input{input}
	, m_content{ftxui::text(L"Initializing UI...")}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 */
void tui::init()
{
	ftxui::Component input = ftxui::Input(m_input, "");

	// On enter pressed
	ftxui::InputBase::From(input)->on_enter = [&]
	{
		if (!m_input->empty())
		{
			print(m_input);
			m_input->clear();

			// TODO: get a pointer to a function call from the argument to call
		}
	};

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
