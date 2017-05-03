// Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef CHANNELWEBSOCKET_HPP_
#define CHANNELWEBSOCKET_HPP_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


class ChannelWebsocket : public QueuingChannel
{
public:

	ChannelWebsocket(const Message& args)
		: Channel("websocket", args), QueuingChannel(args)
	{
		port				= args.get<int>("port");
		auto receive		= args.get<bool>("receive", true);

		log.put("port", port);
		log.put("receive", receive);

		wsServer.init_asio();
		wsServer.set_reuse_addr(true);
		wsServer.clear_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);

		wsServer.set_open_handler([this](websocketpp::connection_hdl hdl)
		{
			{ lock_guard<mutex> lock(connectionsMtx);
				connections.insert(hdl);
			}
			pushMessage(make_unique<Message>(Message::Event::connect));
		});

		wsServer.set_close_handler([this](websocketpp::connection_hdl hdl)
		{
			{ lock_guard<mutex> lock(connectionsMtx);
				connections.erase(hdl);
			}
			if (connections.empty())
				pushMessage(make_unique<Message>(Message::Event::disconnect));
		});

		if (receive) {
			wsServer.set_message_handler([this](websocketpp::connection_hdl hdl, WsServer::message_ptr msg)
			{
				pushMessage(make_unique<Message>(msg->get_payload()));
			});
		}
	}

	void open() override
	{
		wsServer.listen(websocketpp::lib::asio::ip::tcp::v4(), port);
		wsServer.start_accept();
		QueuingChannel::open();
	}


	bool needsRunner() override { return true; }
	void run() override
    {
        wsServer.run();
    }


	void send(const Message& message) override
	{
    	auto msgString = message.toString();

    	lock_guard<mutex> lock(connectionsMtx);
		for(auto& con : connections) {
			try {
				wsServer.send(con, msgString, websocketpp::frame::opcode::text);
			} catch (websocketpp::exception& e) {
				logMsg(LogWarning(e.what()).func(__func__));
			}
		}
	}


	void close() override
	{
		wsServer.stop();
		QueuingChannel::close();
	}


private:

	using WsServer = websocketpp::server<websocketpp::config::asio>;

	int port;
	WsServer wsServer;
	set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;
	mutex connectionsMtx;
};

#endif /* CHANNELWEBSOCKET_HPP_ */
