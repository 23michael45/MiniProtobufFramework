#ifndef HEADER_BASE_SOCKET_CONNECTION
#define HEADER_BASE_SOCKET_CONNECTION

#include "asio/asio/include/asio.hpp"
#include "MessageRoute.h"



class BaseSocketConnection 
{
public: 
	BaseSocketConnection();
	
	virtual void ReadData(asio::streambuf& buf);
	virtual void SendData(asio::streambuf& buf);


private:


	MessageRoute m_pMessageRoute;
};

#endif