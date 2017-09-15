#ifndef HTTPS_H
#define HTTPS_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <vector>
#include <string>
#include <memory>
#include <exception>


class http_error : public std::exception{
public:
    http_error( const std::string& mes ) : error_mes( mes ){}
    ~http_error(){}
    const char* what() const noexcept{ return error_mes.c_str();}
private:
    std::string error_mes;
};


class https_stream {
public:
    https_stream( std::string connect_host );
    std::string post( const std::string& uri,
                      const std::vector<std::string>& header_options,
                      const std::string& body );
    std::string get(  const std::string& uri,
                      const std::vector<std::string>& header_options,
                      const std::string& body );

protected:
    using socket_t =            boost::asio::ip::tcp::socket;
    using endpoint_t =          boost::asio::ip::tcp::endpoint;
    using resolver_t =          boost::asio::ip::tcp::resolver;
    using query_t =             boost::asio::ip::tcp::resolver::query;
    using context_t  =          boost::asio::ssl::context;
    using ssl_stream_t =        boost::asio::ssl::stream<socket_t>;
    std::string host;
    boost::asio::io_service     io_service;
    std::shared_ptr<socket_t>   socket_ptr;
    std::shared_ptr<endpoint_t> endpoint_ptr;
    std::shared_ptr<context_t>  context_ptr;
    size_t get_content_length( const std::string header );
};

#endif  // HTTPS_H