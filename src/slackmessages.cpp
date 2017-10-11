#include "../include/slackmessages.h"

#include <sstream>
#include <iostream>
#include <map>
#include <exception>

#include <boost/property_tree/json_parser.hpp>

using map_function = std::function< void ( const recv_message*, const boost::property_tree::ptree&) >;

std::map< std::string, std::pair<rtm_type, map_function> > rtm_type_map = {
    { "accounts_change",        std::make_pair( rtm_type::accounts_change,          &recv_message::nop ) },
    { "bot_added",              std::make_pair( rtm_type::bot_added,                &recv_message::nop ) },
    { "bot_changed",            std::make_pair( rtm_type::bot_changed,              &recv_message::nop ) },
    { "channel_archive",        std::make_pair( rtm_type::channel_archive,          &recv_message::nop ) },
    { "channel_created",        std::make_pair( rtm_type::channel_created,          &recv_message::nop ) },
    { "channel_deleted" ,       std::make_pair( rtm_type::channel_deleted,          &recv_message::nop ) },
    { "channel_history_changed",std::make_pair( rtm_type::channel_history_changed,  &recv_message::nop ) },
    { "channel_left",           std::make_pair( rtm_type::channel_left,             &recv_message::nop ) },
    { "channel_marked",         std::make_pair( rtm_type::channel_marked,           &recv_message::nop ) },
    { "channel_rename",         std::make_pair( rtm_type::channel_rename,           &recv_message::nop ) },
    { "channel_unarchive",      std::make_pair( rtm_type::channel_unarchive,        &recv_message::nop ) },
    { "command_changed",        std::make_pair( rtm_type::command_changed,          &recv_message::nop ) },
    { "dnd_updated_user",       std::make_pair( rtm_type::dnd_updated_user,         &recv_message::nop ) },
    { "email_domain_changed",   std::make_pair( rtm_type::email_domain_changed,     &recv_message::nop ) },
    { "emoji_changed",          std::make_pair( rtm_type::emoji_changed,            &recv_message::nop ) },
    { "file_change",            std::make_pair( rtm_type::file_change,              &recv_message::nop ) },
    { "file_comment_added",     std::make_pair( rtm_type::file_comment_added,       &recv_message::nop ) },
    { "file_comment_deleted",   std::make_pair( rtm_type::file_comment_deleted,     &recv_message::nop ) },
    { "file_comment_edited",    std::make_pair( rtm_type::file_comment_edited,      &recv_message::nop ) },
    { "file_created",           std::make_pair( rtm_type::file_created,             &recv_message::nop ) },
    { "file_deleted",           std::make_pair( rtm_type::file_deleted,             &recv_message::nop ) },
    { "file_public",            std::make_pair( rtm_type::file_public,              &recv_message::nop ) },
    { "file_shared",            std::make_pair( rtm_type::file_shared,              &recv_message::nop ) },
    { "file_unshared",          std::make_pair( rtm_type::file_unshared,            &recv_message::nop ) },
    { "goodbye",                std::make_pair( rtm_type::goodbye,                  &recv_message::nop ) }, 
    { "group_archive",          std::make_pair( rtm_type::group_archive,            &recv_message::nop ) },
    { "group_close",            std::make_pair( rtm_type::group_close,              &recv_message::nop ) },
    { "group_history_changed",  std::make_pair( rtm_type::group_history_changed,    &recv_message::nop ) },
    { "group_joined",           std::make_pair( rtm_type::group_joined,             &recv_message::nop ) },
    { "group_left",             std::make_pair( rtm_type::group_left,               &recv_message::nop ) },
    { "group_marked",           std::make_pair( rtm_type::group_marked,             &recv_message::nop ) },
    { "group_open",             std::make_pair( rtm_type::group_open,               &recv_message::nop ) },
    { "group_rename",           std::make_pair( rtm_type::group_rename,             &recv_message::nop ) },
    { "group_unarchive",        std::make_pair( rtm_type::group_unarchive,          &recv_message::nop ) },
    { "hello",                  std::make_pair( rtm_type::hello,                    &recv_message::hello_recv ) },
    { "im_close",               std::make_pair( rtm_type::im_close,                 &recv_message::nop ) },
    { "im_cleated",             std::make_pair( rtm_type::im_cleated,               &recv_message::nop ) },
    { "im_history_changed",     std::make_pair( rtm_type::im_history_changed,       &recv_message::nop ) },
    { "im_marked",              std::make_pair( rtm_type::im_marked,                &recv_message::nop ) },
    { "im_open",                std::make_pair( rtm_type::im_open,                  &recv_message::nop ) },
    { "manual_presence_change", std::make_pair( rtm_type::manual_presence_change,   &recv_message::nop ) },
    { "member_joined_channel",  std::make_pair( rtm_type::member_joined_channel,    &recv_message::nop ) },
    { "member_left_channel",    std::make_pair( rtm_type::member_left_channel,      &recv_message::nop ) },
    { "message",                std::make_pair( rtm_type::message,                  &recv_message::message_recv ) },
    { "pin_added",              std::make_pair( rtm_type::pin_added,                &recv_message::nop ) },
    { "pin_removed",            std::make_pair( rtm_type::pin_removed,              &recv_message::nop ) },
    { "pin_change",             std::make_pair( rtm_type::pin_change,               &recv_message::nop ) },
    { "presence_change",        std::make_pair( rtm_type::presence_change,          &recv_message::nop ) },
    { "presence_sub",           std::make_pair( rtm_type::presence_sub,             &recv_message::nop ) },
    { "reaction_added",         std::make_pair( rtm_type::reaction_added,           &recv_message::nop ) },
    { "reaction_removed",       std::make_pair( rtm_type::reaction_removed,         &recv_message::nop ) },
    { "reconnect_uri",          std::make_pair( rtm_type::reconnect_uri,            &recv_message::nop ) },
    { "star_added",             std::make_pair( rtm_type::star_added,               &recv_message::nop ) },
    { "star_removed",           std::make_pair( rtm_type::star_removed,             &recv_message::nop ) },
    { "subteam_created",        std::make_pair( rtm_type::subteam_created,          &recv_message::nop ) },
    { "subteam_members_changed",std::make_pair( rtm_type::subteam_members_changed,  &recv_message::nop ) },
    { "subteam_self_added",     std::make_pair( rtm_type::subteam_self_added,       &recv_message::nop ) },
    { "subteam_self_removed",   std::make_pair( rtm_type::subteam_self_removed,     &recv_message::nop ) },
    { "subteam_updated",        std::make_pair( rtm_type::subteam_updated,          &recv_message::nop ) },
    { "team_domain_change",     std::make_pair( rtm_type::team_domain_change,       &recv_message::nop ) },
    { "team_join",              std::make_pair( rtm_type::team_join,                &recv_message::nop ) },
    { "team_migration_started", std::make_pair( rtm_type::team_migration_started,   &recv_message::nop ) },
    { "team_plan_change",       std::make_pair( rtm_type::team_plan_change,         &recv_message::nop ) },
    { "team_pref_change",       std::make_pair( rtm_type::team_pref_change,         &recv_message::nop ) },
    { "team_profile_change",    std::make_pair( rtm_type::team_profile_change,      &recv_message::nop ) },
    { "team_profile_delete",    std::make_pair( rtm_type::team_profile_delete,      &recv_message::nop ) },
    { "team_profile_reorder",   std::make_pair( rtm_type::team_profile_reorder,     &recv_message::nop ) },
    { "team_rename",            std::make_pair( rtm_type::team_rename,              &recv_message::nop ) },
    { "user_change",            std::make_pair( rtm_type::user_change,              &recv_message::nop ) },
    { "user_typing",            std::make_pair( rtm_type::user_typing,              &recv_message::nop ) }
};

