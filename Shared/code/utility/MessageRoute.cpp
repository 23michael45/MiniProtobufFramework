#include "MessageRoute.h"
#include <iostream>




using namespace std;
using namespace BaseCmd;
using namespace google;




std::shared_ptr<protobuf::Message> createMessage(const std::string &type_name)
{
	auto pmsg = std::shared_ptr<protobuf::Message>(NULL);
	const protobuf::Descriptor* descriptor =
		protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor) {
		const protobuf::Message* prototype =
			protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			pmsg = std::shared_ptr<protobuf::Message>(prototype->New());
		}
	}
	return pmsg;
}

MessageRoute::MessageRoute()
{
}

void MessageRoute::Receive(asio::streambuf& buf)
{
	std::istream is(&buf);
	std::istreambuf_iterator<char> isb(is), end;

	std::ostreambuf_iterator<char> osb(&mDataStreamReceive);

	

	while (isb != end)
		*osb++ = *isb++;

	Process();

}
void MessageRoute::Process()
{

	std::istream is(&mDataStreamReceive);

	is.read(mTempBuffer, sizeof(int));
	int totalLen = *(int*)mTempBuffer;

	if (mDataStreamReceive.size() < totalLen - 4)
	{
		return;
	}


	is.read(mTempBuffer, sizeof(int));
	int headLen = *(int*)mTempBuffer;


	is.read(mTempBuffer, headLen);

	CmdType cmdType;
	cmdType.ParseFromArray(mTempBuffer,headLen);

	int bodyLen = totalLen - headLen - 8;
	is.read(mTempBuffer, bodyLen);
	auto spmsg = createMessage(cmdType.type());

	spmsg->ParseFromArray(mTempBuffer,bodyLen);
	cout << cmdType.type() << "\n" << endl;

	Send(spmsg);

	spmsg.reset();
	
}

bool MessageRoute::Send(std::shared_ptr<protobuf::Message> spmsg)
{
	string stype = spmsg->GetTypeName();

	int bodylen = spmsg->ByteSize();


	CmdType cmdType;
	cmdType.set_type(stype);
	cmdType.set_size(bodylen);


	int headlen = cmdType.ByteSize();

	std::ostream os(&mDataStreamSend);

	int totallen = 8 + headlen + bodylen;

	os.write((const char*)&totallen, sizeof(int));
	os.write((const char*)&headlen, sizeof(int));

	std::string headstring;
	cmdType.SerializeToOstream(&os);


	std::string bodystring;
	spmsg->SerializeToOstream(&os);


	m_fSendFunc(mDataStreamSend);


	return true;
}