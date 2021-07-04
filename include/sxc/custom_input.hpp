#ifndef INPUT_HH
#define INPUT_HH

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#include <ftxui/screen/string.hpp>

#include <algorithm>

/**
 * @brief A clone of Input(InputBase) to override OnEvent for custom functions
 * on arrow-up and arrow-down
 * @see https://arthursonzogni.com/FTXUI/doc/input_8hpp_source.html
 * @see https://arthursonzogni.com/FTXUI/doc/input_8cpp_source.html
 */
class CustomInput : public ftxui::ComponentBase
{
private:
	ftxui::StringRef m_content;
  ftxui::Box m_input_box;
  ftxui::Box m_cursor_box;

  bool OnMouseEvent(ftxui::Event);

public:
  int m_cursor_position = 0;
  std::function<void()> on_change = [] {};
  std::function<void()> on_enter = [] {};
  std::function<void()> on_arrow_up = [] {};
  std::function<void()> on_arrow_down = [] {};

  static CustomInput * From(ftxui::Component component);

  CustomInput(ftxui::StringRef content);
  ~CustomInput() override = default;
  ftxui::Element Render() override;
  bool OnEvent(ftxui::Event) override;
};

#endif
