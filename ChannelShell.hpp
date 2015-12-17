/*
 * ChannelShell.hpp
 *
 *  Created on: 17.12.2015
 *      Author: client
 */

#ifndef CHANNELSHELL_HPP_
#define CHANNELSHELL_HPP_


class ChannelShell : public Channel
{
public:

	void send(MessagePtr& message) override
	{
		if (system(deref(message).get<string>("cmd").c_str()) != 0) {
			throw exception();
		}
	}

};

#endif /* CHANNELSHELL_HPP_ */
