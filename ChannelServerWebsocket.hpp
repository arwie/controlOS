#ifndef CHANNELSERVERWEBSOCKET_HPP_
#define CHANNELSERVERWEBSOCKET_HPP_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


class ChannelServerWebsocket : public ConnectingChannel
{
public:

	ChannelServerWebsocket(int port)
	{
		wsServer.init_asio();

		wsServer.set_open_handler([this](websocketpp::connection_hdl hdl)
		{
			{ lock_guard<mutex> lock(connectionsMtx);
				connections.insert(hdl);
			}
			setConnected(!connections.empty());
		});

		wsServer.set_close_handler([this](websocketpp::connection_hdl hdl)
		{
			{ lock_guard<mutex> lock(connectionsMtx);
				connections.erase(hdl);
			}
			setConnected(!connections.empty());
		});

		wsServer.set_message_handler([this](websocketpp::connection_hdl hdl, WsServer::message_ptr msg)
		{
			pushMessage(make_unique<Message>(msg->get_payload()));
		});

		wsServer.set_reuse_addr(true);
		wsServer.listen(websocketpp::lib::asio::ip::tcp::v4(), port);
		wsServer.start_accept();

		wsServer.clear_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);

		setConnected(false);
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
			wsServer.send(con, msgString, websocketpp::frame::opcode::text);
		}
	}


	void close() override
	{
		wsServer.stop();
		BlockingChannel::close();
	}


	//~ChannelServerWebsocket() { DEBUG("~ChannelServerWebsocket"); }

private:

	using WsServer = websocketpp::server<websocketpp::config::asio>;

	WsServer wsServer;
	set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;
	mutex connectionsMtx;
};

#endif /* CHANNELSERVERWEBSOCKET_HPP_ */
