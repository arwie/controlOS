#ifndef CHANNELCLIENTCNCHAAS_HPP_
#define CHANNELCLIENTCNCHAAS_HPP_


class ChannelClientCncHaas : public BlockingChannel
{
public:

	ChannelClientCncHaas(const string& host, int port)
		: host(host), port(to_string(port))
	{}


	void send(const Message& message) override
	{
	    make_shared<Connection>(*this, message)->execute();
	}


	bool needsRunner() override { return true; }
	void run() override
    {
		boost::asio::io_service::work work(asioService);
	    asioService.run();
    }


	void close() override
	{
		asioService.stop();
		BlockingChannel::close();
	}


private:
    boost::asio::io_service asioService;
    string host;
    string port;


	class Connection : public enable_shared_from_this<Connection>
	{
	public:

		Connection(ChannelClientCncHaas& channel, const Message& message)
			: channel(channel), resolver(channel.asioService), socket(channel.asioService),
			  deadline(channel.asioService, boost::posix_time::milliseconds(3000)),
			  dataItemRegex("^Data item (\\d+) \\(length = (\\d+)\\) = \"(.*)\".*$")
		{
			ostream requestStream(&request);
			requestStream << "GET " << "/m-net?mds,1,S";

			for (const auto& top : message) {
				for (const auto& sub : top.second) {
					queryQueue.emplace(top.first, sub.first);

					requestStream << "," << sub.first;
					string value = sub.second.get_value<string>();
					if (value != "?")
						requestStream << "=" << value;
				}
			}

			requestStream << " HTTP/1.0"					<< "\r\n";
			requestStream << "Host: " << channel.host		<< "\r\n";
			requestStream << "Accept: */*"					<< "\r\n";
			requestStream << "Connection: close"			<< "\r\n";
			requestStream 									<< "\r\n";
		}

		void execute()
		{
			auto thisPtr = shared_from_this();
			deadline.async_wait([thisPtr](const boost::system::error_code& err) {
				thisPtr->cancel();
			});

			resolve();
		}

	private:

		void resolve()
		{
			if (cancelled) return;

			auto thisPtr = shared_from_this();
			resolver.async_resolve(boost::asio::ip::tcp::resolver::query(channel.host, channel.port),
					[thisPtr](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
						if (!err) thisPtr->connect(endpoint_iterator); else DEBUG("cncHaas: " << err);
					}
			);
		}

		void connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			if (cancelled) return;

			auto thisPtr = shared_from_this();
			boost::asio::async_connect(socket, endpoint_iterator,
					[thisPtr](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
						if (!err) thisPtr->write(); else DEBUG("cncHaas: " << err);
					}
			);
		}

		void write()
		{
			if (cancelled) return;

			auto thisPtr = shared_from_this();
			boost::asio::async_write(socket, request,
					[thisPtr](const boost::system::error_code& err, std::size_t bytes_transferred) {
						if (!err) thisPtr->read(); else DEBUG("cncHaas: " << err);
					}
			);
		}

		void read()
		{
			if (cancelled) return;

			auto thisPtr = shared_from_this();
			boost::asio::async_read(socket, response, boost::asio::transfer_at_least(1),
					[thisPtr](const boost::system::error_code& err, std::size_t bytes_transferred) {
						if (!err)
							thisPtr->read();
						else if (err == boost::asio::error::eof)
							thisPtr->parse();
						else
							DEBUG("cncHaas: " << err);
					}
			);
		}

		void parse()
		{
			if (cancelled) return;

			try {
				auto messagePtr = make_unique<Message>();

				istream responseStream(&response);
				string line;
				while (getline(responseStream, line)) {
					boost::smatch match;
					if (boost::regex_match(line, match, dataItemRegex))
					{
						const auto& query = queryQueue.front();

						if (stoi(match[1]) != stoi(query.second))
							throw runtime_error("wrong query order");

						messagePtr->put<string>(query.first+"."+query.second, match[3]);
						queryQueue.pop();
					}
				}

				channel.pushMessage(move(messagePtr));
				deadline.cancel();
			}
			catch (exception& e) { DEBUG("cncHaas: " << e.what()); }
		}

		void cancel()
		{
			cancelled = true;
			boost::system::error_code dummyErr;
			socket.close(dummyErr);
		}


		ChannelClientCncHaas& channel;
		boost::asio::ip::tcp::resolver resolver;
		boost::asio::ip::tcp::socket socket;
		boost::asio::deadline_timer deadline;
		boost::asio::streambuf request;
		boost::asio::streambuf response;
		boost::regex dataItemRegex;
		bool cancelled = false;
		queue<pair<string, string>> queryQueue;
	};
};

#endif /* CHANNELCLIENTCNCHAAS_HPP_ */
