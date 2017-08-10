#include "../include/slackmessages.h"

#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

std::map<std::string, rtm_type> rtm_type_map = {
    { "accounts_change", rtm_type::accounts_change },
    { "bot_added", rtm_type::bot_added },
    { "bot_changed", rtm_type::bot_changed },
    { "channel_archive", rtm_type::channel_archive },
    { "channel_created", rtm_type::channel_created },
    { "channel_deleted" , rtm_type::channel_deleted },
    { "channel_history_changed", rtm_type::channel_history_changed },
    { "channel_deleted", rtm_type::channel_deleted },
    { "channel_left", rtm_type::channel_left },
    { "channel_marked", rtm_type::channel_marked },
    { "channel_rename", rtm_type::channel_rename },
    { "channel_unarchive", rtm_type::channel_unarchive },
    { "command_changed", rtm_type::command_changed },
    { "dnd_updated_user", rtm_type::dnd_updated_user },
    { "email_domain_changed", rtm_type:email_domain_changed },
    { "emoji_changed", rtm_type::emoji_changed },
    { "file_change", rtm_type::file_change },
    { "file_comment_added", rtm_type::file_comment_added },
    { "file_comment_deleted", rtm_type::file_comment_deleted },
    { "file_comment_edited", rtm_type::file_comment_edited },
    { "file_created", rtm_type::file_created },
    { "file_deleted", rtm_type::file_deleted },
    { "file_public", rtm_type::file_public },
    { "file_shared", rtm_type::file_shared },
    { "file_unshared", rtm_type::file_unshared },
    { "goodbye", rtm_type::goodbye }, 
    { "group_archive", rtm_type::group_archive },
    { "group_close", rtm_type::group_close },
    { "group_history_changed", rtm_type::group_history_changed },
    { "group_joined", rtm_type::group_joined },
    { "group_left", rtm_type::group_left },
    { "group_marked", rtm_type::group_marked },
    { "group_open", rtm_type::group_open },
    { "group_rename", rtm_type::group_rename },
    { "group_unarchive", rtm_type::group_unarchive },
    { "hello", rtm_type::hello },
    { "im_close", rtm_type::im_close },
    { "im_cleated", rtm_type::im_cleated },
    { "im_history_changed", rtm_type::im_history_changed },
    { "im_marked", rtm_type::im_marked },
    { "im_open", rtm_type::im_open },
    { "manual_presence_change", rtm_type::manual_presence_change },
    { "member_joined_channel", rtm_type::member_joined_channel },
    { "member_left_channel", rtm_type::member_left_channel },
    { "message", rtm_type::message },
    { "pin_added", rtm_type::pin_added },
    { "pin_removed", rtm_type::pin_removed },
    { "pin_change", rtm_type::pin_change },
    { "presence_change", rtm_type::presence_change },
    { "presence_sub", rtm_type::presence_sub },
    { "reaction_added", rtm_type::reaction_added },
    { "reaction_removed", rtm_type::reaction_removed },
    { "reconnect_uri", rtm_type::reconnect_uri },
    { "star_added", rtm_type::star_added },
    { "star_removed", rtm_type::star_removed },
    { "subteam_created", rtm_type::subteam_created },
    { "subteam_members_changed", rtm_type::subteam_members_changed },
    { "subteam_self_added", rtm_type::subteam_self_added },
    { "subteam_self_removed", rtm_type::subteam_self_removed },
    { "subteam_updated", rtm_type::subteam_updated },
    { "team_domain_change", rtm_type::team_domain_change },
    { "team_join", rtm_type::team_join },
    { "team_migration_started", rtm_type::team_migration_started },
    { "team_plan_change", rtm_type::team_plan_change },
    { "team_pref_change", rtm_type::team_pref_change },
    { "team_profile_change", rtm_type::team_profile_change },
    { "team_profile_delete", rtm_type::team_profile_delete },
    { "team_profile_reorder", rtm_type::team_profile_reorder },
    { "team_rename", rtm_type::team_rename },
    { "user_change", rtm_type::user_change },
    { "user_typing", rtm_type::user_typing }
};

unsigned int send_message::counter = 0;

void connect_response::from_message( std::string mes ){
    boost::property_tree::ptree _ptree;
    std::stringstream _stringstream;
    _stringstream << mes;
    boost::property_tree::json_parser::read_json( _stringstream, _ptree );
    if( _ptree.get<bool>("ok") ){
        url = _ptree.get<std::string>("url");
        bot_name = _ptree.get<std::string>("self.name");
        bot_id = _ptree.get<std::string>("self.id");
        for( auto& child : _ptree.get_child("channnels") ){
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

recv_message::recv_message( std::string& mes ){
    boost::property_tree::ptree _ptree;
    std::stringstream _stringstream;
    _stringstream << mes;
    boost::property_tree::json_parser::read_json( _stringstream, _ptree );

    auto itr = rtm_type_map.find( _ptree.get<std::string>( "type" ) );
    if( itr == rtm_type_map.end() ) type = rtm_type::undefined_type;
    else type = itr->second;

    ts = _ptree.get<std::string>( "ts" );
    user = _ptree.get<std::string>( "user" );
    text = _ptree.get<std::string>( "text" );
}