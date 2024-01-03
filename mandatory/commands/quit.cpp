#include "Server.hpp"

void	Server::_disconnectClientFromAllChannels(const std::vector<std::string> &joinedChannelsNames, const std::string &username)
{
	for (std::vector<std::string>::size_type i = 0; i < joinedChannelsNames.size(); i++)
	{
		if (_allChannels[joinedChannelsNames[i]].isClientOp(username) == true)
			_allChannels[joinedChannelsNames[i]].deleteOp(username);

		_allChannels[joinedChannelsNames[i]].deleteUsername(username);

		if (_allChannels[joinedChannelsNames[i]].getMemberNames().size() == 0)
			_allChannels.erase(joinedChannelsNames[i]);
		else
			_sendMessageToChannel(this->_allChannels[joinedChannelsNames[i]], HEX_INFO + " User '" + username + "' has disconnected from this channel\n");
	}
}

void	Server::_quit(Client &client)
{
	int	i = _getClientIndex(client.getUsername().second);

	std::cout << "Client '" << client.getUsername().second << "' has been disconnected from the server" << std::endl;
	this->_disconnectClientFromAllChannels(client.getJoinedChannelsNames(), client.getUsername().second);
	FD_CLR(client.getSocket(), &this->_readfds);
	client.closeSocket();
	this->_allClients.erase(this->_allClients.begin() + i);
	this->_maxFd = 0;
	this->_setMaxFd();
}
