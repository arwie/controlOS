#ifndef LOG_HPP_
#define LOG_HPP_

class Log : public Message
{
public:

	template<class Type>
	Log& log(const string& path, const Type& value)
	{
		put(path, value);
		return *this;
	}

	Log& message(const string& message)		{ return log("message", message); }

	Log& channel(const string& channel)		{ return log("channel", channel); }

	Log& file(const string& file)			{ return log("code_file", file); }
	Log& func(const string& func)			{ return log("code_func", func); }


protected:

	Log(const int prio)
	{
		put("priority", prio);
	}

	Log(const int prio, const string& msg)
		: Log(prio)
	{
		message(msg);
	}
};



struct LogError		: public Log {
	LogError()						: Log(priority)			{}
	LogError(const string& msg)		: Log(priority, msg)	{}
	static const int priority = 3;
};

struct LogWarning	: public Log {
	LogWarning()					: Log(priority)			{}
	LogWarning(const string& msg)	: Log(priority, msg)	{}
	static const int priority = 4;
};

struct LogNotice	: public Log {
	LogNotice()						: Log(priority)			{}
	LogNotice(const string& msg)	: Log(priority, msg)	{}
	static const int priority = 5;
};

struct LogInfo		: public Log {
	LogInfo()						: Log(priority)			{}
	LogInfo(const string& msg)		: Log(priority, msg)	{}
	static const int priority = 6;
};

struct LogDebug		: public Log {
	LogDebug()						: Log(priority)			{}
	LogDebug(const string& msg)		: Log(priority, msg)	{}
	static const int priority = 7;
};

#endif /* LOG_HPP_ */
