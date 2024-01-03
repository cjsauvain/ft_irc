#include "Server_bonus.hpp"

/*	atoi()	*/
#include <cstdlib>

/*	signal()	*/
#include <signal.h>

int	g_signal = 0;

void	handleSignal(int sig)
{
	static_cast<void>(sig);
	std::cout << '\n';
	g_signal = 130;
}

int	isPortValid(std::string &portStr)
{
	if (portStr.length() > 5)
		return (-1);
	for (std::string::size_type i = 0; i < portStr.length(); i++)
	{
		if (isdigit(portStr[i]) == 0)
			return (-1);
	}

	int	portInt = atoi(portStr.c_str());

	if (portInt > 65535)
		return (-1);
	return (portInt);
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << B_HI_RED << "Error:\n" << RESET
				<< "Correct usage is <./ircserv> <port> <password>"
				<< std::endl;
		return (1);
	}

	std::string			portStr = argv[1];
	int					portInt;

	portInt = isPortValid(portStr);
	if (portInt < 1024)
	{
		std::cerr << B_HI_RED << "Error:\n" << RESET << "<port> must be an numeric value between 1024 and 65535" << std::endl;
		return (1);
	}

	std::string			password;
	std::string			garbabe;
	std::istringstream	iss(argv[2]);

	iss >> password;
	iss >> garbabe;
	if (garbabe.empty() == false)
	{
		std::cerr << B_HI_RED << "Error:\n" << RESET << "<password> must contain only one word" << std::endl;
		return (1);
	}

	Server		server;

	signal(SIGINT, handleSignal);

	try
	{
		server.start(portInt, password);
	}
	catch (std::exception &exception)
	{
		std::cerr << exception.what() << std::endl;
		return (1);
	}
	if (server.run() == false)
		return (1);
	return (g_signal);
}
