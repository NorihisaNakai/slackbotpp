#include "../include/slackbotpp.h"
#include "../include/slackbot.h"

int main( int argc, char* argv[] ){
	try{
		bot_property	property;
		slack_bot		bot( property.get_slack_token(), property.get_slack_host() );
		bot.get_rtm_url();
		bot.start();
	}
	catch( std::exception& e){
		std::cout << e.what() << std::endl;
	}
	return 0;
}
