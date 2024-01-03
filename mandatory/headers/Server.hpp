#ifndef SERVER_HPP
# define SERVER_HPP

/*	select()	*/
# include <map>
# include <sstream>

# include "Channel.hpp"

/*	Hexchat text formatting	*/
# define HEX_INFO std::string("\x02[INFO]\x0F")
# define HEX_BOLD std::string("\x02")
# define HEX_RESET std::string("\x0F")
# define BOT_PASS "Uz/}TGO$5vG<QW-2*Rr}=dkKs%[D38@$4j#i3m)O!PlRC5!hgs"

/*	Colours	*/
# define B_HI_GREEN "\e[1;92m"

# define MAX_PENDING_CON 10 //max pending connections, second argument of the listen function
# define BUFFER_SIZE 1024

extern int	g_signal;

class Channel;

class	Server
{
	public:
		Server(void);
		~Server(void);

		void	start(int, std::string &);
		bool	run(void);


		int		getSocket(void) const;

	private:
		Server(const Server &src);

		Server	&operator=(const Server &src);

		void	_initSinValues(void);
		void	_createSocket(void);
		void	_setSockOptReuseAddr(void) const;
		void	_bindSocket(void) const;
		void	_listenSocket(void) const;
		void	_closeSocket(void) const;
		void	_shutdownServer(void);

		void	_receiveData(int);
		void	_acceptNewClient(void);
		void	_processInput(int, const char *);
		void	_detectCommand(Client &);
		void	_sendMessageToClient(const Client &, const std::string &) const;
		void	_sendMessageToChannel(const Channel &, const std::string &) const;
		void	_sendMessage(const Client &, const Client &, const std::string &) const;
		void	_join(std::istringstream &, Client &);
		void	_createChannel(const std::string &, Client &);
		void	_displayClient(const Client &, const std::string &) const;
		void	_displayAllClients(std::istringstream &, const Client &) const;
		void	_displayChannels(Client &) const;

		void	_nick(std::istringstream &, Client &);

		void	_user(std::istringstream &, Client &);
		void	_userHexchat(std::istringstream &, Client &);
		bool	_isUsernameAlreadyTaken(const std::string &) const;

		void	_pass(std::istringstream &, Client & client);

		void	_whois(std::istringstream &, const Client &) const;

		void	_displayChannels(std::istringstream &, const Client &) const;
		bool	_isChannelNameValid(const std::string &, const Client &) const;

		void		_privmsg(std::istringstream &, const Client & client) const;
		void		_sendToBot(std::istringstream &, const Client &) const;
		void		_sendToClient(std::istringstream &, const std::string &, const Client &) const;
		void		_sendToChannel(std::istringstream &, const std::string &, const Client &) const;
		std::string	_extractString(std::istringstream &) const;

		void	_part(std::istringstream &, Client &);
		bool	_doesChannelExist(const Client &, const std::string &) const;
		void	_removeClientFromChannel(Client &, Channel &);

		void	_quit(Client &);
		void	_disconnectClientFromAllChannels(const std::vector<std::string> &, const std::string &);

		void	_kick(std::istringstream &, Client &);
		void	_kickUserFromChannel(Client &, Channel &, const std::string &);

		void	_invite(std::istringstream &, Client &);

		void		_unsetOperatorChannel(std::istringstream & iss, Channel & channel, Client const & client);
		void		_deleteOptionFromChannel(std::istringstream & iss, std::string & option, Channel & channel, Client const & client);
		void		_setPasswordChannel(std::istringstream & iss, Channel & channel, Client const & client);
		void		_setUserLimitChannel(std::istringstream & iss, Channel & channel, Client const & client);
		void		_addOptionToChannel(std::istringstream & iss, std::string & option, Channel & channel, Client const & client);
		void		_mode(std::istringstream & iss, Client const & client);
		void		_topic(std::istringstream & iss, Client const & client);

		int				_getClientIndex(int) const;
		int				_getClientIndex(const std::string &) const;

		void			_setPassword(const std::stringstream &, Client &);
		void			_setUsername(const std::stringstream &, Client &);
		void			_setNickname(const std::stringstream &, Client &);
		void			_setMaxFd(void);

		std::map<std::string, Channel>	_allChannels;
		std::vector<Client>				_allClients;
		struct sockaddr_in				_sin;
		std::string						_password;
		fd_set							_readfds;
		int								_socket;
		int								_port;
		int								_maxFd;
	
	/*	START OF EXCEPTIONS	*/
	class	Error : public std::exception
	{
		public:
			Error(const std::string &) throw();
			~Error(void) throw();
			const char	*what(void) const throw();

		private:
			const std::string	_errMsg;
	};
	/*	END OF EXCEPTIONS*/
};

#endif /*SERVER_HPP*/
