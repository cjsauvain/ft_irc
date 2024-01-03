#include "Server.hpp"

void	Server::_displayChannels(std::istringstream &iss, const Client &client) const
{
	std::map<std::string, Channel>::const_iterator	it = this->_allChannels.begin();
	std::string										message;
	std::string										garbage;

	iss >> garbage;
	if (iss.eof() != true)
		_sendMessageToClient(client, HEX_INFO + " Usage: /channels\n");
	else if (it == this->_allChannels.end())
		_sendMessageToClient(client, HEX_INFO + " There is currently no existing channel\n");
	else
	{
		message = HEX_INFO + " List of all channels currently existing:\n";
		while (it != this->_allChannels.end())
		{
			message += "- " + it->first;
			if (it->second.isBitSet(INVITE) == true)
				message += " - invite only";
			if (it->second.isBitSet(TOPIC) == true)
				message += " - topic";
			if (it->second.isBitSet(USER_LIMIT) == true)
				message += " - user limit";
			if (it->second.isBitSet(PASSWORD) == true)
				message += " - protected";
			message += '\n';
			it++;
		}
		_sendMessageToClient(client, message);
	}
}
