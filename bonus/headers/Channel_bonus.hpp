#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client_bonus.hpp"
# include "Server_bonus.hpp"

class Server;

typedef enum e_modes
{
	INVITE = 0,
	TOPIC = 1,
	PASSWORD = 2,
	USER_LIMIT = 3,
}	t_modes;

class	Channel
{
	public:
		Channel(void);
		Channel(const Channel &src);
		Channel(const std::string &, const std::string &);
		~Channel(void);

		void	addNewUsername(const std::string &);
		void	deleteUsername(const std::string &);
		void	addOp(const std::string &);
		void	deleteOp(const std::string &);
		void	addInvitedUser(const std::string &);
		void	deleteInvitedUser(const std::string &);
		bool	isClientMember(const std::string &) const;
		bool	isClientOp(const std::string &) const;
		bool	isClientInvited(const std::string &) const;
		void	clearMemberNames(void);
		bool	isBitSet(t_modes option) const;
		void	manageOption(std::istringstream & iss, Client const & client);
		void	manageTopicChannel(std::istringstream & iss, Client const & client);

		const std::vector<std::string>		&getMemberNames(void) const;
		const std::vector<std::string>		&getOps(void) const;
		const std::string					&getName(void) const;
		const std::string					&getPassword(void) const;
		const std::string					&getTopic(void) const;
		int									getUserLimit(void) const;
		int									getNbMembers(void) const;

	private:
		Channel							&operator=(const Channel &src);
		std::vector<std::string>		_memberNames;
		std::vector<std::string>		_inviteList;
		std::vector<std::string>		_allOps;
		std::string						_name;
		std::string						_password;
		std::string						_topic;
		int								_modes;
		int								_userLimit;

    	void		_setPasswordChannel(std::istringstream & iss, Client const & client);
		void		_setOperatorChannel(std::istringstream & iss, Client const & client);
 		bool    	_isLimitValid(std::string const & limitStr);
		void		_setUserLimitChannel(std::istringstream & iss, Client const & client);
		void		_addOptionToChannel(std::istringstream & iss, const std::string & option, \
						Client const & client);
		void		_unsetOperatorChannel(std::istringstream & iss, Client const & client);
		void		_deleteOptionFromChannel(std::istringstream & iss, std::string & option, \
						Client const & client);
		void		_sendMessageToClient(const Client &, const std::string &) const;
};

#endif /*CHANNEL_HPP*/
