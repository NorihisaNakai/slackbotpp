#ifndef JIRA_H
#define JIRA_H

#include <string>

struct jira_issue {
    std::string key;
    std::string summary;
};


class jira_core {
public:
    jira_core( std::string hostname, std::string auth_user, std::string auth_password ) :
        host( hostname ), username( auth_user), password( auth_password)
        {}

    jira_issue get_issue( std::string ticket );
    bool       create_issue( jira_issue );
    bool       delete_issue( jira_issue );

private:
    std::string host;
    std::string username;
    std::string password;
    std::string session_name;
    std::string session_id;
    void        login();
};

#endif //JIRA_H