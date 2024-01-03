#include "Server.hpp"

void	Server::_nick(std::istringstream &iss, Client &client)
{
	std::string	nickname;
	std::string	username;
	std::string	skip;
	bool		carriageFound = false;

	if (client.getPassword() != true)
	{
		_sendMessageToClient(client, HEX_INFO + " You must enter the server's password\n");
		return ;
	}
	iss >> nickname;
	if (iss.peek() == '\r')
	{
		carriageFound = true;
		iss.ignore(2);
	}
	iss >> skip;
	if (iss.eof() == true) // command is '/nick <nickname>\r\n'
	{
		if (nickname == client.getNickname().second)
			_sendMessageToClient(client, HEX_INFO + " This is already your nickname\n");
		else
		{
			client.setNickname(nickname);
			_sendMessageToClient(client, HEX_INFO + " Your nickname have been updated to '" + nickname + "'\n");
		}
	}
	else if (carriageFound == true && skip.compare("USER") == 0) // command is '/nick <nickname>\r\nUSER' from Hexchat
	{
		client.setNickname(nickname);
		_sendMessageToClient(client, HEX_INFO + " Your nickname have been updated to '" + nickname + "'\n");
		_userHexchat(iss, client);
	}
	else // command is '/nick <nickname> <something unexpected>'
		_sendMessageToClient(client, HEX_INFO + " Usage: /nickname <nickname>\n");
}
