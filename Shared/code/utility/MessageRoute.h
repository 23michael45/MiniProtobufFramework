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

#include "code/utility//BaseMessageDispatcher.h"
using namespace google;

#define TEMP_BUFFER_SIZE 1024


class MessageRoute : public std::enable_shared_from_this<MessageRoute>
{
public: 
	MessageRoute();
	~MessageRoute()
	{
		std::cout << "MessageRoute Delete" << std::endl;
	}
	
	void SetMessageDispatcher(std::shared_ptr<BaseMessageDispatcher> sp)
	{
		m_spMessageDispatcher = sp;
	}
	

	void Receive(asio::streambuf& buf);
	bool Send(protobuf::Message& spmsg);



	//std::function<void(std::shared_ptr<asio::streambuf>)> m_fSendFunc;
	std::function<void(asio::streambuf&)> m_fSendFunc;
	asio::streambuf& GetSendBuffer() {
		return mDataStreamSend;
	}
	void Process();

	std::mutex mSendMutex;
	std::mutex mRecvMutex;
private:

	bool ProcessRecv(int& preTotalLen);
	bool ProcessSend();


	int mPreRecivesTotalLen;
	asio::streambuf mDataStreamReceive;
	asio::streambuf mDataStreamSend;



	char mTempBuffer[TEMP_BUFFER_SIZE];

	size_t totalSend;
	size_t totalSend2;


	std::shared_ptr<BaseMessageDispatcher> m_spMessageDispatcher;
};

#endif