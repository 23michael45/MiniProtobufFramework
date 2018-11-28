#include "MoveProcessor.h"
#include "code/utility/MessageRoute.h"

using namespace BaseCmd;
MoveProcessor::MoveProcessor()
{
}

MoveProcessor::~MoveProcessor()
{
}

void MoveProcessor::Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg)
{
	reqMove* p_req = reinterpret_cast<reqMove*>(spMsg.get());

	float forward = p_req->forward();
	float angle = p_req->angle();

	std::cout << "forward::" << forward << " angle:" << angle << std::endl;


	//Do Robot move


	//Rsp
	rspMove rsp;
	rsp.set_error("Move Ok No Error");

	spRoute->Send(rsp);
}