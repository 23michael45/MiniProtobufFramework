#include "MoveProcessor.h"


using namespace BaseCmd;
MoveProcessor::MoveProcessor()
{
}

MoveProcessor::~MoveProcessor()
{
}

void MoveProcessor::Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg)
{
	rspMove* p_rsp = reinterpret_cast<rspMove*>(spMsg.get());

	std::string error = p_rsp->error();
	std::cout << "move rsp:" << error << std::endl;

}