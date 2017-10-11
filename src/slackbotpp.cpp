#include "../include/singleton.h"
#include "../include/slackbotpp.h"
#include "../include/slackbot.h"

int main( int argc, char* argv[] ){
	try{
		auto& property = singleton<bot_property>::get();
		slack_bot		bot(
			property.get_slack_token(),
			property.get_slack_host(),
			property.get_jira_host(),
			property.get_jira_user(),
			property.get_jira_password()
		);
		bot.get_rtm_url();
		bot.start();
	}
	catch( std::exception& e){
		std::cout << e.what() << std::endl;
	}
	singleton_finalizer::finalize();
	return 0;
}
