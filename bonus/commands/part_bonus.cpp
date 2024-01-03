#include "Server_bonus.hpp"

void	Server::_removeClientFromChannel(Client &client, Channel &channel)
{
	std::string	channelName	= channel.getName();
	std::string	clientUsername = client.getUsername().second;

	if (channel.isClientOp(clientUsername) == true)
		channel.deleteOp(clientUsername);

	client.leaveChannel(channelName);
	channel.deleteUsername(clientUsername);

	if (channel.getMemberNames().size() == 0)
		_allChannels.erase(channelName);
	else
		_sendMessageToChannel(channel, HEX_INFO + " User '" + clientUsername + "' left the channel '" + channelName + "'\n");

	_sendMessageToClient(client, HEX_INFO + " You left the channel '" + channelName + "'\n");
}

bool	Server::_doesChannelExist(const Client &client, const std::string &channelName) const
{
	std::map<std::string, Channel>::const_iterator	it = _allChannels.find(channelName);
	
	if (it == _allChannels.end()) //channel does not exist
	{
		_sendMessageToClient(client, HEX_INFO + " This channel does not exist\n");
		return (false);
	}
	return (true);
}

void	Server::_part(std::istringstream &iss, Client &client)
{
	std::string	channelName;
	std::string	garbage;

	iss >> channelName;
	if (iss.eof() == true)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /part <#channel>\n");
		return ;
	}

	iss >> garbage;
	if (iss.eof() == false)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /part <#channel>\n");
		return ;
	}

	if(_isChannelNameValid(channelName, client) == false || _doesChannelExist(client, channelName) == false)
		return ;
	else if (client.isInChannel(channelName) == false)
		_sendMessageToClient(client, HEX_INFO + " You can not leave a channel you have not joined previously\n");
	else
		_removeClientFromChannel(client, _allChannels[channelName]);
}
