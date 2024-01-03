#include "Server.hpp"

void	Server::_whois(std::istringstream &iss, const Client &client) const
{
	std::string id;
	std::string	garbage;

	iss >> id;
	iss >> garbage;
	if (iss.eof() != true)
		_sendMessageToClient(client, HEX_INFO + " Usage: /whois <username>\n");
	else
		_displayClient(client, id);
}
