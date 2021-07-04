#include <sxc/focusable.hpp>

Focusable::Focusable(std::wstring content)
  : m_content{content}
{
}

/**
 * Simply invert the color of the text when focused
 */
ftxui::Element Focusable::Render()
{
  if (Focused())
  {
    return ftxui::text(m_content) | ftxui::inverted | ftxui::focus;
  }
  return ftxui::text(m_content);
}