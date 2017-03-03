/*
* Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class Message : public boost::property_tree::ptree
{
public:
	enum Event {
		message			= 1,
		connect			= 10,
		disconnect		= 11,
	};


	Message() {}
	Message(const Event event) : event(event) {}
	Message(const boost::property_tree::ptree& rhs) : boost::property_tree::ptree(rhs) {}

	Message(const string& json)
	{
		stringstream ss(json);
		boost::property_tree::read_json(ss, *static_cast<boost::property_tree::ptree*>(this));
	}

	string toString() const
	{
		stringstream ss;
		boost::property_tree::write_json(ss, *static_cast<const boost::property_tree::ptree*>(this), false);

		string json;
		getline(ss, json);

		return json;
	}

	void with(const string& with)
	{
		if (with.empty())
			prefix.clear();
		else
			prefix = with + ".";
	}

	template<class Type>
	void put_with(const string& path, const Type& value)
	{
		put(prefix+path, value);
	}

	template<class Type>
	Type get_with(const string& path) const
	{
		return get<Type>(prefix+path);
	}

	void erase_with(string path)
	{
    	path = prefix+path;
		auto lastDot = path.rfind(".");

		if (lastDot != string::npos)
			get_child(path.substr(0, lastDot)).erase(path.substr(lastDot+1));
		else
			erase(path);
	}

	const boost::property_tree::ptree& get_child_with(const string& path) const
	{
		return get_child(prefix+path);
	}

	void put_child_with(const string& path, const boost::property_tree::ptree& child)
	{
		put_child(prefix+path, child);
	}


	Event event = Event::message;


private:
	string prefix;
};

#endif /* MESSAGE_HPP_ */
