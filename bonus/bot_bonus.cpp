#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits>
#include <signal.h>

int	g_signal = 0;

void	handleSignal(int sig)
{
	static_cast<void>(sig);
	std::cout << '\n';
	g_signal = 130;
}

int	getPort(std::string port)
{
	size_t	pos;

	pos = port.find_first_not_of("0123456789");
	if (pos != std::string::npos)
	{
		if (pos == 0 && port[0] == '-' && port.find_first_not_of("0123456789", pos) == std::string::npos)
			std::cout << "Error: <port> must be a positive integer\n";
		else
			std::cout << "Error: <port> must be numeric\n";
		return (-1);
	}

	int i = std::atoi(port.c_str());

	if (i > 65535)
		return (-1);
	return (i);
}

int	createSocket(void)
{
	struct protoent	*proto;
	int				socketfd;
   	
	proto = getprotobyname("tcp");
	if (!proto)
	{
		std::cout << "Error: cannot get protocol's structure\n";
		return (-1);
	}
	socketfd = socket(AF_INET, SOCK_STREAM, proto->p_proto);
	return (socketfd);
}

int	ft_strlen(char *buf)
{
	int i = 0;

	while (buf[i])
		i++;
	return (i);
}

std::string passwordGenerator(int length)
{
	std::string password;
    
	for (int i = 0; i < length; i++)
		password += (std::rand() % 94 + 32);
	password += '\n';
	return (password);
}   

std::string	processInput(const std::string &input)
{
	std::istringstream	iss(input);
	std::string			message;
	std::string			username;
	std::string			command;
	std::string			value;
	std::string			garbage;
	int					valueInt;

	iss >> username >> command >> value >> garbage;
	message = "PRIVMSG " + username + " :";
	if (garbage.empty() == false || command.empty() == true || value.empty() == true
		|| (command.empty() == false && command.compare(":generate") != 0))
	{
		message += "Usage: /msg passBot :generate <value>\n";
		return (message);
	}
	valueInt = std::atoi(value.c_str());
	if (value.find_first_not_of("0123456789") != std::string::npos
			|| value.length() > 2 || valueInt <= 0 || valueInt > 50)
		message += "Password length must be numeric and between 1 and 50\n";
	else
		message += passwordGenerator(valueInt);
	return (message);
}

void	serverCommunication(int sockfd)
{
	std::string	password;
	std::string servPass = "PASS Uz/}TGO$5vG<QW-2*Rr}=dkKs%[D38@$4j#i3m)O!PlRC5!hgs\n";
	std::string	bufStr;
	char		buf[1024];
	int			ret;

	write(sockfd, servPass.c_str(), servPass.length());
	while (g_signal == 0)
	{
		ret = recv(sockfd, buf, 1024, MSG_DONTWAIT);
		if (ret == -1)
		{
			if (g_signal == 130)
			{
				std::cerr << "passBot has been shutdown\n";
				return ;
			}
			continue ;
		}
		else if (ret == 0)
		{
			std::cerr << "Lost connection with server" << std::endl;
			return ;
		}
		else
			buf[ret] = '\0';
		bufStr = buf;
		if (bufStr == "\002[INFO]\017 passBot is already connected\n")
		{
			std::cout << "Someone stole my identify, I am getting out of here" << std::endl;
			return ;
		}
		password = processInput(bufStr);
		ret = write(sockfd, password.c_str(), password.length());
		if (ret == -1)
		{
			std::cerr << "Error: cannot write on socket\n";
			return ;
		}
	}
}

void	connectToServer(int port)
{
	int					sockfd;
	struct sockaddr_in	serveraddr;

	sockfd = createSocket();
	if (sockfd == -1)
		return ;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&serveraddr), sizeof(serveraddr)) == -1)
		std::cerr << "Error connecting to server\n";
	else
		serverCommunication(sockfd);
	close(sockfd);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./passBot <port>\n";
		return (1);
	}

	int i = getPort(argv[1]);

	if (i == -1)
		return (1);
	else
	{
		signal(SIGINT, handleSignal);
		connectToServer(std::atoi(argv[1]));
	}
	return (0);
}
