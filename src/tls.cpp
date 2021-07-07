#include <sxc/tls.hpp>

TLSSession::TLSSession(ssl_stream_t socket)
  : m_socket{std::move(socket)}
{
}

void TLSSession::Start()
{
  Handshake();
}

/**
 * @brief Initiate handshake process and on success, start reading from the
 * connection
 */
void TLSSession::Handshake()
{
  auto self(shared_from_this());
  m_socket.async_handshake(stream_base_t::server,
    [this, self](const std::error_code& ec)
    {
      if (!ec)
      {
        Read();
      }
      else
      {
        // TODO: Handle error stream
      }
    }
  );
}

/**
 * @brief Read asynchronously and handle message
 */
void TLSSession::Read()
{
  auto self(shared_from_this());
  m_socket.async_read_some(boost::asio::buffer(m_data),
    [this, self](const std::error_code& ec, std::size_t length)
    {
      if (!ec)
      {
        // TODO: Read and parse message
      }
      else
      {
        // TODO: Handle error stream
      }
    }
  );
}

/**
 * @brief Write asynchronously to client
 */
void TLSSession::Write(std::string stanza)
{
  auto self(shared_from_this());
  boost::asio::async_write(m_socket, boost::asio::buffer(stanza, stanza.length()),
    [this, self](const std::error_code& ec, std::size_t)
    {
      if (ec)
      {
        // TODO: Handle error stream
      }
    }
  );
}

/**
 * @brief Initialize a TCP server on port 5222
 */
TLSServer::TLSServer(io_context_t & io_context)
  : m_context{context_t::sslv23}
  , m_acceptor{io_context, tcp_t::endpoint(tcp_t::v4(), XMPP_PORT)}
{
  m_context.set_options(
    context_t::default_workarounds
    | context_t::no_sslv2
    | context_t::single_dh_use
  );
  m_context.set_password_callback(std::bind(&TLSServer::GetPW, this));
  m_context.use_certificate_chain_file("user.crt"); // TODO: check if this path works
  m_context.use_private_key_file("user.key", context_t::pem); // TODO:
  m_context.use_tmp_dh_file("dh2048.pem"); // TODO:

  Accept();
}

/**
 * @brief Start listening for new connections
 */
void TLSServer::Accept()
{
}