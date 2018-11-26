#include "SigninProcessor.h"
#include "code/utility/MessageRoute.h"
using namespace BaseCmd;

SigninProcessor::SigninProcessor()
{
}

SigninProcessor::~SigninProcessor()
{
}

void SigninProcessor::Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg)
{
	reqSignin* p_req= reinterpret_cast<reqSignin*>(spMsg.get());

	std::string name = p_req->user(0).name();



	//Rsp
	rspSignin rsp;
	rsp.set_name(name);
	rsp.set_succ(true);

	spRoute->Send(rsp);

}