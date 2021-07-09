#include <sxc/tui.hpp>

TUI::TUI(std::wstring * input)
	: m_input{input}
	, m_container{ftxui::Container::Vertical({})}
	, m_msg_count{0}
	, m_index{-1}
{
}

/**
 * Initialize the interactive components of the renderer and display it
 *
 * @param on_enter			Callback on enter pressed
 * @param on_arrow_up		Callback on arrow up pressed
 * @param on_arrow_down	Callback on arrow down pressed
 */
void TUI::Init(
	std::function<void()> on_enter,
	std::function<void()> on_arrow_up,
	std::function<void()> on_arrow_down
)
{
	ftxui::Component input = ftxui::Make<CustomInput>(m_input);
	CustomInput::From(input)->on_enter = on_enter;

	if (on_arrow_up)
	{
		CustomInput::From(input)->on_arrow_up = on_arrow_up;
	}
	else
	{
		CustomInput::From(input)->on_arrow_up = [this]()
		{
			ScrollUp();
		};
	}

	if (on_arrow_down)
	{
		CustomInput::From(input)->on_arrow_down = on_arrow_down;
	}
	else
	{
		CustomInput::From(input)->on_arrow_down = [this]()
		{
			ScrollDown();
		};
	}

	RenderUI(input);
}

/**
 * @brief Draw out the UI aspects of the application. The events are handled at
 * RegisterEventHandlers()
 * @param input Reference to the input component
 */
void TUI::RenderUI(ftxui::Component& input)
{
	// UI elements
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

	auto screen = ftxui::ScreenInteractive::Fullscreen();
	screen.Loop(renderer);
}

/**
 * Print a given message to the display panel
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
