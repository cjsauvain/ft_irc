#include "Server.hpp"

void	Server::_pass(std::istringstream & iss, Client & client)
{
	std::string	password;
	std::string	garbage;
	if (client.getPassword() == true)
	{
		_sendMessageToClient(client, HEX_INFO + " You already authentificated\n");
		return ;
	}

	iss >> password;
	iss >> garbage;
	if (iss.eof() != true)
		_sendMessageToClient(client, HEX_INFO + " Password must contain only one word\n");
	else if (password == _password)
	{
		client.setPassword(true);
		_sendMessageToClient(client, HEX_INFO + " Correct password\n");
	}
	else if (password == BOT_PASS)
		_quit(client);
	else
		_sendMessageToClient(client, HEX_INFO + " Incorrect password\n");
}
