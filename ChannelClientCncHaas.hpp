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
		stringstream pathStream;
	    pathStream << "/m-net?mds,1,S";
		for (auto& kv : message) {
			pathStream << "," << kv.first;
			string value = kv.second.get_value<string>();
			if (!value.empty())
				pathStream << "=" << value;
		}

	    make_shared<Connection>(*this, pathStream.str())->execute();
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

		Connection(ChannelClientCncHaas& channel, const string& path)
			: channel(channel), resolver(channel.asioService), socket(channel.asioService),
			  dataItemRegex("^Data item (\\d+) \\(length = (\\d+)\\) = \"(.*)\".*$")
		{
			ostream requestStream(&request);
			requestStream << "GET " << path << " HTTP/1.0"	<< "\r\n";
			requestStream << "Host: " << channel.host		<< "\r\n";
			requestStream << "Accept: */*"					<< "\r\n";
			requestStream << "Connection: close"			<< "\r\n";
			requestStream 									<< "\r\n";
		}

		void execute()
		{
			resolve();
		}

		~Connection() { DEBUG("connection deleted"); }

	private:

		void resolve()
		{
			DEBUG("resolving");
			auto thisPtr = shared_from_this();
			resolver.async_resolve(boost::asio::ip::tcp::resolver::query(channel.host, channel.port),
					[thisPtr](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
						if (!err) thisPtr->connect(endpoint_iterator); else DEBUG(err);
					}
			);
		}

		void connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			DEBUG("connecting");
			auto thisPtr = shared_from_this();
			boost::asio::async_connect(socket, endpoint_iterator,
					[thisPtr](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
						if (!err) thisPtr->write(); else DEBUG(err);
					}
			);
		}

		void write()
		{
			DEBUG("writing");
			auto thisPtr = shared_from_this();
			boost::asio::async_write(socket, request,
					[thisPtr](const boost::system::error_code& err, std::size_t bytes_transferred) {
						if (!err) thisPtr->read(); else DEBUG(err);
					}
			);
		}

		void read()
		{
			DEBUG("reading");
			auto thisPtr = shared_from_this();
			boost::asio::async_read(socket, response, boost::asio::transfer_at_least(1),
					[thisPtr](const boost::system::error_code& err, std::size_t bytes_transferred) {
						if (!err)
							thisPtr->read();
						else if (err == boost::asio::error::eof)
							thisPtr->parse();
						else
							DEBUG(err);
					}
			);
		}

		void parse()
		{
			DEBUG("parsing");
			auto messagePtr = make_unique<Message>();

			istream responseStream(&response);
			string line;
			while (getline(responseStream, line)) {
				boost::smatch match;
				if (boost::regex_match(line, match, dataItemRegex))	{
					//for (unsigned int i=0; i<match.size(); ++i) { DEBUG("[" << match[i] << "] "); };
					messagePtr->put<string>(to_string(stoi(match[1])), match[3]);
				}
			}

			channel.pushMessage(move(messagePtr));
		}


		ChannelClientCncHaas& channel;
		boost::asio::ip::tcp::resolver resolver;
		boost::asio::ip::tcp::socket socket;
		boost::asio::streambuf request;
		boost::asio::streambuf response;
		boost::regex dataItemRegex;
	};
};

#endif /* CHANNELCLIENTCNCHAAS_HPP_ */
