#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include "SharedHeader.h"

using asio::ip::tcp;


class TcpConnection  
	: public std::enable_shared_from_this<TcpConnection>,public BaseSocketConnection
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;

	static pointer create(asio::io_context& io_context)
	{
		return pointer(new TcpConnection(io_context));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start();
	void start_read();
	void handle_read(const asio::error_code& ec);
	void handle_write(const asio::error_code&, size_t);

	virtual void ReadData(asio::streambuf& buf) override
	{
		BaseSocketConnection::ReadData(buf);
	}
	virtual void SendData(asio::streambuf& buf) override;
	

private:
	TcpConnection(asio::io_context& io_context)
		: socket_(io_context)
	{
	}

	tcp::socket socket_;
	asio::streambuf input_buffer_;
	char bufread[256];

};

class TcpServer
{
public:
	TcpServer(asio::io_context& io_context)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), 88))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		TcpConnection::pointer new_connection =
			TcpConnection::create(acceptor_.get_executor().context());

		acceptor_.async_accept(new_connection->socket(),
			std::bind(&TcpServer::handle_accept, this, new_connection,
				std::placeholders::_1));
	}

	void handle_accept(TcpConnection::pointer new_connection,
		const asio::error_code& error)
	{
		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}


	tcp::acceptor acceptor_;
};
