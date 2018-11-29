#include "TcpServer.h"

#include "HttpServer/http_server.hpp"

void TcpServerStart()
{
	try
	{
		asio::io_context io_context;
		std::shared_ptr<TcpServer> sp_server = std::make_shared<TcpServer>(io_context);
		sp_server->start();
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
void HttpServerStart()
{
	try
	{
		std::string  address = "127.0.0.1";
		std::string  port = "89";
		std::string  docroot = "D:/DevelopProj/ProtocolBuffers/MiniProtobufFramework/MiniProtobufFramework/MiniProtobufServer/HttpServer/WebSiteRoot";

		// Initialise the server.
		http::server::server s(address, port, docroot);

		// Run the server until stopped.
		s.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
}


int main()
{
	//HttpServerStart();
	TcpServerStart();


	return 0;
}