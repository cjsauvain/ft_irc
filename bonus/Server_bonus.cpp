#include "Server_bonus.hpp"

Server::Server(void): _password(""), _socket(0), _port(0), _maxFd(0), _botConnected(false)
{

}

Server::Server(const Server &src)
{
	*this = src;
}

Server::~Server(void)
{
	this->_closeSocket();
	for (std::vector<Client>::size_type i = 0; i < this->_allClients.size(); i++)
	{
		this->_allClients[i].closeSocket();
	}
	std::cout << "Server has been turned off" << std::endl;
}

Server	&Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->_allChannels = src._allChannels;
		this->_allClients = src._allClients;
		this->_sin = src._sin;
		this->_password = src._password;
		this->_readfds = src._readfds;
		this->_socket = src._socket;
		this->_port = src._port;
		this->_maxFd = src._maxFd;
		this->_botConnected = src._botConnected;
	}
	return (*this);
}

	/*	START OF PUBLIC METHODS	*/
void	Server::start(int port, std::string &password)
{
	this->_port = port;
	this->_password = password;
	this->_initSinValues();

	this->_createSocket();
	
	this->_maxFd = this->_socket;

	FD_ZERO(&this->_readfds);
	FD_SET(this->_socket, &this->_readfds);
	
	this->_setSockOptReuseAddr(); 
	this->_bindSocket();
	this->_listenSocket();

	std::cout << B_HI_GREEN << "Your server has been successfully created" << RESET << std::endl;
}
	/*	END OF PUBLIC METHODS	*/

	/*	START OF PRIVATE METHODS	*/
void	Server::_initSinValues(void)
{
	this->_sin.sin_family = AF_INET;
	this->_sin.sin_port = htons(this->_port);
	this->_sin.sin_addr.s_addr = INADDR_ANY;
}

void	Server::_createSocket(void)
{
	struct protoent	*proto = getprotobyname("tcp");

	if (proto == NULL)
		throw Error("Failed getprotobyname()");

	this->_socket = socket(AF_INET, SOCK_STREAM, proto->p_proto);

	if (this->_socket == -1)
		throw Error("Failed socket()");
}

void	Server::_setSockOptReuseAddr(void) const
{
	int	reuse = 1;

	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
	{
		this->_closeSocket();
		throw Error("Failed setsockopt()");
	}
}

void	Server::_bindSocket(void) const
{
	if (bind(this->_socket, reinterpret_cast<const struct sockaddr *>(&this->_sin), sizeof(this->_sin)) == -1)
	{
		this->_closeSocket();
		throw Error("Failed bind()");
	}
}

void	Server::_listenSocket(void) const
{
	if (listen(this->_socket, MAX_PENDING_CON) == -1)
	{
		this->_closeSocket();	
		throw Error("Failed listen()");
	}
}

void	Server::_closeSocket(void) const
{
	if (close(this->_socket) == -1)
		std::cerr << B_HI_RED << "Error:\n" << RESET << "Failed close(server.socket)" << std::endl;
}

void	Server::_acceptNewClient(void)
{
	Client				newClient;
	struct sockaddr_in	clientSin = newClient.getSin();
	unsigned int		clientSinLength = sizeof(clientSin);
	int					clientSocket;

	clientSocket = accept(this->_socket, reinterpret_cast<struct sockaddr *>(&clientSin), &clientSinLength);
	if (this->_socket == -1)
	{
		this->_closeSocket();
		std::cerr << "Failed accept()" << std::endl;
		return ;
	}
	newClient.setSocket(clientSocket);

	this->_allClients.push_back(newClient);

	FD_SET(clientSocket, &this->_readfds);
	this->_maxFd = (clientSocket > this->_maxFd) ? clientSocket : this->_maxFd;
	std::cout << "New client added" << std::endl;
}

void	Server::_processInput(int socket, const char *buffer)
{
	int	i = this->_getClientIndex(socket);
	if (i == -1)
	{
		std::cerr << "Failed to get client's index" << std::endl;
		return ;
	}
	else if (buffer[0] == '\0') // received '\0', lost connection with this socket
	{
		std::cout << "Lost connection with user '" + _allClients[i].getUsername().second + "'" << std::endl;
		_quit(this->_allClients[i]);
		return ;
	}

	this->_allClients[i].completeInput(buffer);

	std::string::size_type j = this->_allClients[i].getInput().find_first_of('\n', 0);
	if (j != std::string::npos) // command has a '\n' thus can be executed
	{
		this->_detectCommand(this->_allClients[i]);
		if (static_cast<std::vector<Client>::size_type>(i) < this->_allClients.size() && this->_allClients[i].getSocket() == socket)
			this->_allClients[i].resetInput();
	}
}

