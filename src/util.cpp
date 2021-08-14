#include <sxc/util.hpp>

/**
 * @brief Split the given string into words by the space and ignore adjacent
 * spaces. Returns an empty vector on input length is 0.
 */
std::vector<std::wstring> Util::split(std::wstring input)
{
  std::vector<std::wstring> container;
  if (input.size() == 0)
  {
    return container;
  }

  int curr_index = 0;

  // Loop until all the dividers are found.
  // Handle adjacent dividers by searching for the next divider and if the
  // next divider's index is the next one, skip the current index.
  while (true)
  {
    curr_index = input.find_first_of(' ');

    // End of search
    if (curr_index == std::string::npos)
    {
      // Edge case: ending with space causes an empty input
      if (!input.empty())
      {
        container.push_back(input);
      }
      break;
    }
    // Edge case: starting with space. Delete the space and continue iteration
    else if (curr_index == 0)
    {
      input.erase(0, 1);
    }
    else
    {
      std::wstring word(input.substr(0, curr_index));
      container.push_back(word);

      input.erase(0, curr_index + 1);
    }
  }

  return container;
}
