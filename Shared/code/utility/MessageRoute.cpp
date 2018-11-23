#include "MessageRoute.h"
#include <iostream>




using namespace std;
using namespace BaseCmd;
using namespace google;




protobuf::Message *createMessage(const std::string &type_name)
{
	protobuf::Message* message = NULL;
	const protobuf::Descriptor* descriptor =
		protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor) {
		const protobuf::Message* prototype =
			protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}
	return message;
}

MessageRoute::MessageRoute()
{
}

void MessageRoute::Receive(asio::streambuf& buf)
{
	std::istream is(&buf);
	std::istreambuf_iterator<char> isb(is), end;

	std::ostreambuf_iterator<char> osb(&mDataStream);

	

	while (isb != end)
		*osb++ = *isb++;

	Process();

}
void MessageRoute::Process()
{

	std::istream is(&mDataStream);

	is.read(mTempBuffer, sizeof(int));
	int totalLen = *(int*)mTempBuffer;

	if (mDataStream.size() < totalLen - 4)
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
	protobuf::Message* msg = createMessage(cmdType.type());

	msg->ParseFromArray(mTempBuffer,bodyLen);
	string stype = msg->GetTypeName();
	cout << cmdType.type() << "\n" << endl;

	Send(msg);
	
}

template <typename  T>//	where T: public protobuf::Message
bool MessageRoute::Send(T* msg)
{
	protobuf::Message *m = reinterpret_cast<protobuf::Message*>(msg);
	string stype = m->GetTypeName();

	int bodylen = m->ByteSize();


	CmdType cmdType;
	cmdType.set_type(stype);
	cmdType.set_size(bodylen);


	int headlen = cmdType.ByteSize();

	asio::streambuf sb;
	std::ostream os(&sb);

	int totallen = 8 + headlen + bodylen;

	os.write((const char*)&totallen, sizeof(int));
	os.write((const char*)&headlen, sizeof(int));

	std::string headstring;
	cmdType.SerializeToOstream(&os);


	std::string bodystring;
	m->SerializeToOstream(&os);


	m_fSendFunc(sb);


	return true;
}