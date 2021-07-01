#ifndef FOCUSABLE_HH
#define FOCUSABLE_HH

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>

/**
 * A focusable text to allow scrolling in a vertical container.
 * @see https://github.com/ArthurSonzogni/FTXUI/issues/130
 */
class Focusable : public ftxui::ComponentBase
{
private:
	std::wstring m_content;

public:
	Focusable(std::wstring content);
	ftxui::Element Render() final;
};



#endif
