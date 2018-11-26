#pragma once
#include <map>
#include <memory.h>

#include "code/protobuf/BaseCmd.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "code/utility//BaseProcessor.h"


class  BaseMessageDispatcher
{
public:
	 BaseMessageDispatcher();
	~BaseMessageDispatcher();


	virtual void Dispatch(std::shared_ptr<MessageRoute> spRoute, std::string typeName,std::shared_ptr<google::protobuf::Message> spMsg);

protected:

	std::map<std::string, std::shared_ptr<BaseProcessor>> m_ProcessorMap;


};

