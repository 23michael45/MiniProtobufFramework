#ifndef HEADER_MESSAGE_ROUTE
#define HEADER_MESSAGE_ROUTE
#include <queue>
#include <iostream>
#include <sstream>
#include <streambuf>

#include "code/protobuf/BaseCmd.pb.h"
#include "asio/asio/include/asio.hpp"


#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>



#define TEMP_BUFFER_SIZE 4096



class MessageRoute
{
public: 
	MessageRoute();
	

	

	void Receive(asio::streambuf& buf);

	template <typename  T>//	where T: public protobuf::Message
	bool Send(T* msg);


	std::function<void(asio::streambuf&)> m_fSendFunc;
private:

	void Process();


	asio::streambuf mDataStream;

	char mTempBuffer[TEMP_BUFFER_SIZE];

};

#endif