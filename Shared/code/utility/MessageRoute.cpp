#include "MessageRoute.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>




using namespace std;
using namespace BaseCmd;
using namespace google;

extern::mutex globalMutex;

char head[] = { "pbh" };
int hlen = 4;

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
	std::lock_guard<std::mutex> lock(mMutexRec);
	std::istream is(&buf);

	std::ostream oshold(&mDataStreamReceive);
	google::protobuf::io::OstreamOutputStream oos(&oshold);
	google::protobuf::io::CodedOutputStream cos(&oos);


	while (buf.size()> 0)
	{
		int readSize = std::min((int)(buf.size()), (int)TEMP_BUFFER_SIZE);
		is.read(mTempBuffer, readSize);

		cos.WriteRaw(mTempBuffer, readSize);
		oshold.flush();
	}


}

void MessageRoute::Process()
{
	ProcessRecv(mPreRecivesTotalLen);
	ProcessSend();

}

bool MessageRoute::ProcessRecv(int& preTotalLen)
{
	try
	{
		google::protobuf::uint32 totalLen = 0;
		char readhead[4] = { 0 };
		std::lock_guard<std::mutex> lock(mMutexRec);
		std::istream ishold(&mDataStreamReceive);
		if (mDataStreamReceive.size() < sizeof(int) * 2)
		{
			/*gslobalMutex.lock();
			std::cout << std::this_thread::get_id() << "No Data" << std::endl;
			gslobalMutex.unlock();*/
			return false;
		}
		else if (preTotalLen > 0)
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

			google::protobuf::io::IstreamInputStream iishead(&ishold, 4);
			google::protobuf::io::CodedInputStream cishead(&iishead);

			cishead.ReadRaw(readhead, 4);


			if (memcmp(head, readhead, 4) != 0)//not head
			{
				preTotalLen = 0;
				return false;
			}
			else//is head
			{
				google::protobuf::io::IstreamInputStream iistotal(&ishold, sizeof(int));
				google::protobuf::io::CodedInputStream cistotal(&iistotal);
				
				cistotal.ReadLittleEndian32(&totalLen);

				if (totalLen < 0)
				{
					return false;
				}

				if (mDataStreamReceive.size() < totalLen - sizeof(int))
				{
					preTotalLen = totalLen;
					return false;
				}
			}

		}

	


		



		google::protobuf::io::IstreamInputStream iis(&ishold, totalLen - sizeof(int));
		google::protobuf::io::CodedInputStream cis(&iis);


		google::protobuf::uint32 headLen = 0;
		cis.ReadLittleEndian32(&headLen);

		if (headLen < 0 || headLen > TEMP_BUFFER_SIZE)
		{
			preTotalLen = 0;
			return false;
		}

		cis.ReadRaw(mTempBufferProcess, headLen);

		CmdType cmdType;
		cmdType.ParseFromArray(mTempBufferProcess, headLen);



		int bodyLen = totalLen - headLen - 8;

		if (bodyLen < 0 || bodyLen > TEMP_BUFFER_SIZE)
		{
			preTotalLen = 0;
			return false;
		}


		cis.ReadRaw(mTempBufferProcess, bodyLen);

		auto spmsg = createMessage(cmdType.type());

		if (spmsg)
		{
			spmsg->ParseFromArray(mTempBufferProcess, bodyLen);

			//std::cout << cmdType.type() << " revbuf:" << mDataStreamReceive.size() <<'\n' << std::endl;

			//do dispatch
			if (m_spMessageDispatcher)
			{
				m_spMessageDispatcher->Dispatch(shared_from_this(), cmdType.type(), spmsg);
			}


		}
		else
		{
			std::cout << "---------------------------------------------------------------------------------------Serilize Error";
			
		}

		preTotalLen = 0;

		/*delete iis;
		delete cis;*/
	}
	catch (std::exception e)
	{
		std::cout <<  e.what() <<"---------------------------------------------------------------------------------------Deserialize Error";

	}
	return true;

}

bool MessageRoute::Send(protobuf::Message& msg)
{

	std::lock_guard<std::mutex> lock(mMutexSend);

	string stype = msg.GetTypeName();

	int bodylen = msg.ByteSize();


	CmdType cmdType;
	cmdType.set_type(stype);
	cmdType.set_size(bodylen);


	int headlen = cmdType.ByteSize();
	
	int totallen = 8 + headlen + bodylen;


	std::ostream oshold(&mDataStreamSend);
	google::protobuf::io::OstreamOutputStream oos(&oshold);
	google::protobuf::io::CodedOutputStream cos(&oos);


	cos.WriteRaw(head,hlen);
	cos.WriteLittleEndian32(totallen);
	cos.WriteLittleEndian32(headlen);

	cmdType.SerializeToCodedStream(&cos);
	msg.SerializeToCodedStream(&cos);


	/*delete cos;
	delete oos;*/

	//std::cout << stype << std::endl;


	std::lock_guard<std::mutex> glock(globalMutex);
	totalSend += totallen + 4;// mDataStreamSend.size();
	std::cout << "MessageRoute total send:" << totalSend << std::endl;

	return true;
}


bool MessageRoute::ProcessSend()
{
	
	std::lock_guard<std::mutex> lock(mMutexSend);
	if (mDataStreamSend.size() == 0)
	{
		return false;
	}
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