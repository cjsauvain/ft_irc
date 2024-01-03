#include "Server_bonus.hpp"

void	Server::_invite(std::istringstream &iss, Client &client)
{
	std::map<std::string, Channel>::iterator	it;
	std::string	channel;
	std::string	username;
	std::string	garbage;

	iss >> channel;
	iss >> username;
	if (iss.eof() == true)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /invite <#channel> <username>\n");
		return ;
	}
	else if (username == "passBot")
	{
		_sendMessageToClient(client, HEX_INFO + " You can only send private messages to passBot\n");
		return ;
	}

	iss >> garbage;
	if (iss.eof() == false)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /invite <#channel> <username>\n");
		return ;
	}
	else if (_isChannelNameValid(channel, client) == false)
		return ;
	
	it = _allChannels.find(channel);

	if (it == _allChannels.end() || client.isInChannel(channel) == false)
		_sendMessageToClient(client, HEX_INFO + " You can only invite someone in a channel you are in\n");
	else if (_allChannels[channel].isClientOp(client.getUsername().second) == false)
		_sendMessageToClient(client, HEX_INFO + " You must be operator of the channel to execute this command\n");
	else if (_allChannels[channel].isBitSet(INVITE) == false)
		_sendMessageToClient(client, HEX_INFO + " You can only invite people to join a channel that is on invite mode only\n");
	else if (_isUsernameAlreadyTaken(username) == false)
		_sendMessageToClient(client, HEX_INFO + " Unknown username\n");
	else if (_allChannels[channel].isClientInvited(username) == true)
		_sendMessageToClient(client, HEX_INFO + " This user has already been invited\n");
	else if (_allChannels[channel].isClientMember(username) == true)
		_sendMessageToClient(client, HEX_INFO + " This user is already member of this channel\n");
	else
	{
		_allChannels[channel].addInvitedUser(username);
		_sendMessageToClient(client, HEX_INFO + " You invited '" + username + "' to join the channel '" + channel + "'\n");
		int	i = _getClientIndex(username);
		_sendMessageToClient(_allClients[i], HEX_INFO + " You have been invited by '" + client.getUsername().second + "' to join the channel '" + channel + "'\n");
	}
}
