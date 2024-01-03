#include "Channel.hpp"
#include <cstdlib>

void	Channel::_setPasswordChannel(std::istringstream & iss, Client const & client)
{
	std::string password;
	std::string	garbage;

	iss >> password;
	if (iss.eof() == true)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [+k] <password>\n");
		return ;
	}
	iss >> garbage;
	if (iss.eof() == false)
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [+k] <password>\n");
	else
	{
		_password = password;
		_modes |= 1 << PASSWORD;
		_sendMessageToClient(client, HEX_INFO + " You successfully turned the password mode on\n");
	}
}

void	Channel::_setOperatorChannel(std::istringstream & iss, Client const & client)
{
	std::string	username;
	std::string	garbage;

	iss >> username;
	if (iss.eof())
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [+o] <username>\n");
		return ;
	}
	else if (username == "passBot")
	{
		_sendMessageToClient(client, HEX_INFO + " You can only send private messages to passBot\n");
		return ;
	}

	iss >> garbage;
	if (iss.eof() == false)
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [+o] <username>\n");
	else if (isClientMember(username) == false)
		_sendMessageToClient(client, HEX_INFO + " This user is not a member of this channel\n");
	else if (isClientOp(username) == true)
		_sendMessageToClient(client, HEX_INFO + " This user is already an operator of this channel\n");
	else
	{
		addOp(username);
		_sendMessageToClient(client, HEX_INFO + " You successfully turned '" + username + "' into operator\n");
	}
}

bool	Channel::_isLimitValid(std::string const & limitStr)
{
	if (limitStr[0] == '0' && limitStr.length() == 1)
		return (false);
	for (size_t i = 0; i < limitStr.length(); i++)
	{
		if (std::isdigit(limitStr[i]) == 0)
			return (false);
	}
	return (true);
}

void	Channel::_setUserLimitChannel(std::istringstream & iss, Client const & client)
{
	std::string	limitStr;
	std::string	garbage;
	int			limitInt;

	iss >> limitStr;
	if (iss.eof())
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [+l] <limit>\n");
		return ;
	}
	iss >> limitStr;
	if (iss.eof() == false)
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> [+l] <limit>\n");
	else if (_isLimitValid(limitStr) == false)
		_sendMessageToClient(client, HEX_INFO + " The limit must be a valid number between 1 and 50\n");
	else
	{
		limitInt = std::atoi(limitStr.c_str());
		if (limitInt > 50 || limitStr.length() > 2)
			_sendMessageToClient(client, HEX_INFO + " The limit cannot exceed 50\n");
		else
		{
			_modes |= 1 << USER_LIMIT;
			_userLimit = limitInt; 
			_sendMessageToClient(client, HEX_INFO + " You successfully set the user limit to " + \
				limitStr + "\n");
		}
	}
}

void	Channel::_addOptionToChannel(std::istringstream & iss, const std::string & option, Client const & client)
{
	std::string	garbage;

	if (option.length() > 2 || option.find_first_not_of("itkol", 1) != std::string::npos)
		_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> {[+|-]i|t|k|o|l} <optional argument>\n");
	else if (option[1] == 'k')
		_setPasswordChannel(iss, client);
	else if (option[1] == 'o')
		_setOperatorChannel(iss, client);
	else if (option[1] == 'l')
		_setUserLimitChannel(iss, client);
	else if (option[1] == 'i' || option[1] == 't')
	{
		iss >> garbage;
		if (iss.eof() == false)
			_sendMessageToClient(client, HEX_INFO + " Usage: MODE <#channel> {[+]i|t}\n");
		else if (option[1] == 'i')
		{
			_sendMessageToClient(client, HEX_INFO + " You successfully turned the invite mode on\n");
			_modes |= 1 << INVITE;
		}
		else
		{
			_sendMessageToClient(client, HEX_INFO + " You successfully turned the topic mode on\n");
			_modes |= 1 << TOPIC;
		}
	}
}
