#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
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
    while( boost::asio::read( _ssl_stream, _response, boost::asio::transfer_at_least(1), _error_code ) )
	    body = boost::asio::buffer_cast<const char*>(_response.data());
		
	std::size_t pos = body.find("\r\n\r\n");

	if (pos != std::string::npos) body = body.substr(pos + 4);

    boost::property_tree::ptree _ptree;
    std::stringstream _stringstream;
    _stringstream << body;
    boost::property_tree::json_parser::read_json( _stringstream, _ptree );
    _websocket_url = _ptree.get<std::string>("url");
}

void slack_bot::websocket_connect() {
    _client.set_access_channels( websocketpp::log::alevel::all );
    _client.set_error_channels( websocketpp::log::elevel::all );

    // initialize asio
    _client.init_asio();

    // Register out handler
    _client.set_open_handler( bind( &slack_bot::on_open, this, ::_1 ));
    _client.set_message_handler( bind( &slack_bot::on_message, this, ::_1, ::_2 ));
    _client.set_close_handler( bind( &slack_bot::on_close, this, ::_1 ));
    
    websocketpp::lib::error_code    error_code;
    websocketpp::client<websocketpp::config::asio_client>::connection_ptr con = _client.get_connection( _websocket_url, error_code );

    if( error_code ) _client.get_alog().write( websocketpp::log::alevel::app, error_code.message() );

    _client.connect( con );
    _status = websocket_status::open;
}

void slack_bot::start(){
}

void slack_bot::send( const std::string& ){
}

void slack_bot::on_open( websocketpp::connection_hdl ){
}

void slack_bot::on_message( websocketpp::connection_hdl,
                            websocketpp::config::asio_tls_client::message_type::ptr ){

}

void slack_bot::on_close( websocketpp::connection_hdl ){

}
