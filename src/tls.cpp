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
      // TODO: find a method to test the handshake (telnet doesn't seem to work)
      m_ui.Print("Incoming connection...");
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
 * @brief Start listening for incoming connections. For a deeper explanation of
 * the process of an Asio server, visit the link in the reference.
 * @ref https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/overview/core/basics.html
 * 
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
        m_ui.Print("Connection accepted...");
        std::make_shared<TLSSession>(ssl_stream_t(std::move(socket), m_context), m_ui)->Start();
      }

      // Accept new connection
      Accept();
    }
  );
}

TLSClient::TLSClient(
  io_context_t & io_context,
  context_t & ssl_context,
  const results_t & endpoints,
  TUI & ui,
  callback_t on_connect
)
  : m_socket{io_context, ssl_context}
  , m_ui{ui}
{
  m_on_connect = std::move(on_connect);

  m_socket.set_verify_mode(boost::asio::ssl::verify_peer);
  // TODO: Can't i use lambda here?
  m_socket.set_verify_callback(
    std::bind(
      &TLSClient::VerifyCert,
      this,
      std::placeholders::_1,
      std::placeholders::_2
    )
  );
  m_on_connect(); /////////// testing
  Connect(endpoints);
}

bool TLSClient::VerifyCert(bool preverified, ver_context_t & ctx)
{
  // The verify callback can be used to check whether the certificate that is
  // being presented is valid for the peer. For example, RFC 2818 describes
  // the steps involved in doing this for HTTPS. Consult the OpenSSL
  // documentation for more details. Note that the callback is called once
  // for each certificate in the certificate chain, starting from the root
  // certificate authority.

  // In this example we will simply print the certificate's subject name.
  char subject_name[256];
  X509 * cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
  X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
  m_ui.Print("Verifying" + std::string(subject_name));

  return preverified;
}

void TLSClient::Connect(const results_t & endpoints)
{
  boost::asio::async_connect(m_socket.lowest_layer(), endpoints,
    [this](const ec_t & error, const tcp_t::endpoint & endpoint)
    {
      if (!error)
      {
        Handshake();
      }
      else
      {
        m_ui.Print(error.message());
      }
    }
  );
}

void TLSClient::Handshake()
{
  m_socket.async_handshake(stream_base_t::client,
    [this](const ec_t & error)
    {
      if (!error)
      {
        // Update status on connect successful
        m_on_connect();
        SendRequest();
      }
      else
      {
        m_ui.Print(error.message());
      }
    }
  );
}

void TLSClient::SendRequest()
{
  // TODO: Instead of sending a request right away, get the request from
  // TODO: the user by using a prod/cons queue

  // TODO: Read from queue if ready
  strcpy(m_req, "hello world");
  size_t req_length = std::strlen(m_req);

  boost::asio::async_write(m_socket,
    boost::asio::buffer(m_req, req_length),
    [this](const ec_t & error, std::size_t res_length)
    {
      if (!error)
      {
        RecvResponse(res_length);
      }
      else
      {
        m_ui.Print(error.message());
      }
    }
  );
}

void TLSClient::RecvResponse(std::size_t res_length)
{
  boost::asio::async_read(m_socket,
    boost::asio::buffer(m_res, res_length),
    [this](const ec_t & error, std::size_t length)
    {
      if (!error)
      {
        m_ui.Print(std::string(m_res));
      }
      else
      {
        m_ui.Print(error.message());
      }
    }
  );
}