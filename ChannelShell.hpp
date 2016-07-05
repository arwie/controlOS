/*
* Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
*/

#ifndef CHANNELSHELL_HPP_
#define CHANNELSHELL_HPP_


class ChannelShell : public Channel
{
public:
	ChannelShell(const Message& args) : Channel("shell", args)	{}

	void send(const Message& message) override
	{
		if (system(message.get<string>("cmd").c_str()) != 0) {
			throw exception();
		}
	}

};

#endif /* CHANNELSHELL_HPP_ */
