#pragma once 
#include "code/utility/BaseProcessor.h"

class  MoveProcessor : public BaseProcessor
{
public:
	MoveProcessor();
	 ~MoveProcessor();

private:

	virtual void Process(std::shared_ptr<MessageRoute> spRoute, std::shared_ptr<google::protobuf::Message> spMsg);

};
