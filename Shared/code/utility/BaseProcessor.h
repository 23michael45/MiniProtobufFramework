#pragma once 
#include "code/protobuf/BaseCmd.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>


class MessageRoute;

class BaseProcessor
{
public:
	BaseProcessor() {}

	virtual void Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg) = 0;
};