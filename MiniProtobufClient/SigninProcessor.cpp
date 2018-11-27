#include "SigninProcessor.h"

using namespace BaseCmd;
SigninProcessor::SigninProcessor()
{
}

SigninProcessor::~SigninProcessor()
{
}

void SigninProcessor::Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg)
{
	rspSignin* p_rsp = reinterpret_cast<rspSignin*>(spMsg.get());

	std::string name = p_rsp->name();
	bool succ = p_rsp->succ();
	//std::cout << "signin rsp:" << name << " Succ:" << succ <<  std::endl;
}