#ifndef SLACKMESSAGE_H
#define SLACKMESSAGE_H

#include <string>
#include <vector>
#include <utility>
#include <functional>

#include <boost/property_tree/ptree.hpp>

enum rtm_type{
    accounts_change,
    bot_added,
    bot_changed,
    channel_archive,
    channel_created,
    channel_deleted,
    channel_history_changed,
    channel_left,
    channel_marked,
    channel_rename,
    channel_unarchive,
    command_changed,
    dnd_updated_user,
    email_domain_changed,
    emoji_changed,
    file_change,
    file_comment_added,
    file_comment_deleted,
    file_comment_edited,
    file_created,
    file_deleted,
    file_public,
    file_shared,
    file_unshared,
    goodbye,
    group_archive,
    group_close,
    group_history_changed,
    group_joined,
    group_left,
    group_marked,
    group_open,
    group_rename,
    group_unarchive,
    hello,
    im_close,
    im_cleated,
    im_history_changed,
    im_marked,
    im_open,
    manual_presence_change,
    member_joined_channel,
    member_left_channel,
    message,
    pin_added,
    pin_removed,
    pin_change,
    presence_change,
    presence_sub,
    reaction_added,
    reaction_removed,
    reconnect_uri,
    star_added,
    star_removed,
    subteam_created,
    subteam_members_changed,
    subteam_self_added,
    subteam_self_removed,
    subteam_updated,
    team_domain_change,
    team_join,
    team_migration_started,
    team_plan_change,
    team_pref_change,
    team_profile_change,
    team_profile_delete,
    team_profile_reorder,
    team_rename,
    user_change,
    user_typing,
    undefined_type
};

struct user {
    user( std::string _id, std::string _name ) :
        id( _id ),
        name( _name ){}
    std::string id;
    std::string name;
};

struct channel {
    channel( std::string _id, std::string _name, bool flg ) :
        id( _id ),
        name( _name ),
        member_flg( flg )
    {}
    std::string id;
    std::string name;
    bool        member_flg;
};

struct connect_response {
    bool                    status;
    std::string             bot_name;
    std::string             bot_id;
    std::vector<user>       users;
    std::vector<channel>    channels;
    std::string             url;
    std::string             format_json( const std::string& );
    void                    from_message( std::string );
    std::string&            get_channel_id( const std::string& );
};

class recv_message {
public:
    recv_message( std::string& );
    ~recv_message(){}
    static void    hello_recv( const recv_message*, const boost::property_tree::ptree& );
    static void    message_recv( const recv_message*, const boost::property_tree::ptree& );
    static void    undefined( const recv_message*, const boost::property_tree::ptree& );
    static void    nop( const recv_message*, const boost::property_tree::ptree& );
    rtm_type    type;
    std::string ts;
    std::string user;
    std::string channel;
    std::string text;    
};

class  send_message {
public:
    send_message(){}
    ~send_message(){}
    static unsigned int counter;
    std::string         channel;
    std::string         text;
    std::string         get_json();
};

#endif  //SLACKMESSAGE_H