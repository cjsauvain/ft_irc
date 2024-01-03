#include "Server_bonus.hpp"

/*	std::strncmp()	*/
#include <cstring>

void	Server::_receiveData(int socket)
{
	if (socket == this->_socket) // new connection
		_acceptNewClient();
	else // fd has sent information 
	{
		char    buffer[BUFFER_SIZE];
		int     howManyBitsRead;
		
		howManyBitsRead = recv(socket, buffer, BUFFER_SIZE, 0);
		if (howManyBitsRead == -1)
		{
			std::cerr << "Failed recv()" << std::endl;
			return ;
		}
		else if (howManyBitsRead >= 0)
		{
			buffer[howManyBitsRead] = '\0';
			_processInput(socket, buffer);
		}
	}
}

bool	Server::run(void)
{
	while (g_signal == 0)
	{
		fd_set	readfds = _readfds;

		if (select(_maxFd + 1, &readfds, NULL, NULL, NULL) < 0)
		{
			if (g_signal != 130)
				std::cerr << "Failed select()" << std::endl;
			return (false);
		}
		for (int i = 0; i <_maxFd + 1; i++)
		{
			if (FD_ISSET(i, &readfds) == 1)
				_receiveData(i);
		}
	}
	return (true);
}
