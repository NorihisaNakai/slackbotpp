#include "../include/websocket_client.h"

#include <iostream>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

websocket_client::websocket_client( std::string url ) :
    _url( url ),
    _status( status::none ),
    _msg( "" ) {

    _client.set_access_channels( websocketpp::log::alevel::all );
    _client.set_error_channels( websocketpp::log::elevel::all );

    // initialize asio
    _client.init_asio();

    // Register out handler
    _client.set_open_handler( bind( &websocket_client::on_open, this, ::_1 ));
    _client.set_message_handler( bind( &websocket_client::on_message, this, ::_1, ::_2 ));
    _client.set_close_handler( bind( &websocket_client::on_close, this, ::_1 ));
}

void    websocket_client::connect() {
    websocketpp::lib::error_code    error_code;
    websocketpp::client<websocketpp::config::asio_client>::connection_ptr con = _client.get_connection( _url, error_code );

    if( error_code ) _client.get_alog().write( websocketpp::log::alevel::app, error_code.message() );

    _client.connect( con );
    _status = status::open;    
}

void    websocket_client::start() {
    if( _status != status::open )   connect();
    _client.run();
}

void    websocket_client::send( const std::string& message ){
    if( _status == status::open ){
        try {
            _client.send( _hdl, message, websocketpp::frame::opcode::text );
        }
        catch( const websocketpp::lib::error_code &e ){
            std::cout << e.message() << std::endl;
        }
    }
    else{
        std::cout << "status is not open" << std::endl;
    }
}

void    websocket_client::on_open( websocketpp::connection_hdl ){
    _status = status::open;
}

void    websocket_client::on_message( websocketpp::connection_hdl hdl,
                                      websocketpp::config::asio_tls_client::message_type::ptr msg){

    _msg = msg->get_payload();
}

void    websocket_client::on_close( websocketpp::connection_hdl ){
    _status = status::close;
}