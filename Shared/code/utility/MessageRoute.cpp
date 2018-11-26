#include "MessageRoute.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>




using namespace std;
using namespace BaseCmd;
using namespace google;



std::mutex gslobalMutex;


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
	totalSend = 0;
	totalSend2 = 0;
}

void MessageRoute::Receive(asio::streambuf& buf)
{
	std::lock_guard<std::mutex> lock(mRecvMutex);
	std::istream is(&buf);

	std::ostream oshold(&mDataStreamReceive);
	auto oos = new google::protobuf::io::OstreamOutputStream(&oshold);
	auto cos = new google::protobuf::io::CodedOutputStream(oos);


	while (buf.size()> 0)
	{
		int readSize = std::min((int)(buf.size()), (int)TEMP_BUFFER_SIZE);
		is.read(mTempBuffer, readSize);

		cos->WriteRaw(mTempBuffer, readSize);
		oshold.flush();
	}

	delete cos;
	delete oos;


}

void MessageRoute::Process()
{
	ProcessRecv(mPreRecivesTotalLen);
	ProcessSend();

}

bool MessageRoute::ProcessRecv(int& preTotalLen)
{
	std::lock_guard<std::mutex> lock(mRecvMutex);
	if (mDataStreamReceive.size() < sizeof(int))
	{
		/*gslobalMutex.lock();
		std::cout << std::this_thread::get_id() << "No Data" << std::endl;
		gslobalMutex.unlock();*/
		return false;
	}

	std::istream ishold(&mDataStreamReceive);

	std::shared_ptr<protobuf::io::IstreamInputStream> iis;
	std::shared_ptr<google::protobuf::io::CodedInputStream> cis;

	google::protobuf::uint32 totalLen = 0;
	if (preTotalLen > 0)
	{
		totalLen = preTotalLen;
		if (mDataStreamReceive.size() < totalLen - sizeof(int))
		{
			//std::cout << "not enough buf len" << std::endl;
			return false;
		}
	}
	else
	{
		iis = make_shared<google::protobuf::io::IstreamInputStream>(&ishold, sizeof(int));
		cis = make_shared<google::protobuf::io::CodedInputStream>(iis.get());


		cis->ReadLittleEndian32(&totalLen);

		if (mDataStreamReceive.size() < totalLen - sizeof(int))
		{
			preTotalLen = totalLen;
			//std::cout << "not enough buf len" << std::endl;


			//delete cis;
			//delete iis;
			return false;
		}

		/*delete cis;
		delete iis;*/

		cis.reset();
		iis.reset();
	}




	iis = make_shared<google::protobuf::io::IstreamInputStream>(&ishold, totalLen - sizeof(int));
	cis = make_shared<google::protobuf::io::CodedInputStream>(iis.get());

	google::protobuf::uint32 headLen = 0;
	cis->ReadLittleEndian32(&headLen);

	cis->ReadRaw(mTempBuffer, headLen);

	CmdType cmdType;
	cmdType.ParseFromArray(mTempBuffer,headLen);



	int bodyLen = totalLen - headLen - 8;

	cis->ReadRaw(mTempBuffer, bodyLen);

	auto spmsg = createMessage(cmdType.type());
	spmsg->ParseFromArray(mTempBuffer,bodyLen);

	//std::cout << cmdType.type() << " revbuf:" << mDataStreamReceive.size() <<'\n' << std::endl;
	
	//do dispatch
	if (m_spMessageDispatcher)
	{
		m_spMessageDispatcher->Dispatch(shared_from_this(),cmdType.type(), spmsg);
	}


	preTotalLen = 0;

	/*delete iis;
	delete cis;*/
	
	return true;
	
}

bool MessageRoute::Send(protobuf::Message& msg)
{

	std::lock_guard<std::mutex> lock(mSendMutex);

	string stype = msg.GetTypeName();

	int bodylen = msg.ByteSize();


	CmdType cmdType;
	cmdType.set_type(stype);
	cmdType.set_size(bodylen);


	int headlen = cmdType.ByteSize();

	std::ostream os(&mDataStreamSend);

	int totallen = 8 + headlen + bodylen;


	std::ostream oshold(&mDataStreamSend);
	auto oos = new google::protobuf::io::OstreamOutputStream(&oshold);
	auto cos = new google::protobuf::io::CodedOutputStream(oos);

	cos->WriteLittleEndian32(totallen);
	cos->WriteLittleEndian32(headlen);

	cmdType.SerializeToCodedStream(cos);
	msg.SerializeToCodedStream(cos);


	delete cos;
	delete oos;

	//std::cout << stype << std::endl;

	//totalSend += totallen;// mDataStreamSend.size();
	//std::cout << "MessageRoute total send:" << totalSend << std::endl;

	return true;
}


bool MessageRoute::ProcessSend()
{
	if (mDataStreamSend.size() == 0)
	{
		return false;
	}
	std::lock_guard<std::mutex> lock(mSendMutex);

	//std::cout << mDataStreamSend.size() << std::endl;

	//auto spbuf = std::shared_ptr<asio::streambuf>(new asio::streambuf);


	//totalSend2 += mDataStreamSend.size();
	//std::cout << "MessageRoute total send2:" << totalSend2 << std::endl;
	m_fSendFunc(mDataStreamSend);


	//asio::streambuf buf;

	//std::istream is(&mDataStreamSend);

	//std::ostream oshold(&buf);
	//auto oos = new google::protobuf::io::OstreamOutputStream(&oshold);
	//auto cos = new google::protobuf::io::CodedOutputStream(oos);


	//while (mDataStreamSend.size() > 0)
	//{
	//	int readSize = std::min((int)(mDataStreamSend.size()), (int)TEMP_BUFFER_SIZE);
	//	is.read(mTempBuffer, readSize);

	//	cos->WriteRaw(mTempBuffer, readSize);
	//	oshold.flush();
	//}

	//delete cos;
	//delete oos;
	return true;
}