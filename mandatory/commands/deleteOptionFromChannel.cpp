#include "Channel.hpp"

void	Channel::_unsetOperatorChannel(std::istringstream & iss, Client const & client)
{
	std::string	username;
	std::string	garbage;

	iss >> username;
	if (iss.eof())
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [-o] <username>\n");
		return ;
	}
	else if (username == "passBot")
	{
		_sendMessageToClient(client, HEX_INFO + " You can only send private messages to passBot\n");
		return ;
	}
	iss >> garbage;
	if (iss.eof() == false)
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [-o] <username>\n");
	else if (isClientMember(username) == false)
		_sendMessageToClient(client, HEX_INFO + " This user is not a member of this channel\n");
	else if (isClientOp(username) == false)
		_sendMessageToClient(client, HEX_INFO + " This user is not an operator of this channel\n");
	else
	{
		deleteOp(username);
		_sendMessageToClient(client, HEX_INFO + " You successfully removed '" + username + "' from operators\n");
	}
}

void	Channel::_deleteOptionFromChannel(std::istringstream & iss, std::string & option, Client const & client)
{
    std::string garbage;

	if (option.find_first_not_of("itkol", 1) != std::string::npos || option.length() > 2)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> {[+|-]i|t|k|o|l} <argument>\n");
		return ;
	}
	else if (option[1] == 'o')
	{
		_unsetOperatorChannel(iss, client);
		return ;
	}

	iss >> garbage;
	if (iss.eof() == false)
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> {[+|-]i|t|k|l} <argument>\n");
	else if (option[1] == 'k')
	{
		_password.clear();
		_modes &= 0 << PASSWORD;
		_sendMessageToClient(client, HEX_INFO + " You successfully turned the password mode off\n");
	}
	else if (option[1] == 'l')
	{
		_modes &= 0 << USER_LIMIT;
		_sendMessageToClient(client, HEX_INFO + " You successfully unset the user limit\n");
	}
	else if (option[1] == 'i')
	{
		_inviteList.clear();
		_modes &= 0 << INVITE;
		_sendMessageToClient(client, HEX_INFO + " You successfully turned the invite mode off\n");
	}
	else if (option[1] == 't')
	{
		 _modes &= 0 << TOPIC;
		_sendMessageToClient(client, HEX_INFO + " You successfully turned the topic mode off\n");
	}
}
