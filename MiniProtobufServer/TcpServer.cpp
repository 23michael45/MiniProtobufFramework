#include "TcpServer.h"
#include <map>
#include "asio/asio/include/asio.hpp"
#include "asio/asio/include/asio/deadline_timer.hpp"

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}
TcpConnection::~TcpConnection()
{
	sp_executor_context.reset();
	
	//process_thread.reset();
	//delete p_process_thread;
	std::cout << "Connect Lost" << std::endl;

}
void TcpConnection::start()
{
	totalrec = 0;
	std::string ip = socket().remote_endpoint().address().to_string();
	std::cout << "Connection Established!" << ip << std::endl;
	std::string timestring  = make_daytime_string();
	// Start reading messages from the server
	
	//sp_executor_context = std::make_shared<asio::io_context>(); 

	//asio::io_context::strand strand(*sp_executor_context.get());

	//start_read();
	//sp_executor_context->post(std::bind(&TcpConnection::start_read, shared_from_this()));
	io_context_.post(std::bind(&TcpConnection::start_read, shared_from_this()));



	io_context_.post(std::bind(&TcpConnection::Process, shared_from_this()));
	//sp_executor_context->post(std::bind(&TcpConnection::Process, shared_from_this()));
	//process_thread_group.create_thread(bind(&TcpConnection::WorkerThreadCallback,shared_from_this(),sp_executor_context));

	//process_thread = std::make_shared<std::thread>(bind(&TcpConnection::WorkerThreadCallback, shared_from_this(), sp_executor_context));
	//process_thread = std::thread(bind(&TcpConnection::WorkerThreadCallback, shared_from_this(), sp_executor_context));
	//process_thread.detach();
}
/* WorkerThread Callback Skeleton */
void TcpConnection::WorkerThreadCallback(std::shared_ptr<asio::io_context> ios)
{
	/*while (true)
	{*/
		try
		{
			asio::error_code errorCode;
			ios->run(errorCode);
			ios->stop();
			if (errorCode)
			{
				/*lockStream();
				LOG_ERR() << " Error: " << errorCode.message() << endl;
				unlockStream();*/
			}
		}
		catch (std::exception& ex)
		{
			/*lockStream();
			LOG_ERR() << " Exception: " << ex.what() << endl;
			unlockStream();*/
		}
	//}
}
void TcpConnection::Process()
{
		m_spMessageRoute->Process();

	if (!error)
	{
		io_context_.post(std::bind(&TcpConnection::Process, shared_from_this()));
	}
}

// Reading messages from the server
void TcpConnection::start_read()
{

	asio::async_read(socket_, input_buffer_,//asio::buffer(bufread),
		asio::transfer_at_least(1),
		std::bind(&TcpConnection::handle_read, shared_from_this(),
			std::placeholders::_1));

}


void TcpConnection::handle_write(const asio::error_code& ec/*error*/,
	size_t size/*bytes_transferred*/)
{
	if (!ec)
	{
		//std::cout << "Send: " << size << "\n";
	}
	else
	{
		//std::cout << "Error on send: " << ec.message() << "\n";
	}
}

void TcpConnection::SendData(asio::streambuf& buf)
{
	if (!error)
	{
		io_context_.post(std::bind(&TcpConnection::Send, shared_from_this(), std::ref< asio::streambuf>(buf)));
		//sp_executor_context->post(std::bind(&TcpConnection::Send, shared_from_this(), std::ref< asio::streambuf>(buf)));
	}
}
void TcpConnection::Send(asio::streambuf& buf)
{
	asio::async_write(socket_, buf, std::bind(&TcpConnection::handle_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	buf.consume(buf.size());
}
// When stream is received, handle the message from the client
void TcpConnection::handle_read(const asio::error_code& ec)
{

	//std::lock_guard<std::mutex> lock(m_spMessageRoute->mRecvMutex);
	
	if (!ec)
	{
		totalrec += input_buffer_.size();
		std::cout << "receive: " << input_buffer_.size() << " totalrec:" << totalrec<<  "\n";
		ReadData(input_buffer_);

		if (!error)
		{
			//start_read();
			//sp_executor_context->post(std::bind(&TcpConnection::start_read, shared_from_this()));
			io_context_.post(std::bind(&TcpConnection::start_read, shared_from_this()));

		}
	}
	else
	{
		error = true;
		//std::cout << "Error on receive: " << ec.message() << "\n";
	}
}