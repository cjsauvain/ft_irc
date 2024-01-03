#include "Server_bonus.hpp"
#include <limits>

/*
TOPIC #chan :les oiseaux migrateurs
*/

void	Channel::manageTopicChannel(std::istringstream & iss, Client const & client)
{
	std::string	subject;
	std::string	leftovers;

	iss >> subject;
	if (iss.eof() == true) // displaying the topic
	{
		if (_topic.empty() == true)
			_sendMessageToClient(client, HEX_BOLD + "[" + _name + "]" + HEX_RESET + \
				" The topic has not been set yet\n");
		else
			_sendMessageToClient(client, HEX_BOLD + "[" + _name + "]" + HEX_RESET + \
				" TOPIC: " + _topic + "\n");
	}
	else // modifying the topic
	{
		if (subject[0] != ':')
		{
			_sendMessageToClient(client, HEX_INFO + " Usage: /topic <#channel> :<new subject>\n");
			return ;
		}
		std::getline(iss, leftovers);
		subject.erase(0, 1);
		subject.append(leftovers);
		_topic = subject;
		_sendMessageToClient(client, HEX_INFO + " You successfully updated the topic\n");
	}
}

void	Server::_topic(std::istringstream & iss, Client const & client)
{
	std::map<std::string, Channel>::iterator	it;
	std::string									channelStr;

	iss >> channelStr;
	if (iss.eof() == true)
		_sendMessageToClient(client, HEX_INFO + " Usage: /topic <#channel> <optional new subject>\n");
	else if (_isChannelNameValid(channelStr, client) == true)
	{
		it = _allChannels.find(channelStr);
		if (it == _allChannels.end())
			_sendMessageToClient(client, HEX_INFO + " The channel '" + channelStr + "' does not exist\n");
		else if (it->second.isClientMember(client.getUsername().second) == false)
			_sendMessageToClient(client, HEX_INFO + " You are not part of '" + channelStr + "'\n");
		else if (it->second.isBitSet(TOPIC) == true
				&& it->second.isClientOp(client.getUsername().second) == false)
			_sendMessageToClient(client, HEX_INFO + " You must be operator of the channel to update the topic\n");
		else
			it->second.manageTopicChannel(iss, client);
	}
}
