#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class Message : public boost::property_tree::ptree
{
public:
	Message() {}

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
		boost::property_tree::ptree::put(prefix+path, value);
	}

	template<class Type>
	Type get_with(const string& path) const
	{
		return boost::property_tree::ptree::get<Type>(prefix+path);
	}

	void erase_with(string path)
	{
    	path = prefix+path;
		auto lastDot = path.rfind(".");

		if (lastDot != string::npos)
			boost::property_tree::ptree::get_child(path.substr(0, lastDot)).erase(path.substr(lastDot+1));
		else
			boost::property_tree::ptree::erase(path);
	}

private:
	string prefix;
};

using MessagePtr = unique_ptr<Message>;

#endif /* MESSAGE_HPP_ */
