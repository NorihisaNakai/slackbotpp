#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iostream>

#include "../include/slackbot.h"
#include "../include/jira.h"
#include "../include/https.h"
#include "../include/singleton.h"

void slack_bot::get_rtm_url(){

    std::string uri = "/api/rtm.start?token=" + slack_token;
    std::vector<std::string> header_options;
    std::string header_option = "Host:" + slack_host;
    header_options.push_back( header_option );

    https_stream stream( slack_host );
    std::string response_body = stream.get( uri, header_options, "" );

    auto response = singleton<connect_response>::get();
    response.from_message( response_body );

    std::cout << "WEBSOCKET_URL = " << response.url << std::endl;
    for( auto channel : response.channels )
        std::cout << "cannnel_name: " << channel.name << " | channel_id : " << channel.id << std::endl;

    for( auto user : response.users )
        std::cout << "user_name: " << user.name << " | user_id : " << user.id << std::endl;
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
    std::cout << "slack_bot::send" << std::endl;
    if( _status == open ) {
        try{
            _client.send( _hdl, message,websocketpp::frame::opcode::text );
        }
        catch( const websocketpp::lib::error_code& e ){
            std::cout << e.message() << std::endl;
        }
        catch( const std::exception& e ){
            std::cout << "FATAL send error: " << e.what() << std::endl;
        }
    }
}

void slack_bot::on_open( websocketpp::connection_hdl ){
    std::cout << "slack_bot::on_open" << std::endl;
    _status = websocket_status::open;
}

void slack_bot::on_message( websocketpp::connection_hdl hdl,
                            websocketpp::config::asio_tls_client::message_type::ptr message_ptr){
    std::cout << "slack_bot::on_message" << std::endl;
    std::string msg = message_ptr->get_payload();
    std::cout << "[on_message] message : " << msg << std::endl;
    recv_message _recv_message( msg );


    if( _recv_message.type == rtm_type::message ){
        jira_core jira( jira_host, jira_username, jira_password );
        jira_issue issue = jira.get_issue("JIMU-235");

        send_message _send_message;
        _send_message.channel = _recv_message.channel;
        _send_message.text = ">" + _recv_message.text;
        send( _send_message.get_json() );
    }
}

void slack_bot::on_close( websocketpp::connection_hdl ){
    std::cout << "[on_close] close socket" << std::endl;
    _status = websocket_status::close;
}