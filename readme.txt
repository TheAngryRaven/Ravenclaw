Creator:
	Raven (ID: 22885233 | 512388)
	
Licenses:
	RavenClaw: http://opensource.org/licenses/GPL-3.0
	Cryptopp: http://www.boost.org/users/license.html
	
Description:
	A C++ bot base for palringo. It has a handfull of many basic operations.
	A library called crypto++ is required for the bot to function, until I feel that I have polished the bot well enough, you must find and compile this yourself. Until I feel it is polished enough for a true "release" I dont want the bot to be "noob" friendly.
	
	If anyone knows C++ to a well enough extent that they feel they can help, I would be glad to help them get the library up and running, so that they can help with the project. I don't ask that you know much, but if you have even a primitive understanding of c++ to the point you can handle pointers (I am far from an expert myself) I would be greatfull of your help.
	
	This isn't as feature rich as some of the C# bots, but uses C++ which makes it much more challenging and fun to program for. Plus since its c++ you could integrate all kinds of cool scripting languages like python.
	
	Right now this is more or less a bot base, once polished, i will also release the source for a bot that does other things besides core features as a fork.
	
	
Features:
	Basic chat listening, 
		Can listen for keywords in the chat and respond
		
	PM Responses,
		When the bot receives a pm, it PMs the bot owner with the message, then the bot owner can reply back using the bot to the original sender. But can be modified for listening to PMs like it listens to chats.
		
	Full admin functions,
		If given elevated premissions can mod/mute/ban etc etc. 
		
	Bot Admin status,
		Default status is offline, when the bot sees that the bot admin has typed a message in a group the bot is in it sets the admins status to online.
		
		Programatically the bot owner can set keywords where the bot pms them saything they're being looked for. Usefull for checking if the admin the bot belongs to is online.

TODO:
	Random bot crash,
		not sure whats causing it
	
	Image sending,
		i've got the packets needed to send, just need to figure out how to seralize the image.
	
	Sub Profile Reading,
		this handles all the usernames and statuses and such
		
	Clean up code,
		its currently very messy and somewhat unorganized.
		
	Make the bot scrypted,
		No one wants to recompile the bot, the goal is once the base is nailed out and polished, impiliment python or another scripting language as a frontend, and keep RavenClaw as a backend, that only needs to be compiled for new features and bugs.