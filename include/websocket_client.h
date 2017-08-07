#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <string>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class websocket_client {
public:
    enum    status {
        none,
        connected,
        open,
        close
    };
    websocket_client( std::string url );
    void connect();
    void start();
    void send( const std::string& );
    const std::string get_message(){ return _msg; }
    const status get_status(){ return _status; }

protected:
    void on_open( websocketpp::connection_hdl );
    void on_message( websocketpp::connection_hdl hdl,
                     websocketpp::config::asio_tls_client::message_type::ptr );
    void on_close( websocketpp::connection_hdl );

private:
    websocketpp::client<websocketpp::config::asio_client>   _client;
    websocketpp::connection_hdl _hdl;
    status  _status;
    std::string _url;
    std::string _msg;
};

#endif  //WEBSOCKET_CLIENT