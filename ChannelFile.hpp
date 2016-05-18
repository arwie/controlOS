#ifndef CHANNELFILE_HPP_
#define CHANNELFILE_HPP_


static string formatFileNameDat(const string& name)
{
	return "/FFS0/SSMC/" + name.substr(0, 8) + ".DAT";
}


class ChannelFileRead : public Channel
{
public:
	ChannelFileRead(const Message& args) : Channel(args), stream(formatFileNameDat(args.get<string>("file"))) {}

	bool receive(MessagePtr& message) override
	{
		string str;

		if(!getline(stream, str))
			return false;

		message.reset(new Message(str));
		return true;
	}

private:
	ifstream stream;
};



class ChannelFileWrite : public Channel
{
public:
	ChannelFileWrite(const Message& args) : Channel(args), stream(formatFileNameDat(args.get<string>("file"))) {}

	void send(const Message& message) override
	{
		stream << message.toString() << endl;
	}

private:
	ofstream stream;
};

#endif /* CHANNELFILE_HPP_ */
