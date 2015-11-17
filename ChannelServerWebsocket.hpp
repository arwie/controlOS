#ifndef CHANNELSERVERWEBSOCKET_HPP_
#define CHANNELSERVERWEBSOCKET_HPP_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


class ChannelServerWebsocket : public ChannelMessageQueue
{
	using WsServer = websocketpp::server<websocketpp::config::asio>;

public:
	ChannelServerWebsocket(int port)
	{
		wsServer.init_asio();

        wsServer.set_open_handler(bind(&ChannelServerWebsocket::on_open, this, placeholders::_1));
        wsServer.set_close_handler(bind(&ChannelServerWebsocket::on_close, this, placeholders::_1));
        wsServer.set_message_handler(bind(&ChannelServerWebsocket::on_message, this, placeholders::_1, placeholders::_2));

    	wsServer.listen(websocketpp::lib::asio::ip::tcp::v4(), port);
     	wsServer.start_accept();
	}

	bool needsRunner() override { return true; }
	void run() override
    {
        wsServer.run();
    }

	int connected() const
	{

	}


	void send()
	{

	}

	void close()
	{

	}

private:
    void on_open(websocketpp::connection_hdl hdl)
    {
    	cerr << "connected" << endl;
    	connections.insert(hdl);
    }

    void on_close(websocketpp::connection_hdl hdl)
    {
    	cerr << "disconnected" << endl;
    	connections.erase(hdl);
    }

    void on_message(websocketpp::connection_hdl hdl, WsServer::message_ptr msg)
    {
    	cerr << "message: " << msg->get_payload() << endl;

		pushMessage(MessagePtr(new Message(msg->get_payload())));
    }

    WsServer wsServer;
    set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;
};
#endif /* CHANNELSERVERWEBSOCKET_HPP_ */
