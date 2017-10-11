#include <boost/lexical_cast.hpp>

#include <iostream>
#include <fstream>

#include "../include/https.h"

https_stream::https_stream( std::string connect_host ) {
    host            = connect_host;
    socket_ptr      = std::make_shared<socket_t>( socket_t( io_service ) );
    resolver_t        resolver( io_service );
    query_t           query( host, "https" );
    endpoint_ptr    = std::make_shared<endpoint_t>( endpoint_t( *resolver.resolve( query ) ) );
    context_ptr     = std::make_shared<context_t>( context_t( boost::asio::ssl::context::sslv23 ) );
}

size_t https_stream::get_content_length( const std::string header ){
    std::string content_length = "0";
    size_t position = header.find( "Content-Length:" );
    if( position != std::string::npos ){
        size_t end_position = header.find( '\n', position + 14 );
        content_length = header.substr(position + 16, end_position - position - 17 );
    }

    return boost::lexical_cast<size_t>( content_length );
}

std::string https_stream::get( const std::string& uri,
                               const std::vector<std::string>& header_options,
                               const std::string& body ){
    ssl_stream_t ssl_stream( io_service, *context_ptr );
    ssl_stream.lowest_layer().connect( *endpoint_ptr );
    ssl_stream.handshake( boost::asio::ssl::stream_base::client );
    boost::asio::streambuf  request;
    std::ostream            request_ostream(&request);
    request_ostream << "GET " << uri << " HTTP/1.1\r\n";
    request_ostream << "Accept: */*\r\n";
    request_ostream << "Connection: Close\r\n";
    for( auto header_option : header_options ){
        request_ostream << header_option << "\r\n";
    }
    request_ostream << "Content-Length: " << boost::lexical_cast<size_t>( body.length() ) << "\r\n\r\n";
    request_ostream << body << "\r\n\r\n";

    boost::asio::write( ssl_stream, request );

    boost::asio::streambuf  response;
    size_t read_size = boost::asio::read_until( ssl_stream, response, "\r\n\r\n" );

    std::string buffer = boost::asio::buffer_cast<const char*>( response.data() );
    response.consume( read_size );

    std::string http_code = buffer;
    http_code = http_code.substr( 9, 3 );
    if( http_code != "200" ) throw http_error( buffer );
    
    size_t content_length = get_content_length( buffer );
    boost::system::error_code error_code;
    if( content_length != 0 )
        boost::asio::read( ssl_stream, response, boost::asio::transfer_at_least(content_length), error_code );
    else
        boost::asio::read( ssl_stream, response, boost::asio::transfer_all(), error_code );

    return boost::asio::buffer_cast<const char*>( response.data() );
}

std::string https_stream::post( const std::string& uri,
                                const std::vector<std::string>& header_options,
                                const std::string& body ){
    ssl_stream_t ssl_stream( io_service, *context_ptr );
    ssl_stream.lowest_layer().connect( *endpoint_ptr );
    ssl_stream.handshake( boost::asio::ssl::stream_base::client );
    boost::asio::streambuf  request;
    std::ostream            request_ostream(&request);
    request_ostream << "POST " << uri << " HTTP/1.1\r\n";
    request_ostream << "Accept: */*\r\n";
    request_ostream << "Connection: Close\r\n";
    for( auto header_option : header_options ){
        request_ostream << header_option << "\r\n";
    }
    request_ostream << "Content-Length: " << boost::lexical_cast<size_t>( body.length() ) << "\r\n\r\n";
    request_ostream << body << "\r\n\r\n";
    boost::asio::write( ssl_stream, request );

    boost::asio::streambuf  response;
    size_t read_size = boost::asio::read_until( ssl_stream, response, "\r\n\r\n" );

    std::string buffer = boost::asio::buffer_cast<const char*>( response.data() );
    response.consume( read_size );

    std::string http_code = buffer;
    http_code = http_code.substr( 9, 3 );
    if( http_code != "200" ) throw http_error( buffer );
    
    size_t content_length = get_content_length( buffer );
    boost::system::error_code error_code;
    if( content_length != 0 )
        boost::asio::read( ssl_stream, response, boost::asio::transfer_at_least(content_length), error_code );
    else
        boost::asio::read( ssl_stream, response, boost::asio::transfer_all(), error_code );

    std::ofstream   ofs( "./https_get.json");
    ofs << boost::asio::buffer_cast<const char*>( response.data() );
    
    return boost::asio::buffer_cast<const char*>( response.data() );

}