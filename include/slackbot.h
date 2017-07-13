#ifndef SLACK_BOT_H
#define SLACK_BOT_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

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
        open,
        close,
    };

    void get_rtm_url();

    void websocket_connect();
    void start();
    void send( const std::string& );

protected:
    const std::string _token;
    const std::string _host;
    std::string _websocket_url;

    websocketpp::client<websocketpp::config::asio_client>   _client;
    websocketpp::connection_hdl _hdl;
    websocket_status  _status;
    std::string _msg;

    void on_open( websocketpp::connection_hdl );
    void on_message( websocketpp::connection_hdl hdl,
                     websocketpp::config::asio_tls_client::message_type::ptr );
    void on_close( websocketpp::connection_hdl );
};

#endif  // SLACK_BOT_H
