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


	//virtual void SendData(std::shared_ptr<asio::streambuf> spbuf) override;
	virtual void SendData(asio::streambuf& buf) override;
	

	void write_handler(const asio::error_code& ec,	std::size_t bytes_transferred);
	void Receive();
	void Send(asio::streambuf& buf);
	void read_handler(const asio::error_code& ec);

	void Close();
	void Process();

private:

	std::mutex socketMutex;

	tcp::socket socket;
	tcp::resolver r;
	asio::streambuf receivebuf;
	asio::io_context& io_context_;

	size_t totlasendwill;
	size_t totlasend;

	bool closed;

	bool connected;
};
