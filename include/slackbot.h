#ifndef SLACK_BOT_H
#define SLACK_BOT_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include "../include/slackmessages.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class   slack_bot {
public:
    slack_bot(  const std::string slack_token,
                const std::string slack_hostname,
                const std::string jira_hostname,
                const std::string jira_authuser,
                const std::string jira_authpassword ) :
        slack_token( slack_token ),
        slack_host( slack_hostname ),
        jira_host( jira_hostname ),
        jira_username( jira_authuser ),
        jira_password( jira_authpassword ),
        _status( websocket_status::none ){}
    ~slack_bot(){}

    enum websocket_status {
        none,
        connect,
        open,
        close,
    };

    void get_rtm_url();
    void start();
    void send( const std::string& );

protected:
    const std::string   slack_token;
    const std::string   slack_host;
    const std::string   jira_username;
    const std::string   jira_password;
    const std::string   jira_host;
    connect_response    _connect_response;

    websocketpp::client<websocketpp::config::asio_tls_client>   _client;
    websocketpp::connection_hdl _hdl;
    websocket_status  _status;

    void websocket_connect();
    websocketpp::lib::shared_ptr<boost::asio::ssl::context>
        on_tls_init( websocketpp::connection_hdl );
    void on_open( websocketpp::connection_hdl );
    void on_message( websocketpp::connection_hdl,
                     websocketpp::config::asio_tls_client::message_type::ptr );
    void on_close( websocketpp::connection_hdl );

    size_t get_content_length( std::string );
};

#endif  // SLACK_BOT_H