#include <sxc/tls.hpp>

TLSSession::TLSSession(ssl_stream_t socket, TUI & ui)
  : m_socket{std::move(socket)}
  , m_ui{ui}
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
  // The line below increments the ref counter of the shared pointer session so
  // that it does not get deleted
  auto self(shared_from_this());
  m_socket.async_handshake(stream_base_t::server,
    [this, self](const ec_t & ec)
    {
      if (ec)
      {
        m_ui.Print(ec.message());
        return;
      }
      
      Read();
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
    [this, self](const ec_t & ec, std::size_t length)
    {
      if (ec)
      {
        m_ui.Print(ec.message());
        // TODO: Close session on error
        // SEE https://stackoverflow.com/questions/15312219/need-to-call-sslstreamshutdown-when-closing-boost-asio-ssl-socket
        return;
      }
      // TODO: Read and parse message

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
    [this, self](const ec_t & ec, std::size_t)
    {
      if (ec)
      {
        m_ui.Print(ec.message());
      }
    }
  );
}

/**
 * @brief Initialize a TCP server on port 5222
 */
TLSServer::TLSServer(io_context_t & io_context, TUI & ui)
  : m_context{context_t::sslv23}
  , m_acceptor{io_context, tcp_t::endpoint(tcp_t::v4(), XMPP_PORT)}
  , m_ui{ui}
{
  m_context.set_options(
    context_t::default_workarounds
    | context_t::no_sslv2
    | context_t::single_dh_use
  );
  m_context.set_password_callback(std::bind(&TLSServer::GetPW, this));
  // TODO: Perhaps get paths from the arguments I don't like hardcoding it in
  std::string base_path("/home/thinkty/projects/sxc/");
  m_context.use_certificate_chain_file(base_path + "user.crt");
  m_context.use_private_key_file(base_path + "user.key", context_t::pem);
  m_context.use_tmp_dh_file(base_path + "dh2048.pem");

  m_ui.Print("Initializing server...");
  Accept();
}

/**
 * @brief Get the password for the SSL cert
 */
std::string TLSServer::GetPW() const
{
  return "test";
}

/**
 * @brief Start listening for incoming connections
 */
void TLSServer::Accept()
{
  m_acceptor.async_accept(
    [this](const ec_t & ec, tcp_t::socket socket)
    {
      if (ec)
      {
        m_ui.Print(ec.message());
      }
      else
      {
        std::make_shared<TLSSession>(ssl_stream_t(std::move(socket), m_context), m_ui)->Start();
      }

      // Accept new connection
      Accept();
    }
  );
}