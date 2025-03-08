IRC 

# ft_irc

This is a group project for School 42 whose members are:
- Kévin Brousse
- Jordan Sauvain

ft_irc is an irc chat based on Hexchat that supports the creation of channels, the role of operator, allows you to restrict the access to said channels using password or invite-mode only. It also provides a bot that generates passwords (bonus part)

## Commands

### PASS
```
Enter the server's password.
```
### NICK
```
Set/Modify your nickname.
```
### USER
```
Set your username.
```
### PRIVMSG
```
Send a message to a user/channel.
```
### JOIN
```
Create/Join a channel.
```
### WHOIS
```
Get informations about the specified user.
```
### CHANNELS
```
Display all existing channels.
```
### USERS
```
Display all users.
```
### PART
```
Leave the specified channel.
```
### KICK
```
Kick the specified user from a channel. Must be channel operator to do this.
```
### MODE {[+|-]i|t|k|o|l}
```
  i : Set Invite Mode on/off.
  t : Set Topic Mode bit on/off.
  k : Set Password Mode bit on/off.
  l : Set User Limit Mode bit on/off.
```
### INVITE
```
Invite the specified user in the specified channel.
```
### TOPIC
```
Set/Modify the specified channel's topic.
```
### TOPIC
```
Set/Modify the specified channel's topic.
```
