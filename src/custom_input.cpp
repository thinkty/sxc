#include <sxc/custom_input.hpp>

// static
CustomInput * CustomInput::From(ftxui::Component component)
{
  return static_cast<CustomInput *>(component.get());
}

/**
 * Different from the Input component, it does not take a placeholder
 */
CustomInput::CustomInput(ftxui::StringRef content)
  : m_content(content)
{
}

// Component implementation.
ftxui::Element CustomInput::Render()
{
  m_cursor_position =
    std::max(0, std::min<int>(m_content->size(), m_cursor_position));
  auto main_decorator =
    ftxui::flex | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
  bool is_focused = Focused();

  // Not focused.
  if (!is_focused)
  {
    return ftxui::text(*m_content) | main_decorator | ftxui::reflect(m_input_box);
  }

  std::wstring part_before_cursor = m_content->substr(0, m_cursor_position);
  std::wstring part_at_cursor = m_cursor_position < (int) m_content->size()
                                  ? m_content->substr(m_cursor_position, 1)
                                  : L" ";
  std::wstring part_after_cursor = m_cursor_position < (int) m_content->size() - 1
                                       ? m_content->substr(m_cursor_position + 1)
                                       : L"";
  auto focused = is_focused ? ftxui::focus : ftxui::select;

  return
    ftxui::hbox(
      ftxui::text(part_before_cursor),
      ftxui::text(part_at_cursor) | ftxui::underlined | focused | ftxui::reflect(m_input_box),
      ftxui::text(part_after_cursor)
    ) | ftxui::flex | ftxui::inverted | ftxui::frame | main_decorator | ftxui::reflect(m_input_box);
}

/**
 * Same with the original OnEvent except for handling arrow up and down
 */
bool CustomInput::OnEvent(ftxui::Event event)
{
  m_cursor_position =
      std::max(0, std::min<int>(m_content->size(), m_cursor_position));

  if (event.is_mouse())
  {
    return OnMouseEvent(event);
  }

  std::wstring c;

  // Backspace.
  if (event == ftxui::Event::Backspace)
  {
    if (m_cursor_position == 0)
    {
      return false;
    }
    m_content->erase(m_cursor_position - 1, 1);
    m_cursor_position--;
    on_change();
    return true;
  }

  // Delete
  if (event == ftxui::Event::Delete)
  {
    if (m_cursor_position == int(m_content->size()))
    {
      return false;
    }
    m_content->erase(m_cursor_position, 1);
    on_change();
    return true;
  }

  // Enter.
  if (event == ftxui::Event::Return)
  {
    on_enter();
    return true;
  }

  if (event == ftxui::Event::Custom)
  {
    return false;
  }

  if (event == ftxui::Event::ArrowLeft && m_cursor_position > 0)
  {
    m_cursor_position--;
    return true;
  }

  if (event == ftxui::Event::ArrowRight && m_cursor_position < (int) m_content->size())
  {
    m_cursor_position++;
    return true;
  }

  if (event == ftxui::Event::Home)
  {
    m_cursor_position = 0;
    return true;
  }

  if (event == ftxui::Event::End)
  {
    m_cursor_position = (int) m_content->size();
    return true;
  }

  // Content
  if (event.is_character())
  {
    m_content->insert(m_cursor_position, 1, event.character());
    m_cursor_position++;
    on_change();
    return true;
  }
  return false;
}

bool CustomInput::OnMouseEvent(ftxui::Event event)
{
  if (!CaptureMouse(event))
  {
    return false;
  }
  if (!m_input_box.Contain(event.mouse().x, event.mouse().y))
  {
    return false;
  }

  TakeFocus();

  if (event.mouse().button == ftxui::Mouse::Left &&
      event.mouse().motion == ftxui::Mouse::Pressed)
  {
    int new_cursor_position =
      m_cursor_position + event.mouse().x - m_cursor_box.x_min;
    new_cursor_position =
      std::max(0, std::min<int>(m_content->size(), new_cursor_position));
    if (m_cursor_position != new_cursor_position)
    {
      m_cursor_position = new_cursor_position;
      on_change();
    }
  }

  return true;
}