void	Server::_sendMessageToClient(const Client &client, const std::string &message) const
{
	if (send(client.getSocket(), message.c_str(), message.length(), MSG_NOSIGNAL) < 0)
		std::cerr << "Failed send()" << std::endl;
}

void	Server::_sendMessageToChannel(const Channel &channel, const std::string &message) const
{
	for (std::vector<Client>::size_type i = 0; i < _allClients.size(); i++)
	{
		if (_allClients[i].isInChannel(channel.getName()) == true)
		{
			if (send(_allClients[i].getSocket(), message.c_str(), message.length(), MSG_NOSIGNAL) < 0)
				std::cerr << "Failed send()" << std::endl;
		}	
	}
}

void	Server::_displayClient(const Client &client, const std::string &username) const
{
	std::string	message;
	int i = _getClientIndex(username);

	if (i == -1)
	{
		std::cerr << "Failed to get client's index" << std::endl;
		_sendMessageToClient(client, HEX_INFO + " User '" + username + "' does not exist\n");
		return ;
	}

	Client	toFind = this->_allClients[i];
	const std::vector<std::string>	joinedChannels = toFind.getJoinedChannelsNames();

	if (toFind.getUsername().first == true)
	{
		message = "---------------------------------------------------------------\n";
		message += HEX_BOLD + "Username: " + HEX_RESET + toFind.getUsername().second + '\n';
		if (toFind.getNickname().first == true)
			message += HEX_BOLD + "Nickname: " + HEX_RESET + toFind.getNickname().second + '\n';
		if (joinedChannels.size() != 0)
		{
			message += HEX_BOLD + "Channels joined: " + HEX_RESET + joinedChannels[0];
			for (std::vector<std::string>::size_type i = 1; i < joinedChannels.size(); i++)
			{
				message += ", " + joinedChannels[i];
			}
			message += '\n';
		}
		message += "---------------------------------------------------------------\n";
		_sendMessageToClient(client, message);
	}
}

void	Server::_displayAllClients(std::istringstream &iss, const Client &client) const
{
	std::string	message;
	std::string	garbage;

	iss >> garbage;
	if (iss.eof() == false)
	{
		_sendMessageToClient(client, HEX_INFO + " Usage: /users\n");
		return ;
	}
	message = HEX_INFO + " List of all users currently authentified:\n";
	for (std::vector<Client>::const_iterator it = _allClients.begin(); it != _allClients.end(); it++)
	{
		if (it->getAuthentification() == false)
			continue ;
		else
			_displayClient(client, it->getUsername().second);
	}
}
	/*	END OF PRIVATE METHODS	*/

	/*	START OF GETTERS	*/
int	Server::getSocket(void) const
{
	return (this->_socket);
}

int	Server::_getClientIndex(int socket) const
{
	for (std::vector<Client>::size_type i = 0; i < this->_allClients.size(); i++)
	{
		if (this->_allClients[i].getSocket() == socket)
			return (i);
	}
	return (-1);
}

int	Server::_getClientIndex(const std::string &username) const
{
	for (std::vector<Client>::size_type i = 0; i < this->_allClients.size(); i++)
	{
		if (this->_allClients[i].getUsername().second == username)
			return (i);
	}
	return (-1);
}
	/*	END OF GETTERS*/

	/*	START OF SETTERS	*/
void	Server::_setMaxFd(void)
{
	if (this->_allClients.size() == 0)
		this->_maxFd = this->_socket;
	for (std::vector<Client>::size_type i = 0; i < this->_allClients.size(); i++)
	{
		this->_maxFd = (this->_allClients[i].getSocket() > this->_maxFd) ? this->_allClients[i].getSocket() : this->_maxFd;
	}
}
	/*	END OF SETTERS	*/

	/*	START OF EXCEPTIONS	*/
Server::Error::Error(const std::string &str) throw(): _errMsg(str)
{

}

Server::Error::~Error(void) throw()
{

}

const char	*Server::Error::what(void) const throw()
{
	return (this->_errMsg.c_str());
}
	/*	END OF EXCEPTIONS	*/
