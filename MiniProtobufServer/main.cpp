#include "TcpServer.h"


int main()
{

	try
	{
		asio::io_context io_context;
		std::shared_ptr<TcpServer> sp_server =  std::make_shared<TcpServer>(io_context);
		sp_server->start();
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}