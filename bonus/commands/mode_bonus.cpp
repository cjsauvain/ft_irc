#include "Server_bonus.hpp"

void	Server::_mode(std::istringstream & iss, Client const & client)
{
	std::map<std::string, Channel>::iterator	it;
	std::string									channel;

	iss >> channel;
	if (iss.eof())
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> {[+|-]i|t|k|o|l} <optional argument>\n");
		return ;
	}

	if (_isChannelNameValid(channel, client) == false)
		return ;

	it = _allChannels.find(channel);
	if (it == _allChannels.end())
		_sendMessageToClient(client, HEX_INFO + " This channel does not exist\n");
	else
	{
		if (it->second.isClientMember(client.getUsername().second) == false)
			_sendMessageToClient(client, HEX_INFO + " You must be in the channel to execute this command\n");
		else if (it->second.isClientOp(client.getUsername().second) == false)
			_sendMessageToClient(client, HEX_INFO + " You must be operator of the channel to execute this command\n");
		else
			it->second.manageOption(iss, client);
	}
}
