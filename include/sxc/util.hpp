#ifndef UTIL_H
#define UTIL_H

#include <boost/asio.hpp>

#include <vector>
#include <string>
#include <functional>

typedef boost::asio::ip::tcp tcp_t;
typedef tcp_t::resolver::results_type results_t;

class Util
{
private:
public:
  static std::vector<std::wstring> Split(std::wstring input);
  static std::string GetEndPointInfo(
    const results_t & endpoints,
    const std::string & port,
    bool only_ipv4 = false
  );
};

#endif