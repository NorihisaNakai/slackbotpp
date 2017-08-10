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
    slack_bot( const std::string token, const std::string host) :
        _token( token ),
        _host( host ),
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
    const std::string   _token;
    const std::string   _host;
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
};

#endif  // SLACK_BOT_H