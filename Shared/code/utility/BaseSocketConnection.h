#ifndef HEADER_BASE_SOCKET_CONNECTION
#define HEADER_BASE_SOCKET_CONNECTION

#include "asio/asio/include/asio.hpp"
#include "MessageRoute.h"



class BaseSocketConnection 
{
public: 
	BaseSocketConnection(std::shared_ptr<MessageRoute> spmr);
	virtual ~BaseSocketConnection()
	{
		std::cout << "~BaseSocketConnection" << std::endl;
		m_spMessageRoute.reset();
	}
	virtual void ReadData(asio::streambuf& buf);
	//virtual void SendData(std::shared_ptr<asio::streambuf> spbuf);
	virtual void SendData(asio::streambuf& spbuf);



protected:
	std::shared_ptr<MessageRoute> m_spMessageRoute;
};

#endif