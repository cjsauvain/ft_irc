#ifndef CLIENT_HPP
# define CLIENT_HPP

/*  struct sockaddr_in  */
# include <netinet/in.h>

/*	getprotobyname()	*/
# include <netdb.h>

/*	close()	*/
# include <unistd.h>

# include <iostream>
# include <string>
# include <vector>

/*	Colours	*/
# define RESET "\e[0m"
# define B_HI_RED "\e[1;91m"

class	Client
{
	public:
		Client(void);
		Client(const Client &src);
		Client	&operator=(const Client &src);
		~Client(void);

		void	closeSocket(void) const;
		void	completeInput(const char *);
		void	resetInput(void);
		void	addJoinedChannelName(const std::string &);
		void	leaveChannel(const std::string &);
		bool	isInChannel(const std::string &) const;
		void	clearJoinedChannelsNames(void);

		struct sockaddr_in					getSin(void) const;
		const std::vector<std::string>		&getJoinedChannelsNames(void) const;
		const std::pair<bool, std::string>	&getNickname(void) const;
		const std::pair<bool, std::string>	&getUsername(void) const;

		bool								getPassword(void) const;
		const std::string					&getInput(void)const;
		int									getSocket(void) const;
		bool								getAuthentification(void) const;


		void	setNickname(const std::string &);
		void	setUsername(const std::string &);
		void	setPassword(bool status);
		void	setAuthentification(bool status);
		void	setSocket(int);

	private:
		void	_initSinValues(void);

		std::vector<std::string>		_joinedChannelsNames;
		struct sockaddr_in				_sin;
		std::pair<bool, std::string>	_username;
		std::pair<bool, std::string>	_nickname;
    	bool                          	_password;
		bool							_authentification;
		std::string						_input;
		int								_socket;
};

#endif /*CLIENT_HPP*/
