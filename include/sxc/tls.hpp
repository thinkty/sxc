#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <sxc/tui.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <functional>

#define XMPP_PORT 5222

typedef boost::asio::ip::tcp tcp_t;
typedef boost::asio::ssl::stream<tcp_t::socket> ssl_stream_t;
typedef boost::asio::ssl::stream_base stream_base_t;
typedef boost::asio::ssl::context context_t;
typedef boost::asio::io_context io_context_t;
typedef boost::system::error_code ec_t;
typedef std::function<void()> callback_t;

class TLSSession : public std::enable_shared_from_this<TLSSession>
{
private:
  ssl_stream_t m_socket;
  TUI & m_ui;
  char m_data[1024];
  callback_t m_callback;

  void Handshake();
  void Read();
  void Write(std::string); // TODO: Take an xmpp object as argument

public:
  TLSSession(ssl_stream_t socket, TUI & ui);
  void Start();
  void SetCallBack(callback_t callback);
};

/**
 * @see https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/examples/cpp11_examples.html#boost_asio.examples.cpp11_examples.ssl
 */
class TLSServer
{
private:
  tcp_t::acceptor m_acceptor;
  context_t m_context;
  TUI & m_ui;

  std::string GetPW() const;
  void Accept();

public:
  TLSServer(io_context_t & io_context, TUI & ui);
};

#endif