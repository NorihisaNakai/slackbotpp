#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <sstream>

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
    size_t pos509 = json.find("\r\n509\r\n");
    if( pos509 != std::string::npos ){
        json.erase( pos509, 5 );
    }
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
    stringstream << format_json( response );
    boost::property_tree::json_parser::read_json( stringstream, ptree );

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

/* see also http://qiita.com/yutwatan/items/36291d3833eb963fa583
{
    "expand":"renderedFields,names,schema,operations,editmeta,changelog,versionedRepresentations",
    "id":"11542",
    "self":"https://www.sdy.co.jp/jira/rest/api/2/issue/11542",
    "key":"DIR-141",
    "fields":
        {
            "issuetype":
                {
                    "self":"https://www.sdy.co.jp/jira/rest/api/2/issuetype/3",
                    "id":"3",
                    "description":"行うべきタスクを表します。",
                    "iconUrl":"https://www.sdy.co.jp/jira/secure/viewavatar?size=xsmall&avatarId=10318&avatarType=issuetype",
                    "name":"タスク",
                    "subtask":false,
                    "avatarId":10318
                },
            "components":[],
            "timespent":null,
            "timeoriginalestimate":null,
            "description":"h2. 背景\n* 幕張可視化はまだSDYに負うところが大きく、メール対応など沿革対応でもいいのでやってほしく、それにお金をつけてくれるとのこと\n* ただし、李さんの契約をうちでつけるのはNG\n* CW->softmo->SDYとする\n* コンサル契約にする\n\nh2. やること\n* 条件を決める：5/19時点でSDYの希望条件をsoftmoへ提示済み、5/21の週にsoftmoから契約書案が来る予定\n* --契約案が来たら、即日共有＆検討で2日以内に回答できるようにする。--\n* --5月中に内容確定させる--\n* 技術サポート契約の案が来たのでレビューする。\n* 金額、期間等の契約内容についてコメントを6/9までに返したい\n　->レビュー1回に2日、2往復したいので、6/9目標\n* 6/12の週中に契約締結まで行けるよう調整する：締結は別チケットを起こす\n\nh2. ゴール\n* 調整が完了し契約する場合は契約関係のチケットを作成する\n* 契約しない場合は、契約しない調整完了でクローズ\n* --調整完了目標5/31--\n* 調整完了目標は6/9\n",
            "project":
                {
                    "self":"https://www.sdy.co.jp/jira/rest/api/2/project/10003",
                    "id":"10003",
                    "key":"DIR",
                    "name":"director",
                    "avatarUrls":
                        {
                            "48x48":"https://www.sdy.co.jp/jira/secure/projectavatar?pid=10003&avatarId=10011",
                            "24x24":"https://www.sdy.co.jp/jira/secure/projectavatar?size=small&pid=10003&avatarId=10011",
                            "16x16":"https://www.sdy.co.jp/jira/secure/projectavatar?size=xsmall&pid=10003&avatarId=10011",
                            "32x32":"https://www.sdy.co.jp/jira/secure/projectavatar?size=medium&pid=10003&avatarId=10011"
                        }
                },
            "customfield_10010":"0|i004u5:i",
            "fixVersions":[],
            "aggregatetimespent":null,
            "resolution":null,
            "timetracking":{},
            "customfield_10005":[
                "com.atlassian.greenhopper.service.sprint.Sprint@28091bc6[id=36,rapidViewId=12,state=CLOSED,name=20170320-20170403,startDate=2017-03-21T12:43:42.231Z,endDate=2017-04-04T00:43:00.000Z,completeDate=2017-04-04T00:27:50.408Z,sequence=36]",
                "com.atlassian.greenhopper.service.sprint.Sprint@47e2b88[id=39,rapidViewId=12,state=CLOSED,name=20170404-20170417,startDate=2017-04-04T00:28:07.614Z,endDate=2017-04-17T12:28:00.000Z,completeDate=2017-04-17T07:17:15.433Z,sequence=39]",
                "com.atlassian.greenhopper.service.sprint.Sprint@2421a236[id=40,rapidViewId=12,state=CLOSED,name=20170417-20170508,startDate=2017-04-17T07:17:35.271Z,endDate=2017-05-06T19:17:00.000Z,completeDate=2017-05-07T05:35:15.741Z,sequence=40]",
                "com.atlassian.greenhopper.service.sprint.Sprint@7b2ab78[id=41,rapidViewId=12,state=CLOSED,name=20170509-201705022,startDate=2017-05-07T05:35:51.264Z,endDate=2017-05-21T17:35:00.000Z,completeDate=2017-05-22T09:34:09.461Z,sequence=41]",
                "com.atlassian.greenhopper.service.sprint.Sprint@67854763[id=42,rapidViewId=12,state=CLOSED,name=20170508-20170522,startDate=2017-05-22T09:37:22.355Z,endDate=2017-06-04T21:37:00.000Z,completeDate=2017-06-07T09:06:59.102Z,sequence=42]",
                "com.atlassian.greenhopper.service.sprint.Sprint@12520270[id=43,rapidViewId=12,state=CLOSED,name=20170606-20170620,startDate=2017-06-07T09:07:30.409Z,endDate=2017-06-20T21:07:00.000Z,completeDate=2017-06-23T00:16:18.518Z,sequence=43]",
                "com.atlassian.greenhopper.service.sprint.Sprint@3b4b321e[id=44,rapidViewId=12,state=CLOSED,name=20170621-20170703,startDate=2017-06-19T00:16:29.121Z,endDate=2017-07-02T12:16:00.000Z,completeDate=2017-07-03T15:35:15.679Z,sequence=44]",
                "com.atlassian.greenhopper.service.sprint.Sprint@29b0a764[id=45,rapidViewId=12,state=CLOSED,name=20170704-20170716,startDate=2017-07-03T15:35:39.657Z,endDate=2017-07-17T03:35:00.000Z,completeDate=2017-07-19T07:59:10.956Z,sequence=45]",
                "com.atlassian.greenhopper.service.sprint.Sprint@6443536e[id=46,rapidViewId=12,state=CLOSED,name=20170717-20170730,startDate=2017-07-19T07:59:50.444Z,endDate=2017-07-30T19:59:00.000Z,completeDate=2017-08-02T09:57:55.747Z,sequence=46]",
                "com.atlassian.greenhopper.service.sprint.Sprint@66008c14[id=47,rapidViewId=12,state=CLOSED,name=20170731-20170813,startDate=2017-08-01T09:58:18.103Z,endDate=2017-08-12T21:58:00.000Z,completeDate=2017-08-14T10:41:24.863Z,sequence=47]",
                "com.atlassian.greenhopper.service.sprint.Sprint@4398908[id=48,rapidViewId=12,state=ACTIVE,name=20170814-20170827,startDate=2017-08-14T10:41:54.575Z,endDate=2017-08-25T22:41:00.000Z,completeDate=<null>,sequence=48]"
                ],
            "customfield_10006":null,
            "attachment":[
                {
                    "self":"https://www.sdy.co.jp/jira/rest/api/2/attachment/10700",
                    "id":"10700",
                    "filename":"技術サポート契約書.docx",
                    "author":{
                        "self":"https://www.sdy.co.jp/jira/rest/api/2/user?username=h.okada",
                        "name":"h.okada",
                        "key":"h.okada",
                        "emailAddress":"h.okada@sdy.co.jp",
                        "avatarUrls":{
                            "48x48":"https://secure.gravatar.com/avatar/9764d2da7767174ac07046d66f52ed47?d=mm&s=48",
                            "24x24":"https://secure.gravatar.com/avatar/9764d2da7767174ac07046d66f52ed47?d=mm&s=24",
                            "16x16":"https://secure.gravatar.com/avatar/9764d2da7767174ac07046d66f52ed47?d=mm&s=16",
                            "32x32":"https://secure.gravatar.com/avatar/9764d2da7767174ac07046d66f52ed47?d=mm&s=32"
                        },
                        "displayName":"Hajime Okada",
                        "active":true,
                        "timeZone":"Asia/Tokyo"
                    },
                    "created":"2017-06-03T05:42:42.000+0000",
                    "size":27393,
                    "mimeType":"application/vnd.openxmlformats-officedocument.wordprocessingml.document",
                    "content":"https://www.sdy.co.jp/jira/secure/attachment/10700/%E6%8A%80%E8%A1%93%E3%82%B5%E3%83%9D%E3%83%BC%E3%83%88%E5%A5%91%E7%B4%84%E6%9B%B8.docx"
                }
            ],
            "aggregatetimeestimate":null,
            "resolutiondate":null,
            "workratio":-1,
            "summary":"可視化：４月コンサル契約条件調整",
            "lastViewed":"2017-08-28T11:11:39.408+0000",
            "watches":{"self":"https://www.sdy.co.jp/jira/rest/api/2/issue/DIR-141/watchers","watchCount":2,"isWatching":true},"creator":{"self":"https://www.sdy.co.jp/jira/rest/api/2/user?username=h.okada","name":"h.okada","key":"h.okada","emailAddress":"h.okada@sdy.co.jp","avatarUrls":
*/

/*
const options = {
    uri: 'https://<jira_hostname>/rest/api/2/issue/TESTPJ-2',
    headers: {
        'Content-Type': 'application/json',
        cookie: session_name + '=' + session_value,
    },
    method: 'GET',
    json: true,
};

request(options, function (error, response, data) {
    if (!error && response.statusCode == 200) {
        console.log('Get issue: ' + data.fields.summary); // --> Get issue: First issue in TESTPJ
    } else {
        console.log('error: ' + response.statusCode + ': ' + data.errorMessages);
    }
});
*/

