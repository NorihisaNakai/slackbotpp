#ifndef SLACKBOTPP_H
#define SLACKBOTPP_H
#include <string>
#include <fstream>
#include <yaml-cpp/yaml.h>

class bot_property {
public:
    bot_property() :
        node( YAML::LoadFile( "./config.yml" ) ){}

    ~bot_property(){
        save();
    }
    std::string get_slack_token(){ return node["notification"]["slack_token"].as<std::string>(); }
    std::string get_slack_host(){ return node["notification"]["slack_host"].as<std::string>(); }
    std::string get_jira_host(){ return node["jira"]["host"].as<std::string>(); }
    std::string get_jira_user(){ return node["jira"]["username"].as<std::string>(); }
    std::string get_jira_password(){ return node["jira"]["password"].as<std::string>(); }
protected:
    YAML::Node  node;
    void    save(){
        std::fstream ofs("./config.yml");
        ofs << node;
    }
};

#endif  //SLACKBOTPP_H
