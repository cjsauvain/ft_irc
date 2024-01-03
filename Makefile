NAME =	ircserv

NAME_BONUS = ircserv_bonus

NAME_BOT = passBot

SRC =	main.cpp			\
		Server.cpp			\
		Client.cpp			\
		Channel.cpp			\
		detectCommand.cpp	\
		run.cpp				\

SRC_BONUS =	main_bonus.cpp			\
			Server_bonus.cpp		\
			Client_bonus.cpp		\
			Channel_bonus.cpp		\
			detectCommand_bonus.cpp	\
			run_bonus.cpp			\

SRC_BOT = bot_bonus.cpp	\

COMMANDS =	join.cpp					\
			nick.cpp					\
			user.cpp					\
			whois.cpp					\
			displayChannels.cpp			\
			privmsg.cpp					\
			pass.cpp					\
			part.cpp					\
			quit.cpp					\
			kick.cpp					\
			mode.cpp					\
			invite.cpp					\
			addOptionToChannel.cpp		\
			topic.cpp					\
			deleteOptionFromChannel.cpp	\

COMMANDS_BONUS =	join_bonus.cpp						\
					nick_bonus.cpp						\
					user_bonus.cpp						\
					whois_bonus.cpp						\
					displayChannels_bonus.cpp			\
					privmsg_bonus.cpp					\
					pass_bonus.cpp						\
					part_bonus.cpp						\
					quit_bonus.cpp						\
					kick_bonus.cpp						\
					mode_bonus.cpp						\
					invite_bonus.cpp					\
					addOptionToChannel_bonus.cpp		\
					topic_bonus.cpp						\
					deleteOptionFromChannel_bonus.cpp	\

HEADERS =	mandatory/headers/Server.hpp	\
			mandatory/headers/Client.hpp	\
			mandatory/headers/Channel.hpp	\

HEADERS_BONUS =	bonus/headers/Server_bonus.hpp	\
				bonus/headers/Client_bonus.hpp	\
				bonus/headers/Channel_bonus.hpp	\

INCLUDE_FOLDER = -I mandatory/headers/

INCLUDE_FOLDER_BONUS = -I bonus/headers/

SRCS =	$(addprefix mandatory/, $(SRC))					\
		$(addprefix mandatory/commands/, $(COMMANDS))	\

SRCS_BONUS =	$(addprefix bonus/, $(SRC_BONUS))				\
				$(addprefix bonus/commands/, $(COMMANDS_BONUS))	\

SRCS_BOT =	$(addprefix bonus/, $(SRC_BOT))	\

OBJS = $(SRCS:%.cpp=%.o)

OBJS_BONUS = $(SRCS_BONUS:%.cpp=%.o)

OBJS_BOT = $(SRCS_BOT:%.cpp=%.o)

ifdef BONUS
	NAME = $(NAME_BONUS)
	SRCS = $(SRCS_BONUS)
	OBJS = $(OBJS_BONUS)
	INCLUDE_FOLDER = $(INCLUDE_FOLDER_BONUS)
	HEADERS = $(HEADERS_BONUS)
endif

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE_FOLDER) -c $< -o $@

all: $(NAME)

bonus:
	@make BONUS=1
	@make $(NAME_BOT) BONUS=1

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(NAME_BOT): $(OBJS_BOT)
	$(CC) $(OBJS_BOT) -o $(NAME_BOT)

clean:
	rm -rf $(OBJS) $(OBJS_BONUS) $(OBJS_BOT)

fclean: clean
	rm -rf $(NAME) $(NAME_BONUS) $(NAME_BOT)

re: fclean all

.PHONY: all clean fclean re bonus
