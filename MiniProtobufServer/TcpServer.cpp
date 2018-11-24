#include "TcpServer.h"

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

void TcpConnection::start()
{
	std::cout << "Connection Established!" << std::endl;
	std::string timestring  = make_daytime_string();
	// Start reading messages from the server
	start_read();

}



// Reading messages from the server
void TcpConnection::start_read()
{

	asio::async_read(socket_, input_buffer_,//asio::buffer(bufread),
		asio::transfer_at_least(1),
		std::bind(&TcpConnection::handle_read, shared_from_this(),
			std::placeholders::_1));


}


void TcpConnection::handle_write(const asio::error_code& error/*error*/,
	size_t size/*bytes_transferred*/)
{
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

	if (!ec)
	{
		ReadData(input_buffer_);
		start_read();
	}
	else
	{
		std::cout << "Error on receive: " << ec.message() << "\n";
	}
}