unsigned int send_message::counter = 0;

std::string send_message::get_json(){
    std::stringstream   sstream;
    sstream <<
    "{ \r\n" <<
    "   \"id\": " << counter++ <<
    "   \"type\": \"messsage\", \r\n" <<
    "   \"channel\": \"" << channel << "\"\r\n" <<
    "   \"text\": \"" << text << "\"\r\n" <<
    "}\r\n\r\n";

//    std::cout << "send_message" << std::endl;
//    std::cout << sstream.str();

    return sstream.str();
}

std::string connect_response::format_json( const std::string& instr ){
    size_t front_pos = instr.find_first_of("{");
    size_t last_pos = instr.find_last_of("}");

    std::string json = instr.substr( front_pos, last_pos - front_pos + 1 );
    std::cout << "format json = " << json << std::endl;
    return json;   
}

void connect_response::from_message( std::string mes ){
    boost::property_tree::ptree _ptree;
    std::stringstream _stringstream;

    _stringstream << format_json( mes );

    try {
        boost::property_tree::json_parser::read_json( _stringstream, _ptree );
        if( _ptree.get<bool>("ok") ){
            url = _ptree.get<std::string>("url");
            bot_name = _ptree.get<std::string>("self.name");
            bot_id = _ptree.get<std::string>("self.id");
            for( auto& child : _ptree.get_child("channels") ){
                const boost::property_tree::ptree& info = child.second;
                channel _channel( info.get<std::string>("id"),
                                    info.get<std::string>("name"),
                                    info.get<bool>("is_member") );
                channels.push_back( _channel );
            }

            for( auto& child : _ptree.get_child("users") ){
                const boost::property_tree::ptree& info = child.second;
                user _user( info.get<std::string>("id"),
                            info.get<std::string>("name"));
                users.push_back( _user );
            }
        }
    }
    catch( const boost::property_tree::ptree_error& e ){
        std::cout << "connect_response::ptree_error : " << e.what() << std::endl;
        std::cout << "message : " << _stringstream.str() << std::endl;
    }
}

std::string& connect_response::get_channel_id( const std::string& name ){
    for( auto& channel : channels ){
        if( channel.name == name ) return channel.id;
    }
    throw std::runtime_error("channel name not found");
}

recv_message::recv_message( std::string& mes ){
    boost::property_tree::ptree ptree;
    std::stringstream stringstream;
    
    stringstream << mes;
    try{
        boost::property_tree::json_parser::read_json( stringstream, ptree );

        auto itr = rtm_type_map.find( ptree.get<std::string>( "type" ) );
        if( itr == rtm_type_map.end() ) undefined(this, ptree);
        else {
            type = itr->second.first;
            auto function = itr->second.second;
            function( this, ptree );
        }
    }
    catch( boost::property_tree::ptree_error& e ){
        std::cout << "ptree_error: " << e.what() << std::endl;
        std::cout << "json = " << mes << std::endl;
    }
}

void recv_message::hello_recv( const recv_message* parent, const boost::property_tree::ptree& ptree ){
    std::cout << "hello recv" << std::endl;
}

void recv_message::message_recv( const recv_message* parent, const boost::property_tree::ptree& ptree ){
    std::cout << "hello recv" << std::endl;
}

void recv_message::undefined( const recv_message* parent, const boost::property_tree::ptree& ptree ){
    std::cout << "undefined message" << std::endl;
}

void recv_message::nop( const recv_message* parent, const boost::property_tree::ptree& ptree){
    std::cout << "non operation" << std::endl;
}

