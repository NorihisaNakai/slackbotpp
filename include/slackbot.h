#ifndef SLACK_BOT_H
#define SLACK_BOT_H

#include <functional>
#include <vector>

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
                const std::string jira_authpassword );
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

    websocketpp::client<websocketpp::config::asio_tls_client>   client;
    websocketpp::connection_hdl hdl;
    websocket_status  status;

    void websocket_connect();
    websocketpp::lib::shared_ptr<boost::asio::ssl::context>
      on_tls_init( websocketpp::connection_hdl );
    void on_open( websocketpp::connection_hdl );
    void on_message( websocketpp::connection_hdl,
                     websocketpp::config::asio_tls_client::message_type::ptr );
    void on_close( websocketpp::connection_hdl );

    size_t get_content_length( std::string );

    // user executable functions.
    using executable_function_type = std::function<void( const recv_message& )>;
    std::vector< executable_function_type >  user_functions;

    void jira_tickets( const recv_message& );
    void list_users( const recv_message& );
    void list_channels( const recv_message& );
    void nullpo( const recv_message& );
    void show_help( const recv_message& );
};

#endif  // SLACK_BOT_H