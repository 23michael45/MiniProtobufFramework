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

void TcpConnection::start()
{
	totalrec = 0;
	std::cout << "Connection Established!" << std::endl;
	std::string timestring  = make_daytime_string();
	// Start reading messages from the server
	start_read();


	sp_executor_context = std::make_shared<asio::io_context>();
	_thgExecutors.create_thread(bind(&TcpConnection::WorkerThreadCallback,shared_from_this(),sp_executor_context));
}
/* WorkerThread Callback Skeleton */
void TcpConnection::WorkerThreadCallback(std::shared_ptr<asio::io_context> ios)
{
	/*while (true)
	{*/
		try
		{
			asio::error_code errorCode;

			sp_executor_context->post(std::bind(&TcpConnection::Process, shared_from_this()));
			sp_executor_context->run(errorCode);
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
	do 
	{
		m_spMessageRoute->Process();
	} while (!error);

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
		std::cout << "Send: " << size << "\n";
	}
	else
	{
		std::cout << "Error on send: " << ec.message() << "\n";
	}
}

void TcpConnection::SendData(asio::streambuf& buf)
{
	asio::async_write(socket_, buf,
		std::bind(&TcpConnection::handle_write, shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2));
}

// When stream is received, handle the message from the client
void TcpConnection::handle_read(const asio::error_code& ec)
{

	//std::lock_guard<std::mutex> lock(m_spMessageRoute->mRecvMutex);
	
	if (!ec)
	{
		totalrec += input_buffer_.size();
		//std::cout << "receive: " << input_buffer_.size() << " totalrec:" << totalrec<<  "\n";
		ReadData(input_buffer_);
		start_read();
	}
	else
	{
		error = true;
		//std::cout << "Error on receive: " << ec.message() << "\n";
	}
}