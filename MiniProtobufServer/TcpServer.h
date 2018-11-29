#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include "SharedHeader.h"
#include "ServerMessageDispatcher.h"

using asio::ip::tcp;

class TcpServer;
class TcpConnection
	: public std::enable_shared_from_this<TcpConnection>,public BaseSocketConnection
{
public:
	TcpConnection(asio::io_context& io_context, std::shared_ptr<MessageRoute> spmr)
		: BaseSocketConnection(spmr), socket_(io_context), io_context_(io_context)
	{
		error = false;
		spmr->SetMessageDispatcher(std::make_shared<ServerMessageDispatcher>());
	}
	TcpConnection(asio::io_context& io_context)
		: TcpConnection(io_context,std::make_shared<MessageRoute>())
	{
	}
	~TcpConnection();


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
	void Send(asio::streambuf& buf);
	void Process();

	/* WorkerThread Callback Skeleton */
	void WorkerThreadCallback(std::shared_ptr<asio::io_context> ios);

private:


	std::mutex socketSendMutex;
	std::mutex socketRecvMutex;//Receive is Sequence Operation,donot use mutex;

	tcp::socket socket_;
	asio::streambuf input_buffer_;
	asio::io_context& io_context_;
	std::size_t totalrec;

	std::shared_ptr<asio::io_context> sp_executor_context;
	//std::shared_ptr < std::thread> process_thread;
	std::thread process_thread;
	bool error;
};

class TcpServer : public std::enable_shared_from_this<TcpServer>
{
public:
	TcpServer(asio::io_context& io_context)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), 88)), io_context_(io_context)
	{
	}
	~TcpServer()
	{

	}

	void start()
	{
		io_context_.post(std::bind(&TcpServer::Process, shared_from_this()));
		start_accept();
	}
	void Process()
	{

		/*int count = sp_new_connection.use_count();
		if (count == 0)
		{
			start_accept();
		}*/

		io_context_.post(std::bind(&TcpServer::Process, shared_from_this()));
	}
private:
	void start_accept()
	{
		sp_new_connection = std::make_shared<TcpConnection>(io_context_);

		acceptor_.async_accept(sp_new_connection->socket(),
			std::bind(&TcpServer::handle_accept, this, sp_new_connection,
				std::placeholders::_1));

		sp_new_connection.reset();
	}

	void handle_accept(std::shared_ptr<TcpConnection> sp,
		const asio::error_code& error)
	{
		int count = -0;
		if (!error)
		{
		
			sp->start();
		}

		while (sp_new_connection.use_count() != 0)
		{
		}
		start_accept();
	}

	std::shared_ptr<TcpConnection> sp_new_connection;
	asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};
