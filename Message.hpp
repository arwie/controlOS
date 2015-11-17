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
		boost::property_tree::read_json(ss, *dynamic_cast<boost::property_tree::ptree*>(this));
	}


	string toString() const
	{
		stringstream ss;
		boost::property_tree::write_json(ss, *dynamic_cast<const boost::property_tree::ptree*>(this), false);
		return ss.str();
	}
};
#endif /* MESSAGE_HPP_ */
