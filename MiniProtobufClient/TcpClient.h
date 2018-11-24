#pragma once
#include <iostream>
#include "asio/asio/include/asio.hpp"
#include "code/utility/BaseSocketConnection.h"

using asio::ip::tcp;

#define MAX_BUFFER_SIZE 1024
class TcpClient : public std::enable_shared_from_this<TcpClient>,public BaseSocketConnection
{
public:
	TcpClient(asio::io_context& io_context, std::shared_ptr<MessageRoute> spmr);


	void Connect(std::string ip, std::string port);
	void resolve_handler(const asio::error_code& ec,tcp::resolver::iterator i);
	void connect_handler(const asio::error_code& ec, tcp::resolver::iterator i);


	void SendData(asio::streambuf& buf);

	void write_handler(const asio::error_code& ec,	std::size_t bytes_transferred);
	void Receive();
	void read_handler(const asio::error_code& ec,	std::size_t bytes_transferred);

	void PostSend(asio::streambuf& buf);
	void RealSend();


private:



	tcp::socket socket;
	tcp::resolver r;
	asio::streambuf receivebuf;
	asio::io_context& io_context_;
};
