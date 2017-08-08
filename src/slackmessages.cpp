#include "slackmessages.h"

#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

std::map<std::string, rtm_types> rtm_type_map = {
    { "account_change", rtm_types::account_change },
    { "bot_added", rtm_types::bot_added },
    { "bot_changed", rtm_types::bot_changed },
    { "channel_archive", rtm_types::channel_archive },
    { "channel_created", rtm_types::channel_created },
    { "channel_deleted" , rtm_types::channel_deleted },
    { "channel_history_changed", rtm_types::channel_history_changed },
    { "channel_deleted", rtm_types::channel_deleted },
    { "channel_left", rtm_types::channel_left },
    { "channel_marked", rtm_types::channel_marked },
    { "channel_rename", rtm_types::channel_rename },
    { "channel_unarchive", rtm_types::channel_unarchive },
    { "command_changed", rtm_types::command_changed },
    { "dnd_updated_user", rtm_types::dnd_updated_user },
    { "email_domain_changed", rtm_types:email_domain_changed: },
    { "emoji_changed", rtm_types::emoji_changed },
    { "file_change", rtm_types::file_change },
    { "file_comment_added", rtm_types::file_comment_added },
    { "file_comment_deleted", rtm_types::file_comment_deleted },
    { "file_comment_edited", rtm_types::file_comment_edited },
    { "file_created", rtm_types::file_created },
    { "file_deleted", rtm_types::file_deleted },
    { "file_public", rtm_types::file_public },
    { "file_shared", rtm_types::file_shared },
    { "file_unshared", rtm_types::file_unshared },
    { "goodbye", rtm_types::goodbye }, 
    { "group_archive", rtm_types::group_archive },
    { "group_close", rtm_types::group_close },
    { "group_history_changed", rtm_types::group_history_changed },
    { "group_joined", rtm_types::group_joined },
    { "group_left", rtm_types::group_left },
    { "group_marked", rtm_types::group_marked },
    { "group_open", rtm_types::group_open },
    { "group_rename", rtm_types::group_rename },
    { "group_unarchive", rtm_types::group_unarchive },
    { "hello", rtm_types::hello },
    { "im_close", rtm_types::im_close },
    { "im_cleated", rtm_types::im_cleated },
    { "im_history_changed", rtm_types::im_history_changed },
    { "im_marked", rtm_types::im_marked },
    { "im_open", rtm_types::im_open },
    { "manual_presence_change", rtm_types::manual_presence_change },
    { "member_joined_channel", rtm_types::member_joined_channel },
    { "member_left_channel", rtm_types::member_left_channel },
    { "message", rtm_types::message },
    { "pin_added", rtm_types::pin_added },
    { "pin_removed", rtm_types::pin_removed },
    { "pin_change", rtm_types::pin_change },
    { "presence_change", rtm_types::presence_change },
    { "presence_sub", rtm_types::presence_sub },
    { "reaction_added", rtm_types::reaction_added },
    { "reaction_removed", rtm_types::reaction_removed },
    { "reconnect_uri", rtm_types::reconnect_uri },
    { "star_added", rtm_types::star_added },
    { "star_removed", rtm_types::star_removed },
    { "subteam_created", rtm_types::subteam_created },
    { "subteam_members_changed", rtm_types::subteam_members_changed },
    { "subteam_self_added", rtm_types::subteam_self_added },
    { "subteam_self_removed", rtm_types::subteam_self_removed },
    { "subteam_updated", rtm_types::subteam_updated },
    { "team_domain_change", rtm_types::team_domain_change },
    { "team_join", rtm_types::team_join },
    { "team_migration_started", rtm_types::team_migration_started },
    { "team_plan_change", rtm_types::team_plan_change },
    { "team_pref_change", rtm_types::team_pref_change },
    { "team_profile_change", rtm_types::team_profile_change },
    { "team_profile_delete", rtm_types::team_profile_delete },
    { "team_profile_reorder", rtm_types::team_profile_reorder },
    { "team_rename", rtm_types::team_rename },
    { "user_change", rtm_types::user_change },
    { "user_typing", rtm_types::user_typing }
};

void connect_response::from_message( std::string mes ){
    boost::property_tree::ptree _ptree;
    std::stringstream _stringstream;
    _stringstream << mes;
    boost::property_tree::json_parser::read_json( _stringstream, _ptree );
    if( ptree.get<bool>("ok") ){
        url = ptree.get<std::string>("url");
        bot_name = ptree.get<std::string>("self.name");
        bot_id = ptree.get<std::string>("self.id");
        for( auto& child : ptree.get_child("channnels") ){
            const boost::property_tree::ptree& info = child.second;
            channel _channel( info.get<std::string>("id"),
                                info.get<std::string>("name"),
                                info.get<bool>("is_member") );
            channnels.push_back( _channel );
        }

        for( auto& child : ptree.get_child("users") ){
            const boost::property_tree::ptree& info = child.second;
            user _user( info.get<std::string>("id"),
                        info.get<std::string("name"));
            users.push_back( user );
        }
}