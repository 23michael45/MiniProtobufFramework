#include <iostream>
#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"
#include "eventpp/eventqueue.h"
#include "TcpClient.h"

#include "protobuf/BaseCmd.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace std;
using namespace BaseCmd;
using namespace google;


void CallbackList()
{


	eventpp::CallbackList<void(const std::string &, const bool)> callbackList;

	callbackList.append([](const std::string & s, const bool b) {
		std::cout << std::boolalpha << "Got callback 1, s is " << s.c_str() << " b is " << b << std::endl;
	});
	callbackList.append([](std::string s, int b) {
		std::cout << std::boolalpha << "Got callback 2, s is " << s.c_str() << " b is " << b << std::endl;
	});
	callbackList("Hello world", true);

}


void EventDispatcher()
{
	eventpp::EventDispatcher<int, void()> dispatcher;
	dispatcher.appendListener(3, []() {
		std::cout << "Got event 3." << std::endl;
	});
	dispatcher.appendListener(5, []() {
		std::cout << "Got event 5." << std::endl;
	});
	dispatcher.appendListener(5, []() {
		std::cout << "Got another event 5." << std::endl;
	});
	// dispatch event 3
	dispatcher.dispatch(3);
	// dispatch event 5
	dispatcher.dispatch(5);
}
void func1(const std::string& s, const bool b) {
	std::cout << std::boolalpha << "Got event 3, s is " << s << " b is " << b << std::endl;
}

void EventQueue()
{
	eventpp::EventQueue<int, void(const std::string &, const bool)> queue;

	queue.appendListener(3, func1);
	queue.appendListener(5, [](const std::string& s, const bool b) {
		std::cout << std::boolalpha << "Got event 5, s is " << s << " b is " << b << std::endl;
	});

	// The listeners are not triggered during enqueue.
	queue.enqueue(3, "Hello", true);
	queue.enqueue(5, "World", false);

	// Process the event queue, dispatch all queued events.
	queue.process();
}

void bindfunc()
{
	using namespace std;
	auto fun = [](int *array, int n, int num) {
		for (int i = 0; i < n; i++)
		{
			if (array[i] > num)
				cout << array[i] << ends;
		}
		cout << endl;
	};
	int array[] = { 1, 3, 5, 7, 9 };
	//_1，_2 是占位符
	auto fun1 = bind(fun, std::placeholders::_1, std::placeholders::_2, 5);
	//等价于调用fun(array, sizeof(array) / sizeof(*array), 5);
	fun1(array, sizeof(array) / sizeof(*array));
	cin.get();
}

#define  MAX_BUFFER_SIZE 1024
int main(int argc, char* argv[])
{


	try
	{
		std::string host = "127.0.0.1";
		std::string port = "80";

		asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints =
			resolver.resolve(host, port);

		tcp::socket socket(io_context);
		asio::connect(socket, endpoints);

		for (;;)
		{
			char buff[MAX_BUFFER_SIZE];

			reqSignin req;
			sUser* user = req.add_user();
			user->set_name("michael");
			user->set_id(99);
			int bodyLen = req.ByteSize();

			CmdType cmdHead;
			cmdHead.set_type(req.GetTypeName());
			cmdHead.set_size(bodyLen);
			int headLen = cmdHead.ByteSize();

			int totalLen = headLen + bodyLen + 8;



			asio::streambuf sb;
			std::ostream os(&sb);

			os.write((const char*)&totalLen, sizeof(int));
			os.write((const char*)&headLen, sizeof(int));

			memset(buff, 0, MAX_BUFFER_SIZE);
			if (cmdHead.SerializeToArray(buff, MAX_BUFFER_SIZE))
			{
				os.write((const char*)buff, cmdHead.ByteSize());
			}
			memset(buff, 0, MAX_BUFFER_SIZE);
			if (req.SerializeToArray(buff, MAX_BUFFER_SIZE))
			{

				os.write((const char*)buff, req.ByteSize());
			}

			memset(buff, 0, MAX_BUFFER_SIZE);


			//memcpy_s((void*)buf.data(), sizeof(s), s.c_str(), sizeof(s));

			asio::error_code error;

			size_t len = socket.write_some(asio::buffer(sb.data(), totalLen), error);

			len = socket.read_some(asio::buffer(buff), error);
			std::cout.write(buff, len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}


