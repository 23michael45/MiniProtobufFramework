#pragma once 
#include "code/utility/BaseProcessor.h"

class  SigninProcessor : public BaseProcessor
{
public:
	 SigninProcessor();
	 ~SigninProcessor();

private:

	virtual void Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg);

};
