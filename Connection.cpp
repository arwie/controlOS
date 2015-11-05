#include "Connection.h"

#include <utility>

using namespace std;


Connection::Connection(Socket&& socket)
	:socket(move(socket))
{
	// TODO Auto-generated constructor stub

}

Connection::~Connection() {
	// TODO Auto-generated destructor stub
}

