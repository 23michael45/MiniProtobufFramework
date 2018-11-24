#include "TcpClient.h"

TcpClient::TcpClient(asio::io_context& io_context, std::shared_ptr<MessageRoute> spmr) :
	BaseSocketConnection(spmr), io_context_(io_context), socket(io_context), r(io_context)
{



}

void TcpClient::Connect(std::string ip, std::string port)
{
	tcp::resolver::query q(ip, port);
	r.async_resolve(q, bind(&TcpClient::resolve_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

}
void TcpClient::resolve_handler(
	const asio::error_code& ec,
	tcp::resolver::iterator i)
{
	if (!ec)
	{
		asio::async_connect(socket, i, bind(&TcpClient::connect_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	}
	else
	{

		std::cerr << ec.message() << std::endl;
	}
}
void TcpClient::connect_handler(
	const asio::error_code& ec,
	tcp::resolver::iterator i)
{
   	Receive();
}

void TcpClient::SendData(asio::streambuf& buf)
{
	PostSend(buf);
}


void TcpClient::Receive()
{
	asio::async_read(socket, receivebuf, bind(&TcpClient::read_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}
void TcpClient::read_handler(
	const asio::error_code& ec,
	std::size_t bytes_transferred)
{
	ReadData(receivebuf);
	Receive();
}

void TcpClient::PostSend(asio::streambuf& buf)
{
	io_context_.post(std::bind(&TcpClient::RealSend, shared_from_this()));
}
void TcpClient::RealSend()
{

	asio::async_write(socket,
		m_spMessageRoute->GetSendBuffer(), std::bind(&TcpClient::write_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}
void TcpClient::write_handler(
	const asio::error_code& ec,
	std::size_t bytes_transferred)
{
	if (!ec)
	{
		std::cout << "Write buf Success" << std::endl;

	}
	else
	{

	}
}