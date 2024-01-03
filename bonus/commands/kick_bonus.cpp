#include "Server_bonus.hpp"
#include <limits>

/*
Hexchat sends:
KICK localhost #channel :user reason reason reason reason

KICK IRC #channel :username reason oui non peut-etre
*/

void	Server::_kickUserFromChannel(Client &kicked, Channel &channel, const std::string &reason)
{
	std::string	kickedUsername = kicked.getUsername().second;
	std::string	message;

	message = HEX_INFO + " You have been kicked from the channel '" + channel.getName() + "'";
	if (reason.length() > 0)
		message += " for the following reason: " + reason + '\n';
	else
		message += '\n';
	kicked.leaveChannel(channel.getName());
	channel.deleteUsername(kickedUsername);
	if (channel.isClientOp(kickedUsername) == true)
		channel.deleteOp(kickedUsername);
	_sendMessageToClient(kicked, message);
	message = HEX_INFO + " User '" + kickedUsername + "' has been kicked from the channel '" + channel.getName() + "'";
	if (reason.length() > 0)
		message += " for the following reason: " + reason + '\n';
	else
		message += '\n';
	_sendMessageToChannel(channel, message);
}

void	Server::_kick(std::istringstream &iss, Client &client)
{
	std::string	channelName;
	std::string	username;
	std::string	reason;
	int			clientIndex;

	iss.ignore(std::numeric_limits<std::streamsize>::max(), '#'); // no '#' detected
	if (iss.eof() == true)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /kick <#channel> :<username> <optional reason>\n");
		return ;
	}
	iss >> channelName;
	channelName.insert(0, 1, '#');
	
	if (_doesChannelExist(client, channelName) == false)
		return ;
	else if (_allChannels[channelName].isClientOp(client.getUsername().second) == false)
	{
		_sendMessageToClient(client, HEX_INFO + " You must be operator of the channel to execute this command\n");
		return ;
	}

	iss.ignore(std::numeric_limits<std::streamsize>::max(), ':'); // no ':' detected
	if (iss.eof() == true)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /kick <#channel> :<username> <optional reason>\n");
		return ;
	}
	iss >> username;
	if (username == "passBot")
	{
		_sendMessageToClient(client, HEX_INFO + " You can only send private messages to passBot\n");
		return ;
	}
	if (_allChannels[channelName].isClientMember(username) == false)
	{
		_sendMessageToClient(client, HEX_INFO + " This user '" + username + "' is not a member of the channel '" + channelName + "'\n");
		return ;
	}
	iss.ignore(1); // jumps the space after username
	reason = _extractString(iss);
	clientIndex = _getClientIndex(username);
	_kickUserFromChannel(_allClients[clientIndex], _allChannels[channelName], reason);
}
