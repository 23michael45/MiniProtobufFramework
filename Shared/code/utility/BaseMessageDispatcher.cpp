
#include "BaseMessageDispatcher.h"



using namespace google::protobuf;
using namespace BaseCmd;



BaseMessageDispatcher::BaseMessageDispatcher()
{


}

BaseMessageDispatcher::~BaseMessageDispatcher()
{
}

void BaseMessageDispatcher::Dispatch(std::shared_ptr<MessageRoute> spRoute,std::string typeName,std::shared_ptr<google::protobuf::Message> spMsg)
{
	if (m_ProcessorMap.find(typeName) != m_ProcessorMap.end())
	{
		m_ProcessorMap[typeName]->Process(spRoute,spMsg);
	}
}