#include <sxc/tui.hpp>

TUI::TUI(std::wstring * input)
	: m_input{input}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 */
void TUI::Init()
{
	ftxui::Component input = ftxui::Input(m_input, "");

	auto renderer = ftxui::Renderer(input, [&]
		{
			return ftxui::vbox(
				{
					ftxui::text(L"Example...") | ftxui::flex,
					ftxui::separator(),
					ftxui::hbox(
						{
							ftxui::text(L"> "), input->Render()
						}
					),
				}
			) | ftxui::border;
		}
	);

	auto screen = ftxui::ScreenInteractive::Fullscreen();
	screen.Loop(renderer);
}
