#ifndef CHANNELCLIENTCNCHAAS_HPP_
#define CHANNELCLIENTCNCHAAS_HPP_


class ChannelClientCncHaas : public BlockingChannel
{
public:

	ChannelClientCncHaas(const Message& args)
		: Channel(args), BlockingChannel(args),
		  host(args.get<string>("host")),
		  port(args.get<string>("port", "80")),
		  timeoutMs(args.get<int>("timeout", 3000))
	{

		string address = host;
		boost::smatch match;

		// IPv4 syntax
		if (boost::regex_match(address, match, boost::regex("^(.*):(\\d+)$"))) {
			host = match[1];
			port = match[2];
		}

		// UPv6 syntax
		if (boost::regex_match(address, match, boost::regex("^\\[(.*)\\]:(\\d+)$"))) {
			host = match[1];
			port = match[2];
		}
	}


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

protected:
	void logMsg(const Log& msg) override
	{
		BlockingChannel::logMsg(const_cast<Log&>(msg).channel("clientCncHaas").log("host", host).log("port", port));
	}


private:
    boost::asio::io_service asioService;
    string host;
    string port;
    int timeoutMs;


	class Connection : public enable_shared_from_this<Connection>
	{
	public:

		Connection(ChannelClientCncHaas& channel, const Message& message)
			: channel(channel), resolver(channel.asioService), socket(channel.asioService),
			  deadline(channel.asioService, boost::posix_time::milliseconds(channel.timeoutMs)),
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

			parseErrorLog.log("request", string(boost::asio::buffer_cast<const char*>(request.data()), request.size()));
		}

		void execute()
		{
			auto thisPtr = shared_from_this();
			deadline.async_wait([thisPtr](const boost::system::error_code& err) {
				if (err == boost::asio::error::operation_aborted)
					return;

				thisPtr->cancelled = true;
				boost::system::error_code dummyErr;
				thisPtr->socket.close(dummyErr);

				thisPtr->channel.logMsg(LogError("connection timeout").log("timeout", thisPtr->channel.timeoutMs));
			});

			resolve();
		}

	private:

		void resolve()
		{
			auto thisPtr = shared_from_this();
			resolver.async_resolve(boost::asio::ip::tcp::resolver::query(channel.host, channel.port),
					[thisPtr](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
						if (thisPtr->cancelled)
							return;
						if (!err)
							thisPtr->connect(endpoint_iterator);
						else {
							thisPtr->deadline.cancel();
							thisPtr->channel.logMsg(LogError("resolve failed: "+err.message()));
						}
					}
			);
		}

		void connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			auto thisPtr = shared_from_this();
			boost::asio::async_connect(socket, endpoint_iterator,
					[thisPtr](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
						if (thisPtr->cancelled)
							return;
						if (!err)
							thisPtr->write();
						else {
							thisPtr->deadline.cancel();
							thisPtr->channel.logMsg(LogError("connect failed: "+err.message()));
						}
					}
			);
		}

		void write()
		{
			auto thisPtr = shared_from_this();
			boost::asio::async_write(socket, request,
					[thisPtr](const boost::system::error_code& err, std::size_t bytes_transferred) {
						if (thisPtr->cancelled)
							return;
						if (!err)
							thisPtr->read();
						else {
							thisPtr->deadline.cancel();
							thisPtr->channel.logMsg(LogError("write failed: "+err.message()));
						}
					}
			);
		}

		void read()
		{
			auto thisPtr = shared_from_this();
			boost::asio::async_read(socket, response, boost::asio::transfer_at_least(1),
					[thisPtr](const boost::system::error_code& err, std::size_t bytes_transferred) {
						if (thisPtr->cancelled)
							return;
						if (!err)
							thisPtr->read();
						else if (err == boost::asio::error::eof)
							thisPtr->parse();
						else {
							thisPtr->deadline.cancel();
							thisPtr->channel.logMsg(LogError("read failed: "+err.message()));
						}
					}
			);
		}

		void parse()
		{
			deadline.cancel();

			try {
				auto messagePtr = make_unique<Message>();

				parseErrorLog.log("response", string(boost::asio::buffer_cast<const char*>(response.data()), response.size()));

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

				if (!queryQueue.empty())
					throw runtime_error("incomplete query");

				channel.pushMessage(move(messagePtr));
			}
			catch (exception& e) {
				channel.logMsg(parseErrorLog.message("failed to parse response: "+string(e.what())));
			}
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
		LogError parseErrorLog;
	};
};

#endif /* CHANNELCLIENTCNCHAAS_HPP_ */
