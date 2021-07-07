#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <functional>

#define XMPP_PORT 5222

typedef boost::asio::ip::tcp tcp_t;
typedef boost::asio::ssl::stream<tcp_t::socket> ssl_stream_t;
typedef boost::asio::ssl::stream_base stream_base_t;
typedef boost::asio::ssl::context context_t;
typedef boost::asio::io_context io_context_t;

class TLSSession : public std::enable_shared_from_this<TLSSession>
{
private:
  ssl_stream_t m_socket;
  char m_data[1024];

  void Handshake();
  void Read();
  void Write(std::string); // TODO: Take an xmpp object as argument

public:
  TLSSession(ssl_stream_t socket);
  void Start();
};

/**
 * @see https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/examples/cpp11_examples.html#boost_asio.examples.cpp11_examples.ssl
 */
class TLSServer
{
private:
  tcp_t::acceptor m_acceptor;
  context_t m_context;

  std::string GetPW() const;
  void Accept();

public:
  TLSServer(io_context_t & io_context);
};

#endif