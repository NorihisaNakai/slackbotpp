#ifndef SLACKMESSAGE_H
#define SLACKMESSAGE_H

#include <std::string>
#include <std::vector>

struct user {
    user( std::string _id, std::string _name ) {
        id( _id ),
        name( _name )
    };
    std::string id;
    std::string name;
};

struct channel {
    channel( std::string _id, std::string name, bool flg ){
        id( _id ),
        name( _name ),
        member_flg( flg )
    };
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
    void                    from_message( std::string );
}



enum rtm_types{
    account_change,
    bot_added,
    bot_changed,
    channel_archive,
    channel_created,
    channel_deleted,
    channel_history_changed,
    channel_deleted,
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
};





#endif  //SLACKMESSAGE_H