#include "TcpClient.h"

TcpClient::TcpClient(asio::io_context& io_context, std::shared_ptr<MessageRoute> spmr) :
	BaseSocketConnection(spmr), io_context_(io_context), socket(io_context), r(io_context)
{

	totlasend = 0;
	totlasendwill = 0;

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

	io_context_.post(std::bind(&TcpClient::Process, shared_from_this()));
}

void TcpClient::Process()
{
	m_spMessageRoute->Process();

	io_context_.post(std::bind(&TcpClient::Process, shared_from_this()));
}


//void TcpClient::SendData(std::shared_ptr<asio::streambuf> spbuf)
//{
//	asio::async_write(socket,*(spbuf.get()), std::bind(&TcpClient::write_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
//}
void TcpClient::SendData(asio::streambuf& buf)
{
	totlasendwill += buf.size();
	std::cout << "Write buf will:" << buf.size() << "   total send will:" << totlasendwill << std::endl;
	asio::async_write(socket, buf, std::bind(&TcpClient::write_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
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

void TcpClient::write_handler(
	const asio::error_code& ec,
	std::size_t bytes_transferred)
{
	//std::lock_guard<std::mutex> lock(m_spMessageRoute->mSendMutex);
	if (!ec)
	{
	
		totlasend += bytes_transferred;

		std::cout << "Write buf Success:" << bytes_transferred << "   total send:" << totlasend << std::endl;
		
	}
	else
	{

	}
}