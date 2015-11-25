#ifndef CHANNELSERVERWEBSOCKET_HPP_
#define CHANNELSERVERWEBSOCKET_HPP_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


class ChannelServerWebsocket : public ChannelFifo
{
public:

	ChannelServerWebsocket(int port)
	{
		wsServer.init_asio();

        wsServer.set_open_handler(bind(&ChannelServerWebsocket::on_open, this, placeholders::_1));
        wsServer.set_close_handler(bind(&ChannelServerWebsocket::on_close, this, placeholders::_1));
        wsServer.set_message_handler(bind(&ChannelServerWebsocket::on_message, this, placeholders::_1, placeholders::_2));

    	wsServer.listen(websocketpp::lib::asio::ip::tcp::v4(), port);
     	wsServer.start_accept();

     	setConnected(false);
	}


	bool needsRunner() override { return true; }
	void run() override
    {
        wsServer.run();
    }


	void send(MessagePtr& message) override
	{
    	auto msgString = deref(message).toString();

    	lock_guard<mutex> lock(connectionsMtx);
		for(auto& con : connections) {
			wsServer.send(con, msgString, websocketpp::frame::opcode::text);
		}
	}


	void close() override
	{
		wsServer.stop();
		ChannelFifo::close();
	}


private:

	using WsServer = websocketpp::server<websocketpp::config::asio>;


    void on_open(websocketpp::connection_hdl hdl)
    {
    	{ lock_guard<mutex> lock(connectionsMtx);
    		connections.insert(hdl);
    	}
    	setConnected(connections.size());
    }

    void on_close(websocketpp::connection_hdl hdl)
    {
    	{ lock_guard<mutex> lock(connectionsMtx);
    		connections.erase(hdl);
    	}
    	setConnected(connections.size());
    }

    void on_message(websocketpp::connection_hdl hdl, WsServer::message_ptr msg)
    {
		pushMessage(make_unique<Message>(msg->get_payload()));
    }


    WsServer wsServer;
    set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;
    mutex connectionsMtx;
};

#endif /* CHANNELSERVERWEBSOCKET_HPP_ */
