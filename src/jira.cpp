#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <sstream>
#include <regex>
#include <iostream>
#include <fstream>

#include "../include/https.h"
#include "../include/jira.h"


std::string jira_core::encode64( const std::string& val ){
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
    return tmp.append((3 - val.size() % 3) % 3, '=');
}

std::string  jira_core::format_json( const std::string& instr ){
    size_t front_pos = instr.find_first_of("{");
    size_t last_pos = instr.find_last_of("}");

    std::string json = instr.substr( front_pos, last_pos - front_pos + 1 );

    std::regex re( "\\r\\n.*?\\r\\n" );
    json = std::regex_replace( json, re, "" );
    std::cout << "jira json: " << json << std::endl;
    return json;
}

jira_issue jira_core::get_issue( std::string ticket ){
    login();
    std::vector<std::string> header_options;
    header_options.push_back( "Host: " + host );
    header_options.push_back( "Content-Type: application/json" );
    std::string cookie = "cookie: ";
    cookie += session_name + "=" + session_id;
    header_options.push_back( cookie );

    https_stream    stream( host );
    std::string uri = "/jira/rest/api/2/issue/" + ticket;
    std::string response = stream.get( uri, header_options, "" );

    boost::property_tree::ptree ptree;
    std::stringstream stringstream;
    for( auto i = 0; i < 10; ++i ){
        try{
            stringstream << format_json( response );
            boost::property_tree::json_parser::read_json( stringstream, ptree );
            break;
        }
        catch( boost::property_tree::ptree_error& e ){
            std::cout << "jira_core::get_issue error: " << e.what() << std::endl;
            std::cout << "json = " << stringstream.str() << std::endl;
        }
        catch( http_error& e ){
            std::cout << "http error: " << e.what() << std::endl;
            jira_issue issue = { ticket, "can not access jira" };
            return issue;
        }
    }
    jira_issue issue;
    issue.key = ptree.get<std::string>("key");
    issue.summary = ptree.get<std::string>("fields.summary");

    return issue;
}

void    jira_core::login(){
    std::vector<std::string>    header_options;
    header_options.push_back( "Host: " + host );
    header_options.push_back( "Content-Type: application/json" );

    std::stringstream body;
    body
        << "{\r\n"
        << "    \"username\": \"" << username << "\",\r\n"
        << "    \"password\": \"" << password << "\"\r\n"
        << "}\r\n";

    https_stream                stream( host );
    std::string response =      stream.post( "/jira/rest/auth/1/session", header_options, body.str() );

    boost::property_tree::ptree ptree;
    std::stringstream stringstream;

    stringstream << format_json( response );
    boost::property_tree::json_parser::read_json( stringstream, ptree );
    session_name = ptree.get<std::string>( "session.name" );
    session_id = ptree.get<std::string>( "session.value" );
    
}
