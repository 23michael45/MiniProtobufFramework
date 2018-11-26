#include "TcpClient.h"
#include "code/utility/BaseMessageDispatcher.h"
#include "ClientMessageDispatcher.h"

std::mutex globalMutex;

TcpClient::TcpClient(asio::io_context& io_context, std::shared_ptr<MessageRoute> spmr) :
	BaseSocketConnection(spmr), io_context_(io_context), socket(io_context), r(io_context)
{
	spmr->SetMessageDispatcher(std::make_shared<ClientMessageDispatcher>());
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
	//std::cout << "Write buf will:" << buf.size() << "   total send will:" << totlasendwill << std::endl;
	asio::async_write(socket, buf, std::bind(&TcpClient::write_handler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}


void TcpClient::Receive()
{
	asio::async_read(socket, receivebuf, asio::transfer_at_least(1), bind(&TcpClient::read_handler, shared_from_this(), std::placeholders::_1));
}
void TcpClient::read_handler(const asio::error_code& ec)
{
	ReadData(receivebuf);
	Receive();
}

void TcpClient::write_handler(
	const asio::error_code& ec,
	std::size_t bytes_transferred)
{
	std::lock_guard<std::mutex> lock(m_spMessageRoute->mSendMutex);


	std::lock_guard<std::mutex> glock(globalMutex);
	if (!ec)
	{
	
		totlasend += bytes_transferred;


		std::cout << "TID:" << std::this_thread::get_id() << "   Write buf Success:" << bytes_transferred << "   total send:" << totlasend << std::endl;
		
	}
	else
	{

	}
}