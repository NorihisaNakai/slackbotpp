#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <sstream>

#include "../include/slackbot.h"

void slack_bot::get_rtm_url(){
    std::cout << "host: " << _host << std::endl;
    std::cout << "token: " << _token << std::endl;
    
    boost::asio::io_service         _io_service;
    boost::asio::ip::tcp::socket    _socket(_io_service);
    boost::asio::ip::tcp::resolver  _resolver(_io_service);
    boost::asio::ip::tcp::resolver::query
                                    _query( _host, "https" );
    boost::asio::ip::tcp::endpoint  _endpoint(*_resolver.resolve(_query));

    boost::asio::ssl::context       _context( boost::asio::ssl::context::sslv23);
    boost::asio::ssl::stream< boost::asio::ip::tcp::socket > _ssl_stream( _io_service, _context );
    _ssl_stream.lowest_layer().connect( _endpoint );
    _ssl_stream.handshake( boost::asio::ssl::stream_base::client );

    boost::asio::streambuf          _request;
    std::ostream                    _request_ostream(&_request);
    _request_ostream << "GET /api/rtm.start?token=";
    _request_ostream << _token;
    _request_ostream << " HTTP/1.1\r\n"
        "Host: ";
    _request_ostream << _host << "\r\n\r\n";

    boost::asio::write( _ssl_stream, _request );

    boost::asio::streambuf _response;
    boost::asio::read_until(_ssl_stream, _response, "\r\n");
    std::istream response_stream(&_response);

    boost::system::error_code   _error_code;
    std::string body;


    boost::asio::read( _ssl_stream, _response, boost::asio::transfer_at_least(32768), _error_code );
//    boost::asio::read_until( _ssl_stream, _response, "\r\n", _error_code );
    if( _error_code ){
        std::cout << "sslstream read error:" << _error_code.message() << std::endl;
    }
		
    body = boost::asio::buffer_cast<const char*>(_response.data());
	std::size_t pos = body.find("\r\n\r\n");

	if (pos != std::string::npos) body = body.substr(pos + 4);

    std::cout << "body = " << body << std::endl;

    _connect_response.from_message( body );

    std::cout << "WEBSOCKET_URL = " << _connect_response.url << std::endl;
}

void slack_bot::websocket_connect() {
    _client.set_access_channels( websocketpp::log::alevel::all );
    _client.set_error_channels( websocketpp::log::elevel::all );

    // initialize asio
    _client.init_asio();

    // Register out handler
    _client.set_open_handler( bind( &slack_bot::on_open, this, ::_1 ));
    _client.set_tls_init_handler( bind( &slack_bot::on_tls_init, this, ::_1 ));
    _client.set_message_handler( bind( &slack_bot::on_message, this, ::_1, ::_2 ));
    _client.set_close_handler( bind( &slack_bot::on_close, this, ::_1 ));
    
    websocketpp::lib::error_code    error_code;
     auto con = _client.get_connection( _connect_response.url, error_code );

    if( error_code ) {
        std::cout << "connect error: URL " << _connect_response.url << std::endl;
        _client.get_alog().write( websocketpp::log::alevel::app, error_code.message() );
    }

    _hdl = con->get_handle();
    _client.connect( con );
    _status = websocket_status::connect;
}

websocketpp::lib::shared_ptr<boost::asio::ssl::context>
    slack_bot::on_tls_init( websocketpp::connection_hdl hdl ){

    websocketpp::lib::shared_ptr<boost::asio::ssl::context> ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);
    try{
        ctx->set_options( boost::asio::ssl::context::default_workarounds |
                          boost::asio::ssl::context::no_sslv2 |
                          boost::asio::ssl::context::no_sslv3 |
                          boost::asio::ssl::context::single_dh_use );
    }
    catch( std::exception& e ){
        std::cout << e.what() << std::endl;
    }
    return ctx;
}


void slack_bot::start(){
    if( _status != websocket_status::open ) websocket_connect();
    _client.run();
}

void slack_bot::send( const std::string& message ){
    if( _status == open ) {
        try{
            _client.send( _hdl, message,websocketpp::frame::opcode::text );
        }
        catch( const websocketpp::lib::error_code& e ){
            std::cout << e.message() << std::endl;
        }
        catch( ... ){
            std::cout << "FATAL send error: " << std::endl;
        }
    }
}

void slack_bot::on_open( websocketpp::connection_hdl ){
    std::cout << "connection open" << std::endl;
    _status = websocket_status::open;
}

void slack_bot::on_message( websocketpp::connection_hdl hdl,
                            websocketpp::config::asio_tls_client::message_type::ptr message_ptr){
    std::string msg = message_ptr->get_payload();
    std::cout << "[on_message] message : " << msg << std::endl;
}

void slack_bot::on_close( websocketpp::connection_hdl ){
    _status = websocket_status::close;
    std::cout << "[on_close] close socket" << std::endl;
}
