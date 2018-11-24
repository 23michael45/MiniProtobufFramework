#include "BaseSocketConnection.h"

BaseSocketConnection::BaseSocketConnection(std::shared_ptr<MessageRoute> spmr):m_spMessageRoute(spmr)
{
	m_spMessageRoute->m_fSendFunc = std::bind(&BaseSocketConnection::SendData, this, std::placeholders::_1);
}

void BaseSocketConnection::ReadData(asio::streambuf& buf)
{
	m_spMessageRoute->Receive(buf);
}
void BaseSocketConnection::SendData(asio::streambuf& buf)
{
	std::cout << "Base Send" << std::endl;
}
