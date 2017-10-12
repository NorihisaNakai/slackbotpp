#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iostream>
#include <regex>

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
    std::string response_body;
    for( auto i = 0 ; i < 10; ++i ){
        try{
            response_body = stream.get( uri, header_options, "" );
            break;
        }
        catch( ... ){
            std::cout << "connect error" << std::endl;
        }
    }
    auto& response = singleton<connect_response>::get();
    response.from_message( response_body );

    for( auto channel : response.channels )
        std::cout << "cannnel_name: " << channel.name << " | channel_id : " << channel.id << std::endl;

    for( auto user : response.users )
        std::cout << "user_name: " << user.name << " | user_id : " << user.id << std::endl;
}

void slack_bot::websocket_connect() {
    client.set_access_channels( websocketpp::log::alevel::all );
    client.set_error_channels( websocketpp::log::elevel::all );

    // initialize asio
    client.init_asio();

    // Register out handler
    client.set_open_handler( bind( &slack_bot::on_open, this, ::_1 ));
    client.set_tls_init_handler( bind( &slack_bot::on_tls_init, this, ::_1 ));
    client.set_message_handler( bind( &slack_bot::on_message, this, ::_1, ::_2 ));
    client.set_close_handler( bind( &slack_bot::on_close, this, ::_1 ));
    
    websocketpp::lib::error_code    error_code;
    auto& response = singleton<connect_response>::get();
    auto con = client.get_connection( response.url, error_code );

    if( error_code ) {
        std::cout << "connect error: URL " << response.url << std::endl;
        client.get_alog().write( websocketpp::log::alevel::app, error_code.message() );
    }

    hdl = con->get_handle();
    client.connect( con );
    status = websocket_status::connect;
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
    if( status != websocket_status::open ) websocket_connect();
    client.run();
}

void slack_bot::send( const std::string& message ){
    std::cout << "slack_bot::send" << std::endl;
    if( status == open ) {
        try{
            client.send( hdl, message,websocketpp::frame::opcode::text );
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
    status = websocket_status::open;
}

void slack_bot::on_message( websocketpp::connection_hdl hdl,
                            websocketpp::config::asio_tls_client::message_type::ptr message_ptr){
    std::cout << "slack_bot::on_message" << std::endl;
    std::string msg = message_ptr->get_payload();
    std::cout << "[on_message] message : " << msg << std::endl;
    recv_message _recv_message( msg );

    if( _recv_message.type == rtm_type::message ){
        for( auto func : user_functions ){
            func( _recv_message );
        }
    }
}

void slack_bot::on_close( websocketpp::connection_hdl ){
    std::cout << "[on_close] close socket" << std::endl;
    status = websocket_status::close;
}

slack_bot::slack_bot(  const std::string slack_token,
                      const std::string slack_hostname,
                      const std::string jira_hostname,
                      const std::string jira_authuser,
                      const std::string jira_authpassword ) :
                      slack_token( slack_token ),
                      slack_host( slack_hostname ),
                      jira_host( jira_hostname ),
                      jira_username( jira_authuser ),
                      jira_password( jira_authpassword ),
                      status( websocket_status::none ){

    user_functions.push_back( std::bind( &slack_bot::jira_tickets, this, std::placeholders::_1 ) );
    user_functions.push_back( std::bind( &slack_bot::list_users, this, std::placeholders::_1 ) );
    user_functions.push_back( std::bind( &slack_bot::list_channels, this, std::placeholders::_1 ) );
    user_functions.push_back( std::bind( &slack_bot::show_help, this, std::placeholders::_1 ) );
}

void slack_bot::jira_tickets( const recv_message& recv_mes ){
    // parse issue
    std::regex re( "(JIMU|DIR|jimu|dir)\\-(\\d+)" );
    std::smatch match;
    if( std::regex_search( recv_mes.text, match, re ) ){
        jira_core jira( jira_host, jira_username, jira_password );
        jira_issue issue = jira.get_issue(match.str(0));
        send_message send_mes;
        send_mes.channel = recv_mes.channel;
        send_mes.text = ">";
        send_mes.text += issue.summary;
        std::cout << "send_message : "<< send_mes.get_json() << std::endl;
        send( send_mes.get_json());
    }
}

void slack_bot::list_users( const recv_message& recv_mes ){
    std::regex re( "^nya list users" );
    std::smatch match;
    if( std::regex_search( recv_mes.text, match, re ) ){
        send_message send_mes;
        send_mes.channel = recv_mes.channel;
        auto& response = singleton<connect_response>::get();
        for( auto& user : response.users ){
            send_mes.text += user.name;
            send_mes.text += "\\r\\n";
        }
        std::cout << "send_message : " << send_mes.get_json() << std::endl;
        send( send_mes.get_json() );
    }
}

void slack_bot::list_channels( const recv_message& recv_mes ){
    std::regex re( "^nya list channes");
    std::smatch match;
    if( std::regex_search( recv_mes.text, match, re ) ){
        send_message send_mes;
        send_mes.channel = recv_mes.channel;
        auto& response = singleton<connect_response>::get();
        for( auto& channel : response.channels ){
            send_mes.text += channel.name;
            send_mes.text += "\\r\\n";
        }
        std::cout << "send_message : " << send_mes.get_json() << std::endl;
        send( send_mes.get_json() );
    }
}

void slack_bot::show_help( const recv_message& recv_mes ){
    std::regex re( "^nya help");
    std::smatch match;
    if( std::regex_search( recv_mes.text, match, re ) ){
        send_message send_mes;
        send_mes.channel = recv_mes.channel;
        send_mes.text = 
        " nya help show this message\\r\\n"
        " (JIMU|DIR|jimu|dir)\\-(\\d+) display sdy-jira ticket summary\\r\\n"
        " list users display sdyslack user list\\r\\n"
        " list channels display sdyslack channel list\\r\\n";
        std::cout << "send_message : " << send_mes.get_json() << std::endl;
        send( send_mes.get_json() );
    }
}