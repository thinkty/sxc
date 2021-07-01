#include <sxc/focusable.hpp>

Focusable::Focusable(std::wstring content)
  : m_content{content}
{
}

ftxui::Element Focusable::Render()
{
  if (Focused())
  {
    return ftxui::text(m_content) | ftxui::inverted | ftxui::focus;
  }
  else
  {
    return ftxui::text(m_content);
  }
}